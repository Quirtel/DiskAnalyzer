#include "showdrives.h"
#include "ui_showdrives.h"
#include <QDir>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QStringListIterator>
#include <QStorageInfo>

ShowDrives::ShowDrives(QStorageInfo info, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowDrives)
{
	ui->setupUi(this);
	if (info.name() == "")
	{
		ui->label->setText(info.device()); // если у устройства нет имени, то прописываем точку монтирования
	}
	else
	{
		ui->label->setText(info.name()); // иначе прописываем имя
		ui->label1->setText("Устройство: " + info.device()); // и отдельно указываем точку монтирования
	}

	if(info.isReadOnly())
		ui->label_readWriteAcc->setText("Только чтение");
	else
		ui->label_readWriteAcc->setText("Чтение и запись");

	ui->label2->setText("Файловая система:  " + info.fileSystemType());

	double size_free = static_cast<double>(info.bytesFree());
	double size_used = static_cast<double>(info.bytesTotal() - info.bytesFree());
	double size_total = static_cast<double>(info.bytesTotal());

	ui->progressBar->setMaximum(convertToMegabytes(size_total));
	ui->progressBar->setValue(convertToMegabytes(size_used));

	QString str_size_free = getUnitForMemory(size_free);
	QString str_size_used = getUnitForMemory(size_used);
	QString str_size_total = getUnitForMemory(size_total);

	ui->label_usedSpace->setText("Занято: " + QString::number(size_used,'f',2) + " " + str_size_used);

	ui->label_freespace->setText("Свободно: " + QString::number(size_free,'f',2) + " " + str_size_free);

	ui->label_sizeTotal->setText("Всего памяти: " + QString::number(size_total,'f',2) + " " + str_size_total);
}

double ShowDrives::convertToMegabytes(double size)
{
	return static_cast<double>(size / 1000 / 1000);
}

QString ShowDrives::getUnitForMemory(double &size)
{
	QStringList list;
	list << "KB" << "MB" << "GB" << "TB";
	QString unit("bytes");

	QStringListIterator i1(list);

	while(size  >= 1000.0 && i1.hasNext())
	{
		unit = i1.next();
		size /= 1000.0;
	}

	return unit;
}

ShowDrives::~ShowDrives()
{
	delete ui;
}

void ShowDrives::check(bool c)
{
	ui->checkBox->setChecked(c);
}

void ShowDrives::on_checkBox_clicked()
{
}
