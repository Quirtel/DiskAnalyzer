#include "filescanprocessor.h"
#include "ui_filescanprocessor.h"
#include "taskview.h"
#include <QList>
#include <QStorageInfo>

FileScanProcessor::FileScanProcessor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileScanProcessor)
{
	ui->setupUi(this);

	ui->listWidget->setStyleSheet("QListWidget:item { border: 1px solid black;}"); // установка рамки на каждый виджет в списке
}

FileScanProcessor::~FileScanProcessor()
{
	delete ui;
}

void FileScanProcessor::closeEvent(QCloseEvent *event)
{
	emit stopTasks();
	event->accept();
}

void FileScanProcessor::addTask(QStorageInfo info)
{
	if (!list_of_devices.contains(info))
	{
		taskView *tskview = new taskView(info, this);
		list_of_devices.push_back(info);

		QListWidgetItem *item = new QListWidgetItem();
		ui->listWidget->addItem(item);
		item->setSizeHint(QSize(90,100));
		ui->listWidget->setItemWidget(item, tskview);

		connect(this, &FileScanProcessor::stopTasks, tskview, &taskView::stopProcess); // по закрытии окна остановить все процессы
		connect(tskview, &taskView::destroyed, this, [=]()
		{
			delete item;
			list_of_devices.removeOne(info); //удаляем из списка информацию об устройстве
		}); //по удалении виджета удаляется строка в списке

		tskview->execute();//запускаем каждый поток
	}
}
