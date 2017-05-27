#ifndef DIRINFO_H
#define DIRINFO_H

#include <QObject>
#include <QDir>
#include <QString>

class DirInfo : public QObject
{
	Q_OBJECT
public:
	explicit DirInfo(QString path);
	QString getDirectoryPath();
	QDir directory;
	quint64 getDirectorySize();
	void setDirectorySize(quint64 size);

signals:

public slots:

private:
	quint64 dir_size;

};

#endif // DIRINFO_H
