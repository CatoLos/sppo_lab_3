#include "PercentageStrategyByFile.h"
#include <QFileInfo>
#include <QDirIterator>

PercentageStrategyByFile::PercentageStrategyByFile()
{}

PercentageStrategyByFile::~PercentageStrategyByFile()
{}

QVector<QPair<QString, uint64_t>> PercentageStrategyByFile::calculate(QString const& path)
{
    //настраиваем фильтр на скрытые файлы и папки
    QDir::Filters filters = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden;
    //данные файлов (название файла и размер этого файла)
    QVector<QPair<QString, uint64_t>> result;
    uint64_t totalSize = getTotalSize(path, filters);
    QFileInfo fileInf(path);


    if (totalSize)
    {
        if (fileInf.isDir())
        {
            QDirIterator it(fileInf.absoluteFilePath(), filters);
            while (it.hasNext())
            {
                it.next();
                auto file = it.fileInfo();
                result.append(qMakePair(file.fileName(),
                file.isDir() ? getTotalSize(file.absoluteFilePath(), filters) : file.size()));
            }
        }
        else //если изначально не папка
        {
            result.append(qMakePair(fileInf.fileName(), (uint64_t)fileInf.size()));
        }
    }

    //добавляем в конец общий размер все папки
    result.append(qMakePair(QString("Total size"), totalSize));
    return result;
}
