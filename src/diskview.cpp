#include "diskview.h"
#include "ui_diskview.h"

DiskView::DiskView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiskView)
{
	ui->setupUi(this);
}

DiskView::~DiskView()
{
	delete ui;
}
