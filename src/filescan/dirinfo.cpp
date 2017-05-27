#include "dirinfo.h"
#include <QDir>

DirInfo::DirInfo(QString path)
{
	directory.setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::NoSymLinks);
	directory.setPath(path);
	dir_size = 0;
}

QString DirInfo::getDirectoryPath()
{
	return directory.canonicalPath();
}

void DirInfo::setDirectorySize(quint64 size)
{
	this->dir_size = size;
}

quint64 DirInfo::getDirectorySize()
{
	return this->dir_size;
}
