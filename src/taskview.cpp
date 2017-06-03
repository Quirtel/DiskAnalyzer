#include "taskview.h"
#include "../ui/ui_taskview.h"
#include "filescan/filescan.h"
#include "filescan/dirinfo.h"
#include <QStorageInfo>
#include <QThread>
#include "showdrives.h"
#include<QMessageBox>

taskView::taskView(QStorageInfo storage, QWidget *parent) : QWidget(parent), storage_info(storage),
    ui(new Ui::taskView)
{
	ui->setupUi(this);
	bytesRead = 0;
	isReady = false;
	errored = false;
	view = nullptr;

	if (storage_info.name() == "")
	{
		ui->label_driveName->setText(storage_info.device()); // если у устройства нет имени, то прописываем точку монтирования
	}
	else
	{
		ui->label_driveName->setText(storage_info.name()); // иначе прописываем имя
	}

	ui->progressBar->setMaximum(convertFromBytes(storage_info.bytesTotal() - storage_info.bytesFree()));

	scan_adapter = new Filescan(storage);
	connect(scan_adapter, SIGNAL(currentFileScan(QString)), this, SLOT(updateInfo(QString)));

	connect(scan_adapter, &Filescan::bytesRead, this, [&](quint64 b)
	{
		bytesRead += b;
		ui->progressBar->setValue(static_cast<int>(convertFromBytes(bytesRead)));
	});

	thr = new QThread(this);
	connect(scan_adapter, &Filescan::errorOccured, this, [=]()
	{
		errored = true;
		workFailed();
	});

	connect(thr, &QThread::started, scan_adapter, &Filescan::startWork);
	connect(thr, &QThread::finished, this, &taskView::workFinished);


	scan_adapter->moveToThread(thr);
}

taskView::~taskView()
{
	scan_adapter->sendStop();
	if (thr->isRunning())
	{
		thr->terminate();
		thr->wait();
	}

	thr->deleteLater();
	scan_adapter->deleteLater();
	delete ui;
}

void taskView::updateInfo(QString info)
{
	ui->label_readBytes->setText(info);
}

void taskView::mouseDoubleClickEvent(QMouseEvent *) {
	if(isReady)
	{
		if (view == nullptr)
		{
			view = new DiskView(storage_info,scan_adapter,0);
			connect(view, &DiskView::destroyed, this, [=]()
			{
				view = nullptr;
			});
			view->show();
		}
	}
}

QString taskView::getUnit(quint64 value)
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

void taskView::workFinished()
{
	if (!errored)
	{
		isReady = true; //завершен ли процесс
		ui->progressBar->setValue(ui->progressBar->maximum());
		ui->label_readBytes->setText("Сканирование завершено");
		ui->pushButton_cancel->setVisible(false);
	}
}

void taskView::workFailed()
{
	isReady = false;
	if (errored)
	{
		ui->progressBar->setValue(0);
		ui->label_readBytes->setText("Ошибка сканирования");
		ui->pushButton_cancel->setText("Закрыть");
		ui->pushButton_cancel->setVisible(true);
	}
}

double taskView::convertFromBytes(quint64 value)
{
	double buf = value;
	while(buf >= 1000.0)
	{
		buf /= 1000.0;
	}
	return buf;
}

void taskView::on_pushButton_cancel_clicked()
{
	if (errored == false)
	{
		QMessageBox msgBox;
		msgBox.setText("Вы действительно хотите отменить сканирование?");
		msgBox.setIcon(QMessageBox::Question);
		QPushButton *Yes = msgBox.addButton(tr("Да"), QMessageBox::ActionRole);
		QPushButton *No = msgBox.addButton(tr("Нет"), QMessageBox::ActionRole);
		msgBox.exec();
		if(msgBox.clickedButton() == Yes)
		{
			delete this;
		}
		else if (msgBox.clickedButton() == No)
		{
			return;
		}
	}
	else
	{
		delete this;
	}
}

void taskView::stopProcess()
{
	delete this;
}

QString taskView::device_name() const
{
	if (storage_info.name() == "")
		return storage_info.device();
	else return storage_info.name();
}
