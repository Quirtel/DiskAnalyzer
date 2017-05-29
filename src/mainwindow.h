#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStorageInfo>
#include "showdrives.h"
#include "filescanprocessor.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void refreshVolumes();
	void detectVolsAmount();

private slots:
	void on_pushButton_selectAll_clicked();

	void on_pushButton_update_clicked();

	void on_pushButton_deselectAll_clicked();

	void on_pushButton_ok_clicked();

signals:
	void checkAll();
	void uncheckAll();

private:
	Ui::MainWindow *ui;
	QLabel *disks_amount;
	QList <ShowDrives *> view_list;
	double overall_memory; // объем памяти со всех носителей
	FileScanProcessor *scan_prc; //окно для показа текущих задач сканирования
	QList<QStorageInfo> drives_list;
};

#endif // MAINWINDOW_H
