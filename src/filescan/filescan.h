#ifndef FILESCAN_H
#define FILESCAN_H
#include <QString>
#include <QObject>
#include <QVector>
#include <QMutex>
#include "dirinfo.h"
#include <QMap>
#include <QFileInfo>

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
	bool signal_stop; // переменная-маркер для прерывания

	~Filescan();

public slots:
	void startWork();
	void sendStop();

signals:
    void bytesRead(quint64);
	void currentFileScan(QString);

private:
	QMutex mutex;
	QVector <QFileInfo> all_files;
};

#endif // FILESCAN_H
