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
#include <QStorageInfo>
#include <QScrollBar>
#include <QDebug>
#include "taskview.h"

DiskView::DiskView(QStorageInfo info,Filescan *scan_res, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiskView), scanAdapter(scan_res), disk_info(info)
{
	ui->setupUi(this);
	filesListInit();
	foldersListInit(disk_info.rootPath());


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

void DiskView::filesListInit() // добавление списка файлов в таблицу
{
	ui->tableWidget_files->setColumnCount(2);
	ui->tableWidget_files->setHorizontalHeaderItem(0, new QTableWidgetItem("Название")); // добавляем столбец
	ui->tableWidget_files->setHorizontalHeaderItem(1, new QTableWidgetItem("Размер")); // и здесь тоже

	ui->tableWidget_files->setColumnWidth(0,400); // устанавливаем ширину столбцов в таблице
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

void DiskView::foldersListInit(const QString &entry_directory)
{
	disconnect(ui->tableWidget_dirs->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFolders(int)));
	current_directory = entry_directory;

	ui->label_folderPath->setText("Directory: " + current_directory);

	if(entry_directory == disk_info.rootPath()) //если путь совпадает с корнем диска
	{
		ui->pushButton_levelUp->setEnabled(false); // отключить кнопку
	}
	else
	{
		ui->pushButton_levelUp->setEnabled(true);
	}

	ui->tableWidget_dirs->clear();
	ui->tableWidget_dirs->setRowCount(0);

	QDir dir(entry_directory);
	dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden);

	list_of_dirs = dir.entryInfoList();

	ui->tableWidget_dirs->setColumnCount(2);
	ui->tableWidget_dirs->setHorizontalHeaderItem(0, new QTableWidgetItem("Название")); // добавляем столбец
	ui->tableWidget_dirs->setHorizontalHeaderItem(1, new QTableWidgetItem("Размер")); // и здесь тоже

	ui->tableWidget_dirs->setColumnWidth(0,400); // устанавливаем ширину столбцов в таблице
	ui->tableWidget_dirs->setColumnWidth(1,120);

	for (int i = 0; i < 30; i++)
	{
		if (i < list_of_dirs.size()) //сравниваем для того чтобы избежать выхода за границы вектора
		{
			if (list_of_dirs.at(i).fileName() != "")
			{
				ui->tableWidget_dirs->setRowCount(ui->tableWidget_dirs->rowCount() + 1);

				QTableWidgetItem *item_dirname = new QTableWidgetItem();
				item_dirname->setText(list_of_dirs.at(i).absoluteFilePath());
				item_dirname->setWhatsThis(list_of_dirs.at(i).absoluteFilePath());

				QTableWidgetItem *item_size = new QTableWidgetItem();
				item_size->setData(Qt::DisplayRole,QString::number(
				                       taskView::convertFromBytes(scanAdapter->dirs_map.value(list_of_dirs.at(i).absoluteFilePath())->getDirectorySize()),'f',2)
				                       + ' ' + taskView::getUnit(scanAdapter->dirs_map.value(list_of_dirs.at(i).absoluteFilePath())->getDirectorySize()));


				ui->tableWidget_dirs->setItem(i, 0, item_dirname);
				ui->tableWidget_dirs->setItem(i, 1, item_size);
			}
		}
		else break;
	}

	connect(ui->tableWidget_dirs->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFolders(int)));
}

void DiskView::addItemToListOfFiles(int index) //функция автоматически подгружает элементы в таблицу, для экономии памяти
{
	int minimum_item_index = ui->tableWidget_files->rowAt(0); // находим индекс первого видимого элемента
	qDebug() << "Minimum" << minimum_item_index;

	int maximum_item_index = ui->tableWidget_files->rowAt(ui->tableWidget_files->height() - 25); // находим индекс последнего видимого элемента

	qDebug() << "Maximum" << maximum_item_index;

	qDebug() << index;

	int index_delta = maximum_item_index - minimum_item_index; //находим их разность
	qDebug() << "Index delta = " << index_delta;

	disconnect(ui->tableWidget_files->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFiles(int)));

	if (maximum_item_index == ui->tableWidget_files->rowCount() - 1 && maximum_item_index < all_files.size() - 1)
	{
		ui->tableWidget_files->setRowCount(ui->tableWidget_files->rowCount() + index_delta);
		for (int i = 0; i < index_delta; i++)
		{
			if (maximum_item_index + i + 1 < all_files.size() && all_files.at(maximum_item_index + i + 1).absoluteFilePath() != "")
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

void DiskView::addItemToListOfFolders(int index) // а эта функция подгружает записи о директориях
{
	int minimum_item_index = ui->tableWidget_dirs->rowAt(0); // находим индекс первого видимого элемента
	qDebug() << "Minimum" << minimum_item_index;

	int maximum_item_index = ui->tableWidget_dirs->rowAt(ui->tableWidget_dirs->height() - 25); // находим индекс последнего видимого элемента
	if (maximum_item_index == -1) return;

	qDebug() << "Maximum" << maximum_item_index;

	qDebug() << index;

	int index_delta = maximum_item_index - minimum_item_index; //находим их разность
	qDebug() << "Index delta = " << index_delta;

	disconnect(ui->tableWidget_dirs->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFolders(int)));

	if (maximum_item_index == ui->tableWidget_dirs->rowCount() - 1 && maximum_item_index < list_of_dirs.size() - 1)
	{
		ui->tableWidget_dirs->setRowCount(ui->tableWidget_dirs->rowCount() + index_delta);
		for (int i = 0; i < index_delta; i++)
		{
			if (maximum_item_index + i + 1 < list_of_dirs.size() && list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath() != "")
			{
				QTableWidgetItem *item_dirname = new QTableWidgetItem();
				item_dirname->setText(list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath());
				item_dirname->setWhatsThis(list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath());

				QTableWidgetItem *item_size = new QTableWidgetItem();
				item_size->setData(Qt::DisplayRole,QString::number(
				                       taskView::convertFromBytes(scanAdapter->dirs_map.value(list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath())->getDirectorySize()),'f',2)
				                       + ' ' + taskView::getUnit(scanAdapter->dirs_map.value(list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath())->getDirectorySize()));

				ui->tableWidget_dirs->setItem(maximum_item_index + i + 1, 0, item_dirname);
				ui->tableWidget_dirs->setItem(maximum_item_index + i + 1, 1, item_size);
			}
		}
	}

	connect(ui->tableWidget_dirs->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(addItemToListOfFolders(int)));

	qDebug() << "\n";
}

void DiskView::on_tableWidget_dirs_cellDoubleClicked(int row, int column)
{
	foldersListInit(ui->tableWidget_dirs->item(row, 0)->text());
}

void DiskView::on_pushButton_levelUp_clicked()
{
	QDir dir(current_directory);
	dir.cdUp();

	foldersListInit(dir.absolutePath());
}
