#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QStorageInfo>
#include <QDebug>
#include "showdrives.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->listWidget->setStyleSheet("QListWidget:item { border: 1px solid black;}"); // установка рамки на каждый виджет в списке

	disks_amount = new QLabel(this);
	ui->statusBar->insertWidget(0, disks_amount, 200); // вставляем надпись в статус-бар

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
	for (int i = 0; i < view_list.size(); i++){
		delete view_list.at(i);
	}
	view_list.clear();
	ui->listWidget->clear();

	foreach (QStorageInfo storage, QStorageInfo::mountedVolumes()) // для каждого элемента информации о диске
	{
		ShowDrives *wnd = new ShowDrives(storage, this); // создаем виджет
		view_list.push_back(wnd); // заносим его в список, чтобы контролировать

		QListWidgetItem *item = new QListWidgetItem(); // создаем контейнер под запись
		ui->listWidget->addItem(item);
		item->setSizeHint(QSize(50,100));
		ui->listWidget->setItemWidget(item, wnd); // присваиваем виджет контейнеру
	}

	detectVolsAmount();

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
	emit checkAll();
}

void MainWindow::on_pushButton_update_clicked()
{
	this->refreshVolumes();
}
