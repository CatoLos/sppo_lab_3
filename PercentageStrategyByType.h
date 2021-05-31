#pragma once
#include "FilePercentageStrategy.h"

class PercentageStrategyByType : public FilePercentageStrategy
{
public:

    explicit PercentageStrategyByType();
    ~PercentageStrategyByType();

    QVector<QPair<QString, uint64_t>> calculate(QString const& path) override;
};

