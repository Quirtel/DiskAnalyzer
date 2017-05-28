#include "filescanprocessor.h"
#include "ui_filescanprocessor.h"
#include "taskview.h"
#include <QList>
#include <QStorageInfo>

FileScanProcessor::FileScanProcessor(QList<QStorageInfo> list ,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileScanProcessor),
    list_of_devices(list)
{
	ui->setupUi(this);

	ui->listWidget->setStyleSheet("QListWidget:item { border: 1px solid black;}"); // установка рамки на каждый виджет в списке

	foreach (QStorageInfo info, list_of_devices) {
		taskView *tskview = new taskView(info, this);
		taskList.push_back(tskview);

		QListWidgetItem *item = new QListWidgetItem();
		ui->listWidget->addItem(item);
		item->setSizeHint(QSize(50,100));
		ui->listWidget->setItemWidget(item, tskview);

		tskview->execute();
	}
}

FileScanProcessor::~FileScanProcessor()
{
	delete ui;
}

void FileScanProcessor::closeEvent(QCloseEvent *event)
{
	foreach (taskView *ts, taskList) {
		ts->deleteLater();
	}
}
