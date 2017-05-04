#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QStorageInfo>
#include <QDebug>
#include "showdrives.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


foreach (QStorageInfo storage, QStorageInfo::mountedVolumes())
{
        ShowDrives *wnd = new ShowDrives(this, storage.device());
        QListWidgetItem *item = new QListWidgetItem();
        ui->listWidget->addItem(item);
        item->setSizeHint(QSize(50,100));
        ui->listWidget->setItemWidget(item, wnd);
    }
foreach (QStorageInfo storage, QStorageInfo::mountedVolumes())
    {
        qDebug() << storage.rootPath();
        if (storage.isReadOnly())
            qDebug() << "isReadOnly:" << storage.isReadOnly();

        qDebug() << "name:" << storage.name();
        qDebug() << "fileSystemType:" << storage.fileSystemType();
        qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
        qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
    }

    qDebug() << "All Done.";
}

MainWindow::~MainWindow()
{
	delete ui;
}
