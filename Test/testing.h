#ifndef TESTING_H
#define TESTING_H

#include <QObject>
#include <QWidget>
#include <QTest>
#include "../src/mainwindow.h"

class Testing : public QObject
{
	Q_OBJECT
private:
	MainWindow *wnd;

private slots:
	void create_obj();
	void checkVolumesAmount();
	void readWriteCheck();
	void delete_obj();
};


#endif // TESTING_H
