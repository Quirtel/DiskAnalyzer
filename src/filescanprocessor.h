#ifndef FILESCANPROCESSOR_H
#define FILESCANPROCESSOR_H

#include <QMainWindow>

namespace Ui {
	class FileScanProcessor;
}

class FileScanProcessor : public QMainWindow
{
	Q_OBJECT

public:
	explicit FileScanProcessor(QWidget *parent = 0);
	~FileScanProcessor();

private:
	Ui::FileScanProcessor *ui;
};

#endif // FILESCANPROCESSOR_H
