#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QWidget>
#include <QStorageInfo>
#include <QString>
#include <QThread>
#include "diskview.h"
#include "filescan/filescan.h"
#include "filescan/dirinfo.h"

namespace Ui {
class taskView;
}

class taskView : public QWidget
{
    Q_OBJECT
public:
	explicit taskView(QStorageInfo storage, QWidget *parent = 0);
	void execute();
    void mouseDoubleClickEvent(QMouseEvent *);
	QString device_name() const;
	static double convertFromBytes(quint64 value); // переводим в нужную единицу измерения(нужно для прогресс-бара)
	static QString getUnit(quint64 value); // получаем единицу измерения

	~taskView();

signals:
	void deleted();

public slots:
	void stopProcess();
	void updateInfo(QString);
	void workFinished();

private slots:
	void on_pushButton_cancel_clicked();

private:
	QStorageInfo storage_info;

	quint64 bytesRead; // сколько байт было всего прочитано
	QThread *thr;
	Filescan *scan_adapter; //объект, который будет производить сканирование
	bool isReady;
	DiskView *view;

	Ui::taskView *ui;
};

#endif // TASKVIEW_H
