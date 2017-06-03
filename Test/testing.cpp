#include "testing.h"
#include <QTest>
#include <QApplication>
#include <QStringList>
#include <QString>
#include <QFile>
#include <QStorageInfo>
#include <QByteArray>
#include "../src/mainwindow.h"
#include "../src/showdrives.h"
#include "../ui/ui_mainwindow.h"

QTEST_MAIN(Testing)

void Testing::create_obj()
{
	wnd = new MainWindow(0);
}

void Testing::checkVolumesAmount()
{
	wnd->refreshVolumes();
	int a = QStorageInfo::mountedVolumes().size();

	QCOMPARE(a, wnd->getRecordCount());
}

void Testing::readWriteCheck()
{
	QList<ShowDrives *> drives_list = wnd->getDiskRecords();
	bool all_correct;
	foreach (ShowDrives *test, drives_list) {
		if (!test->object.isReadOnly() && test->getRWlabel() == "Чтение и запись")
		{
			all_correct = true;
		}
		else
			all_correct = false;
	}
}

void Testing::delete_obj()
{
	delete wnd;
}
