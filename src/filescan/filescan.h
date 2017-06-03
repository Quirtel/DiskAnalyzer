#ifndef FILESCAN_H
#define FILESCAN_H
#include <QString>
#include <QObject>
#include <QVector>
#include <QMutex>
#include "dirinfo.h"
#include <QMap>
#include <QFileInfo>
#include <QStorageInfo>

class Filescan : public QObject
{
	Q_OBJECT
public:
	Filescan(QStorageInfo DI);
	unsigned long long size;
	quint64 getFilesOfDir_recursion(const QString &path);
	QString dir_address;
	DirInfo *root; //корневая папка
	void startAnalyse();
	QMap <QString, DirInfo *> getDirsMap() const;
	bool signal_stop; // переменная-маркер для прерывания
	bool errored;
	QVector <QFileInfo> getListOfFiles() const;
	DirInfo * getDirByPath(const QString &path);

	~Filescan();

public slots:
	void startWork();
	void sendStop();

signals:
	void currentFileScan(QString);
	void bytesRead(quint64);
	void errorOccured();

private:
	QMutex mutex;
	QVector <QFileInfo> all_files;
	QMap <QString, DirInfo *> dirs_map;
	QStorageInfo disk_info;
};

#endif // FILESCAN_H
