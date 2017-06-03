#include "diskview.h"
#include "../ui/ui_diskview.h"
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
#include <QMessageBox>
#include <QGroupBox>
#include <QtCharts>
#include "taskview.h"

DiskView::DiskView(QStorageInfo info,Filescan *scan_res, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiskView), scanAdapter(scan_res), disk_info(info)
{
	ui->setupUi(this);
	filesListInit();
	foldersListInit(disk_info.rootPath());

	series_files = nullptr;
	chartView_files = nullptr;
	vbox_files = nullptr;

	series_folders = nullptr;
	chartView_folders = nullptr;
	vbox_folders = nullptr;

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
				item_dirname->setText(list_of_dirs.at(i).fileName());
				item_dirname->setWhatsThis(list_of_dirs.at(i).absoluteFilePath());

				QTableWidgetItem *item_size = new QTableWidgetItem();
				item_size->setData(Qt::DisplayRole,QString::number(
				                       taskView::convertFromBytes(scanAdapter->getDirsMap().value(list_of_dirs.at(i).absoluteFilePath())->getDirectorySize()),'f',2)
				                   + ' ' + taskView::getUnit(scanAdapter->getDirsMap().value(list_of_dirs.at(i).absoluteFilePath())->getDirectorySize()));


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
				item_dirname->setText(list_of_dirs.at(maximum_item_index + i + 1).fileName());
				item_dirname->setWhatsThis(list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath());

				QTableWidgetItem *item_size = new QTableWidgetItem();
				item_size->setData(Qt::DisplayRole,QString::number(
				                       taskView::convertFromBytes(scanAdapter->getDirsMap().value(list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath())->getDirectorySize()),'f',2)
				                   + ' ' + taskView::getUnit(scanAdapter->getDirsMap().value(list_of_dirs.at(maximum_item_index + i + 1).absoluteFilePath())->getDirectorySize()));

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
	QDir dir(ui->tableWidget_dirs->item(row, 0)->whatsThis());
	dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden);
	QList<QFileInfo> list_next = dir.entryInfoList();
	if (!list_next.isEmpty())
	{
		foldersListInit(ui->tableWidget_dirs->item(row, 0)->whatsThis());
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText("Эта папка не содержит поддиректорий");
		msgBox.exec();
	}
}

void DiskView::on_pushButton_levelUp_clicked()
{
	QDir dir(current_directory);
	dir.cdUp();
	foldersListInit(dir.absolutePath());
}

void DiskView::on_tableWidget_files_cellClicked(int row, int column)
{
	QFileInfo file(ui->tableWidget_files->item(row,0)->whatsThis());
	QStorageInfo storage = disk_info;

	if (vbox_files != nullptr)
	{
		delete vbox_files;
		vbox_files = nullptr;
	}

	if (chartView_files != nullptr)
	{
		delete chartView_files;
		chartView_files = nullptr;
	}

	if (series_files != nullptr)
	{
		delete series_files;
		series_files = nullptr;
	}

	QT_CHARTS_USE_NAMESPACE
	series_files = new QPieSeries(this);
	series_files->setHoleSize(0.35);
	slice_files = series_files->append(file.fileName(), ((storage.bytesTotal()-storage.bytesFree()-(storage.bytesTotal()-storage.bytesFree()-file.size()))/1000000));
	slice_files->setExploded();
	slice_files->setLabelVisible();
	series_files->append("Использовано", ((storage.bytesTotal()-storage.bytesFree()-file.size())/1000000));
	series_files->append("Свободно",((storage.bytesTotal()-file.size()-(storage.bytesTotal()-storage.bytesFree()-file.size()))/1000000));

	chartView_files = new QChartView(this);
	chartView_files->setRenderHint(QPainter::Antialiasing);
	chartView_files->chart()->addSeries(series_files);
	chartView_files->chart()->legend()->setAlignment(Qt::AlignBottom);
	chartView_files->chart()->legend()->setFont(QFont("Arial",10));

	vbox_files = new QVBoxLayout();
	vbox_files->addWidget(chartView_files);
	ui->groupBox_inFile->setLayout(vbox_files);
}

void DiskView::on_tableWidget_dirs_cellClicked(int row, int column)
{
	DirInfo *dir = scanAdapter->getDirByPath(ui->tableWidget_dirs->item(row,0)->whatsThis());

	if (vbox_folders != nullptr)
	{
		delete vbox_folders;
		vbox_folders = nullptr;
	}

	if (chartView_folders != nullptr)
	{
		delete chartView_folders;
		chartView_folders = nullptr;
	}

	if (series_folders != nullptr)
	{
		delete series_folders;
		series_folders = nullptr;
	}

	QT_CHARTS_USE_NAMESPACE
	series_folders = new QPieSeries(this);
	series_folders->setHoleSize(0.35);
	slice_folders = series_folders->append(dir->getDirectoryPath(),
	                                       ((disk_info.bytesTotal()-disk_info.bytesFree()-
	                                         (disk_info.bytesTotal()-disk_info.bytesFree()-dir->getDirectorySize()))/1000000));
	slice_folders->setExploded();
	slice_folders->setLabelVisible();
	series_folders->append("Использовано", ((disk_info.bytesTotal()-disk_info.bytesFree()-dir->getDirectorySize())/1000000));
	series_folders->append("Свободно",((disk_info.bytesTotal()-dir->getDirectorySize()-
	                                    (disk_info.bytesTotal()-disk_info.bytesFree()-dir->getDirectorySize()))/1000000));

	chartView_folders = new QChartView(this);
	chartView_folders->setRenderHint(QPainter::Antialiasing);
	chartView_folders->chart()->addSeries(series_folders);
	chartView_folders->chart()->legend()->setAlignment(Qt::AlignBottom);
	chartView_folders->chart()->legend()->setFont(QFont("Arial",10));

	vbox_folders = new QVBoxLayout();
	vbox_folders->addWidget(chartView_folders);
	ui->groupBox_inDir->setLayout(vbox_folders);
}
