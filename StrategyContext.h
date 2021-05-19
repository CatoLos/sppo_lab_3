#pragma once
#include "FilePercentageStrategy.h"

//Контекст, где хранится указатели на стратегии
class StrategyContext
{
public:

    StrategyContext(FilePercentageStrategy* Strategy = nullptr);
    
    void setStrategy(FilePercentageStrategy* Strategy);
    void calculate(QString const& path);
    void sort();

    QVector<QPair<QString, uint64_t>> getData();
    void clearData();

    ~StrategyContext();

private:

    FilePercentageStrategy*             m_strategy;
    QVector<QPair<QString, uint64_t>>   m_data;
};

