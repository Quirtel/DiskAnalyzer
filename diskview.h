#ifndef DISKVIEW_H
#define DISKVIEW_H

#include <QMainWindow>

namespace Ui {
	class DiskView;
}

class DiskView : public QMainWindow
{
	Q_OBJECT

public:
	explicit DiskView(QWidget *parent = 0);
	~DiskView();

private:
	Ui::DiskView *ui;
};

#endif // DISKVIEW_H
