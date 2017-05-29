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
	signal_stop = false;
}

Filescan::~Filescan()
{
	foreach (DirInfo *info, dirs_map.values()) {
		delete info;
	}
	delete root;
}

void Filescan::startWork()
{
	startAnalyse();
}

void Filescan::sendStop()
{
	signal_stop = true; //ставим прерывания процесса
}

void Filescan::startAnalyse()
{
	root->setDirectorySize(getFilesOfDir_recursion(dir_address));
	std::sort(all_files.begin(), all_files.end(), [&](QFileInfo a1, QFileInfo a2)
	{
		return a1.size() > a2.size();
	});
	this->thread()->quit();
}

quint64 Filescan::getFilesOfDir_recursion(const QString &path)
{
	QFileInfo str_info(path);
	quint64 sizex = 0;
	if (signal_stop) return 0;

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
				all_files.append(fileInfo);
				sizex += fileInfo.size();
				emit currentFileScan(fileInfo.absoluteFilePath());
				emit bytesRead(fileInfo.size());
			}
		}
	}
	return sizex;
}

QVector <QFileInfo> Filescan::getListOfFiles() const
{
	return this->all_files;
}











