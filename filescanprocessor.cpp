#include "filescanprocessor.h"
#include "ui_filescanprocessor.h"

FileScanProcessor::FileScanProcessor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileScanProcessor)
{
	ui->setupUi(this);
}

FileScanProcessor::~FileScanProcessor()
{
	delete ui;
}
