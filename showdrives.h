#ifndef SHOWDRIVES_H
#define SHOWDRIVES_H

#include <QWidget>
#include <QString>

namespace Ui {
class ShowDrives;
}

class ShowDrives : public QWidget
{
    Q_OBJECT

public:
    explicit ShowDrives(QWidget *parent = 0,QString num = "0");
    ~ShowDrives();

private:
    Ui::ShowDrives *ui;
};

#endif // SHOWDRIVES_H
