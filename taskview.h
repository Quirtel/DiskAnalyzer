#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QWidget>

class taskView : public QWidget
{
    Q_OBJECT
public:
    explicit taskView(QWidget *parent = 0);
    void mouseDoubleClickEvent(QMouseEvent *);



signals:

public slots:
};

#endif // TASKVIEW_H
