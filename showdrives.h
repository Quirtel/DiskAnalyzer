#ifndef SHOWDRIVES_H
#define SHOWDRIVES_H

#include <QWidget>
#include <QString>
#include <QStorageInfo>

namespace Ui {
class ShowDrives;
}

class ShowDrives : public QWidget
{
    Q_OBJECT

public:
	explicit ShowDrives(QStorageInfo info, QWidget *parent = 0);
	static QString getUnitForMemory(double &size);
	static double convertToMegabytes(double size);
	void check(bool c);

    ~ShowDrives();

public slots:
	void on_checkBox_clicked();

private:
    Ui::ShowDrives *ui;
};

#endif // SHOWDRIVES_H
