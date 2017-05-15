#include "processwindow.h"
#include "ui_processwindow.h"

ProcessWindow::ProcessWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProcessWindow)
{
	ui->setupUi(this);
}

ProcessWindow::~ProcessWindow()
{
	delete ui;
}
