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
	DirInfo *root; //корневая папка
	void startAnalyse();
	QMap <QString, DirInfo *> getDirsMap() const;
	bool signal_stop; // переменная-маркер для прерывания
	QVector <QFileInfo> getListOfFiles() const;

	~Filescan();

public slots:
	void startWork();
	void sendStop();

signals:
	void currentFileScan(QString);
	void bytesRead(quint64);

private:
	QMutex mutex;
	QVector <QFileInfo> all_files;
	QMap <QString, DirInfo *> dirs_map;
};

#endif // FILESCAN_H
