#pragma once
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QDir>

/*
    FilePercentageStrategy - интерфейс всех стратегий.
    
    getTotalSize - метод получения размера сущности (файла, папки) в байтах.
    параметры: path - путь до сущности, filters - фильтры сущностей, которые учитывается при подсчете размера.

    calculate - чистый виртуальный метод. Через этот метод происходит выполнение алгоритма соответсвующей стратегии.
    результат: вектор файлов и их размеров
*/


class FilePercentageStrategy
{
public:

    explicit FilePercentageStrategy();
    virtual ~FilePercentageStrategy();

    uint64_t getTotalSize(QString const& path, QDir::Filters filters) const;

    virtual QVector<QPair<QString, uint64_t>> calculate(QString const& path) = 0;
};

