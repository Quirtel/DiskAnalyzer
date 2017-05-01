#include "showdrives.h"
#include "ui_showdrives.h"
#include <QDir>
#include <QDebug>
#include <QString>
#include <QStorageInfo>

ShowDrives::ShowDrives(QWidget *parent,QString num) :
    QWidget(parent),
    ui(new Ui::ShowDrives)
{
    ui->setupUi(this);
    ui->label->setText("Number" + num);
}

ShowDrives::~ShowDrives()
{
    delete ui;
}
