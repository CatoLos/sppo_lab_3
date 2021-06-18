#include "PercentageStrategyByType.h"
#include <QFileInfo>
#include <QDirIterator>


PercentageStrategyByType::PercentageStrategyByType()
{}

PercentageStrategyByType::~PercentageStrategyByType()
{}

QVector<QPair<QString, uint64_t>> PercentageStrategyByType::calculate(QString const& path)
{
    //настраиваем фильтр на скрытые файлы и папки
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden;
    uint64_t totalSize = getTotalSize(path, filters);
    //данные файлов (название файла и размер этого файла)
    QVector<QPair<QString, uint64_t>> result;
    QFileInfo fileInf(path);

    if (fileInf.isDir())
    {
        //Хэш-таблица типов файлов, значением выступаем общий размер файлов с таким типов в папке
        QHash<QString, uint64_t> typesTable;
        QDirIterator dirIt(fileInf.absoluteFilePath(), filters, QDirIterator::Subdirectories);

        while (dirIt.hasNext())
        {
            dirIt.next();
            auto file = dirIt.fileInfo();

            if (typesTable.contains(file.suffix()))
                typesTable[file.suffix()] += file.size();
            else
                typesTable[file.suffix()] = file.size();
        }

        QHashIterator<QString, uint64_t> tableIt(typesTable);
        result.resize(typesTable.size());

        for (size_t i = 0; tableIt.hasNext(); ++i)
        {
            tableIt.next();
            //если нет расширения у файла
            if (tableIt.key().isEmpty())
                result[i] = qMakePair(QString("without type"), tableIt.value());
            else
                result[i] = qMakePair('.' + tableIt.key(), tableIt.value());
        }
    }
    else
    {
        result.append(qMakePair(fileInf.suffix(), fileInf.size()));
    }

    //добавляем в конец общий размер всей папки
    result.append(qMakePair(QString("Total size"), totalSize));
    return result;
}
