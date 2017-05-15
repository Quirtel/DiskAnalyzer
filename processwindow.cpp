#include "processwindow.h"
#include "ui_processwindow.h"
#include <QStorageInfo>
#include <QList>
#include <QCloseEvent>

ProcessWindow::ProcessWindow(QList<QStorageInfo> list_of_volumes, QWidget *parent) :
    QMainWindow(parent),  ui(new Ui::ProcessWindow), vols_list(list_of_volumes)
{
	ui->setupUi(this);
}

ProcessWindow::~ProcessWindow()
{
	delete ui;
}

void ProcessWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
	this->deleteLater();
}

void ProcessWindow::on_pushButton_cancel_clicked()
{
	this->close();
}
