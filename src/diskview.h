#ifndef DISKVIEW_H
#define DISKVIEW_H

#include <QMainWindow>
#include "filescan/dirinfo.h"
#include "filescan/filescan.h"
#include <QVector>
#include <QCloseEvent>
#include <QStorageInfo>
#include <QPieSeries>
#include <QVBoxLayout>
#include <QChartView>

namespace Ui {
	class DiskView;
}

class DiskView : public QMainWindow
{
	Q_OBJECT

public:
	explicit DiskView(QStorageInfo info, Filescan *scan_res, QWidget *parent = 0);
	~DiskView();

private slots:
	void on_tableWidget_files_cellDoubleClicked(int row, int column);

	void addItemToListOfFiles(int index);

	void addItemToListOfFolders(int index);

	void on_tableWidget_dirs_cellDoubleClicked(int row, int column);

	void on_pushButton_levelUp_clicked();

    void on_tableWidget_files_cellClicked(int row, int column);

	void on_tableWidget_dirs_cellClicked(int row, int column);

private:
	Ui::DiskView *ui;
	Filescan *scanAdapter;
	QVector <QFileInfo> all_files;
	void closeEvent(QCloseEvent *event);
	void filesListInit();
	void foldersListInit(const QString &entry_directory);
	QStorageInfo disk_info;
	QString current_directory;
	QList<QFileInfo> list_of_dirs;
    QFileInfo file;
	QtCharts::QPieSeries *series_files;
	QtCharts::QChartView *chartView_files;
	QVBoxLayout *vbox_files;
	QtCharts::QPieSlice *slice_files;
	QtCharts::QPieSeries *series_folders;
	QtCharts::QChartView *chartView_folders;
	QVBoxLayout *vbox_folders;
	QtCharts::QPieSlice *slice_folders;

};

#endif // DISKVIEW_H
