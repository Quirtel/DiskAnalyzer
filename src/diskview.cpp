#include "diskview.h"
#include "ui_diskview.h"
#include "filescan/dirinfo.h"
#include "filescan/filescan.h"
#include <QTableWidgetItem>
#include <QList>
#include <QFileInfo>
#include <QVector>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QScrollBar>
#include <QDebug>
#include "taskview.h"

DiskView::DiskView(Filescan *scan_res, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiskView), scanAdapter(scan_res)
{
	ui->setupUi(this);

	ui->tableWidget_files->setColumnCount(2);
	ui->tableWidget_files->setHorizontalHeaderItem(0, new QTableWidgetItem("Название")); // добавляем столбец
	ui->tableWidget_files->setHorizontalHeaderItem(1, new QTableWidgetItem("Размер")); // и здесь тоже

	ui->tableWidget_files->setColumnWidth(0,300); // устанавливаем ширину столбцов в таблице
	ui->tableWidget_files->setColumnWidth(1,120);

	all_files = scanAdapter->getListOfFiles();

	for (int i = 0; i < 30; i++)
	{
		if (i < all_files.size()) //сравниваем для того чтобы избежать выхода за границы вектора
		{
			if (all_files.at(i).fileName() != "")
			{
				ui->tableWidget_files->setRowCount(ui->tableWidget_files->rowCount() + 1);

				QTableWidgetItem *item_filename = new QTableWidgetItem();
				item_filename->setText(all_files.at(i).fileName());
				item_filename->setWhatsThis(all_files.at(i).absoluteFilePath());

				QTableWidgetItem *item_size = new QTableWidgetItem();
				item_size->setText(QString::number(taskView::convertFromBytes(all_files.at(i).size()),'f',2)
				                   + ' ' + taskView::getUnit(all_files.at(i).size()));

				ui->tableWidget_files->setItem(i, 0, item_filename);
				ui->tableWidget_files->setItem(i, 1, item_size);
			}
		}
	}

	connect(ui->tableWidget_files->verticalScrollBar(),SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFiles(int)));

}

void DiskView::closeEvent(QCloseEvent *event)
{
	event->accept();
	this->deleteLater();
}

DiskView::~DiskView()
{
	delete ui;
}

void DiskView::on_tableWidget_files_cellDoubleClicked(int row, int column)
{
	if(column == 0)
	{
		QFileInfo file(ui->tableWidget_files->item(row,column)->whatsThis());
		QDesktopServices::openUrl(QUrl("file://" + file.absolutePath(), QUrl::TolerantMode));
	}
}

void DiskView::addItemToListOfFiles(int index) //функция автоматически подгружает элементы в таблицу, для экономии памяти
{
	int minimum_item_index = ui->tableWidget_files->rowAt(0);
	qDebug() << "Minimum" << minimum_item_index;

	int maximum_item_index = ui->tableWidget_files->rowAt(ui->tableWidget_files->height() - 25);

	qDebug() << "Maximum" << maximum_item_index;

	qDebug() << index;

	int index_delta = maximum_item_index - minimum_item_index;
	qDebug() << "Index delta = " << index_delta;

	disconnect(ui->tableWidget_files->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFiles(int)));

	if (maximum_item_index == ui->tableWidget_files->rowCount() - 1 && maximum_item_index < all_files.size() - 1)
	{
		ui->tableWidget_files->setRowCount(ui->tableWidget_files->rowCount() + index_delta);
		for (int i = 0; i < index_delta; i++)
		{
			if (maximum_item_index + i + 1 < all_files.size())
			{
				QTableWidgetItem *item_filename = new QTableWidgetItem();
				item_filename->setText(all_files.at(maximum_item_index + i + 1).fileName());
				item_filename->setWhatsThis(all_files.at(maximum_item_index + i + 1).absoluteFilePath());

				QTableWidgetItem *item_size = new QTableWidgetItem();
				item_size->setText(QString::number(taskView::convertFromBytes(all_files.at(maximum_item_index + i + 1).size()),'f',2)
				                   + ' ' + taskView::getUnit(all_files.at(maximum_item_index + i + 1).size()));

				ui->tableWidget_files->setItem(maximum_item_index + i + 1, 0, item_filename);
				ui->tableWidget_files->setItem(maximum_item_index + i + 1, 1, item_size);
			}
		}
	}

	connect(ui->tableWidget_files->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFiles(int)));


	qDebug() << "\n";
}
