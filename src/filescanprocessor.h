#ifndef FILESCANPROCESSOR_H
#define FILESCANPROCESSOR_H

#include <QMainWindow>
#include <QList>
#include <QStorageInfo>
#include <QCloseEvent>
#include "taskview.h"

namespace Ui {
	class FileScanProcessor;
}

class FileScanProcessor : public QMainWindow
{
	Q_OBJECT

public:
	explicit FileScanProcessor(QList<QStorageInfo> list,QWidget *parent = 0);
	~FileScanProcessor();

private:
	Ui::FileScanProcessor *ui;
	QList<QStorageInfo> list_of_devices;
	void closeEvent(QCloseEvent *event);
	QList <taskView *> taskList;

signals:
	void stopTasks();
};

#endif // FILESCANPROCESSOR_H
