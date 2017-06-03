#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QStorageInfo>
#include <QDebug>
#include "showdrives.h"
#include "filescanprocessor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->listWidget->setStyleSheet("QListWidget:item { border: 1px solid black;}"); // установка рамки на каждый виджет в списке

	disks_amount = new QLabel(this);
	ui->statusBar->insertWidget(0, disks_amount, 200); // вставляем надпись в статус-бар

	scan_prc = new FileScanProcessor(this);

	this->refreshVolumes();
	this->detectVolsAmount();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::detectVolsAmount()
{
	int detected_disks = QStorageInfo::mountedVolumes().size(); // получаем количество дисков

	switch (detected_disks % 10) { //сделано для того, чтобы информация была в правильном падеже
		case 1:
			disks_amount->setText("Обнаружен " + QString::number(detected_disks) + " диск");
			break;
		case 2:
		case 3:
		case 4:
			disks_amount->setText("Обнаружено " + QString::number(detected_disks) + " диска");
			break;

		default:
			disks_amount->setText("Обнаружено " + QString::number(detected_disks) + " дисков");
			break;
	}
}

void MainWindow::refreshVolumes()
{
	overall_memory = 0.0;

	for (int i = 0; i < view_list.size(); i++){
		delete view_list.at(i);
	}
	view_list.clear();
	ui->listWidget->clear();

	foreach (QStorageInfo storage, QStorageInfo::mountedVolumes()) // для каждого элемента информации о диске
	{
		ShowDrives *wnd = new ShowDrives(storage, this); // создаем виджет
		view_list.push_back(wnd); // заносим его в список, чтобы контролировать
		overall_memory += (storage.bytesTotal() - storage.bytesFree());

		QListWidgetItem *item = new QListWidgetItem(); // создаем контейнер под запись
		ui->listWidget->addItem(item);
		item->setSizeHint(QSize(50,120));
		ui->listWidget->setItemWidget(item, wnd); // присваиваем виджет контейнеру
	}

	detectVolsAmount();

	ui->label_disksAmount->setText("Всего дисков: " + QString::number(QStorageInfo::mountedVolumes().size()));

	QString unit = ShowDrives::getUnitForMemory(overall_memory);
	ui->label_overall_memory->setText("Объем занятой памяти: " + QString::number(overall_memory, 'f', 2) + " " + unit);

	foreach (QStorageInfo storage, QStorageInfo::mountedVolumes()) // для каждого элемента информации о диске
	{
		qDebug() << storage.rootPath(); //выводим информацию для дебага
		if (storage.isReadOnly())
			qDebug() << "isReadOnly:" << storage.isReadOnly();

		qDebug() << "name:" << storage.name();
		qDebug() << "fileSystemType:" << storage.fileSystemType();
		qDebug() << "size:" << storage.bytesTotal() << "Bytes";
		qDebug() << "availableSize:" << storage.bytesAvailable() << "Bytes";
	}

	qDebug() << "All Done.";
}

void MainWindow::on_pushButton_selectAll_clicked()
{
	for (int i = 0; i < view_list.size(); i++){
		view_list.at(i)->check(true);
	}
}

void MainWindow::on_pushButton_update_clicked()
{
	this->refreshVolumes();
}

void MainWindow::on_pushButton_deselectAll_clicked()
{
	for (int i = 0; i < view_list.size(); i++){
		view_list.at(i)->check(false);
	}
}

void MainWindow::on_pushButton_ok_clicked()
{
	scan_prc->show();
	foreach (ShowDrives *obj, view_list) {
		if (obj->isSelected())
		{
			scan_prc->addTask(obj->object);
		}
	}
}

void MainWindow::on_pushButton_showTasks_clicked()
{
	scan_prc->show();
}
