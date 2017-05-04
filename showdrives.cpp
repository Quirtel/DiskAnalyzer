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
    ui->label->setText(num);

    foreach (QStorageInfo storage, QStorageInfo::mountedVolumes())
    {   ui->label1->setText("name: " + storage.name());
        ui->label2->setText("fileSystemType:  " + storage.fileSystemType());
}
}
ShowDrives::~ShowDrives()
{
    delete ui;
}
