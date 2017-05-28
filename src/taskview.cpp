#include "taskview.h"
#include "ui_taskview.h"
#include "filescan/filescan.h"
#include "filescan/dirinfo.h"
#include <QStorageInfo>
#include <QThread>
#include "showdrives.h"

taskView::taskView(QStorageInfo storage, QWidget *parent) : QWidget(parent), storage_info(storage),
    ui(new Ui::taskView)
{
	ui->setupUi(this);
	bytesRead = 0;

	if (storage_info.name() == "")
	{
		ui->label_driveName->setText(storage_info.device()); // если у устройства нет имени, то прописываем точку монтирования
	}
	else
	{
		ui->label_driveName->setText(storage_info.name()); // иначе прописываем имя
	}

	ui->progressBar->setMaximum(convertFromBytes(storage_info.bytesTotal() - storage_info.bytesFree()));

	scan_adapter = new Filescan(storage.rootPath());
	connect(scan_adapter, &Filescan::currentFileScan, this, [=](QFileInfo a){
		if (!a.isDir())
		{
			bytesRead += a.size();
			ui->label_readBytes->setText("Прочитано: " +
			                             QString::number(convertFromBytes(bytesRead)) + '/' +
			                             QString::number(convertFromBytes(storage_info.bytesAvailable())));
		}
	});
	thr = new QThread(this);
	connect(thr, &QThread::started, scan_adapter, &Filescan::startWork);
	connect(thr, &QThread::finished, thr, &QThread::deleteLater);
	scan_adapter->moveToThread(thr);
}

taskView::~taskView()
{
	scan_adapter->sendStop();
	thr->terminate();
	thr->wait();

	scan_adapter->deleteLater();
	delete ui;
}

void taskView::mouseDoubleClickEvent(QMouseEvent *) {

}

QString taskView::getUnit(quint64 &value)
{
	QStringList list;
	list << "KB" << "MB" << "GB";
	QString unit("bytes");

	QStringListIterator i1(list);

	while(value >= 1000.0 && i1.hasNext())
	{
		unit = i1.next();
		value /= 1000.0;
	}

	return unit;
}

void taskView::execute()
{
	thr->start();
}

double taskView::convertFromBytes(quint64 value)
{
	return static_cast<double>(value / 1000 / 1000);
}

void taskView::on_pushButton_cancel_clicked()
{
	scan_adapter->sendStop();
}

void taskView::stopProcess()
{
	delete this;
}
