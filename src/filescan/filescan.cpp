#include "filescan.h"
#include <QString>
#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>
#include <QDateTime>
#include <QMimeType>
#include <QMimeDatabase>
#include <QThread>

Filescan::Filescan(const QString &entryDir) : dir_address(entryDir)
{
	root = new DirInfo(dir_address);
	dirs_map.insert(dir_address, root);
}

Filescan::~Filescan()
{
	foreach (DirInfo *info, dirs_map.values()) {
		delete info;
	}
	delete this;
}

void Filescan::startWork()
{
	mutex.lock();
	startAnalyse();
	mutex.unlock();

	this->thread()->quit();
}

void Filescan::startAnalyse()
{
	root->setDirectorySize(getFilesOfDir_recursion(dir_address));
}

quint64 Filescan::getFilesOfDir_recursion(const QString &path)
{
	QFileInfo str_info(path);
	quint64 sizex = 0;
	if (str_info.isDir())
	{
		QDir dir(path);
		dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Hidden);
		QFileInfoList list = dir.entryInfoList();

		for(int i = 0; i < list.size(); i++)
		{
			QFileInfo fileInfo = list.at(i);
			if (fileInfo.isDir())
			{
				qDebug() << fileInfo.absoluteFilePath();

				DirInfo *info = new DirInfo(fileInfo.absoluteFilePath());
				info->setDirectorySize(getFilesOfDir_recursion(fileInfo.absoluteFilePath()));
				dirs_map.insert(fileInfo.absoluteFilePath(), info);

				sizex += info->getDirectorySize();
			}
			else
			{
				sizex += fileInfo.size();
			}
		}
	}
	return sizex;
}













