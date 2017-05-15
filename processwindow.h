#ifndef PROCESSWINDOW_H
#define PROCESSWINDOW_H

#include <QMainWindow>

namespace Ui {
	class ProcessWindow;
}

class ProcessWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit ProcessWindow(QWidget *parent = 0);
	~ProcessWindow();

private:
	Ui::ProcessWindow *ui;
};

#endif // PROCESSWINDOW_H
