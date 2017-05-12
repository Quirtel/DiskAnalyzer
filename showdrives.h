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
	QString getUnitForMemory(double &size);
	int convertToMegabytes(double size);

    ~ShowDrives();

public slots:
	void on_checkBox_clicked();

private:
    Ui::ShowDrives *ui;
};

#endif // SHOWDRIVES_H
