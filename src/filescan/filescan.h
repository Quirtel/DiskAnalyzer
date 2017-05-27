#ifndef FILESCAN_H
#define FILESCAN_H
#include <QString>
#include <QObject>
#include <QVector>
#include "dirinfo.h"
#include <QMap>

class Filescan : public QObject
{
	Q_OBJECT
public:
	Filescan(const QString &entryDir);
	unsigned long long size;
	quint64 getFilesOfDir_recursion(const QString &path);
	QString dir_address;
	DirInfo *root;
	void startAnalyse();
	QMap <QString, DirInfo *> dirs_map;

	~Filescan();

public slots:
	void startWork();

signals:
	void sendStop();

private:
	QMutex mutex;
	QVector <QFileInfo> all_files;
};

#endif // FILESCAN_H
