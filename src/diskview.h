#ifndef DISKVIEW_H
#define DISKVIEW_H

#include <QMainWindow>
#include "filescan/dirinfo.h"
#include "filescan/filescan.h"
#include <QVector>
#include <QCloseEvent>

namespace Ui {
	class DiskView;
}

class DiskView : public QMainWindow
{
	Q_OBJECT

public:
	explicit DiskView(Filescan *scan_res, QWidget *parent = 0);
	~DiskView();

private slots:
	void on_tableWidget_files_cellDoubleClicked(int row, int column);

	void addItemToListOfFiles(int index);

private:
	Ui::DiskView *ui;
	Filescan *scanAdapter;
	QVector <QFileInfo> all_files;
	void closeEvent(QCloseEvent *event);
};

#endif // DISKVIEW_H
