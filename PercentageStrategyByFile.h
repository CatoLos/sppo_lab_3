#pragma once
#include "FilePercentageStrategy.h"

class PercentageStrategyByFile : public FilePercentageStrategy
{
public:

    explicit PercentageStrategyByFile();
    ~PercentageStrategyByFile();

    QVector<QPair<QString, uint64_t>> calculate(QString const& path) override;
};

