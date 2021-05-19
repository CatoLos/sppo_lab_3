#include "FilePercentageStrategy.h"
#include <QFileInfo>
#include <QDirIterator>

FilePercentageStrategy::FilePercentageStrategy()
{}

FilePercentageStrategy::~FilePercentageStrategy()
{}

// метод получения размера сущности (файла, папки) в байтах.
uint64_t FilePercentageStrategy::getTotalSize(QString const& path, QDir::Filters filters) const
{
    QFileInfo fileInf(path);

    if (fileInf.isDir()) //если папка
    {
        //если папка пустая - возвращать 0.
        if (fileInf.dir().isEmpty())
            return 0;


        QDirIterator dirIt(fileInf.absoluteFilePath(), filters, QDirIterator::Subdirectories);
        uint64_t total = 0;
        
        while (dirIt.hasNext())
        {
            dirIt.next();
            total += dirIt.fileInfo().size();
        }

        return total;
    }
    else
        return fileInf.size(); //иначе вернуть размер файла
}
