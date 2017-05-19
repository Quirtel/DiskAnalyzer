#ifndef PROCESSWINDOW_H
#define PROCESSWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QStorageInfo>

namespace Ui {
	class ProcessWindow;
}

class ProcessWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit ProcessWindow(QList<QStorageInfo> list_of_volumes, QWidget *parent = 0);
	~ProcessWindow();
	void closeEvent(QCloseEvent *event);

private slots:
	void on_pushButton_cancel_clicked();

private:
	Ui::ProcessWindow *ui;
	QList<QStorageInfo> vols_list;
};

#endif // PROCESSWINDOW_H
