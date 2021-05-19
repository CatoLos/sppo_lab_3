#include "StrategyContext.h"

StrategyContext::StrategyContext(FilePercentageStrategy* Strategy) :
    m_strategy(Strategy)
{}

void StrategyContext::setStrategy(FilePercentageStrategy* Strategy)
{
    if(m_strategy)
        delete m_strategy;

    m_strategy = Strategy;
}

void StrategyContext::calculate(QString const& path)
{
    if (m_strategy)
        m_data = m_strategy->calculate(path);
}

void StrategyContext::sort()
{
    //сортировать элементы по убыванию их размера
    if (m_data.size() > 1)
        std::sort(m_data.begin(), m_data.end() - 1, 
            [](QPair<QString, uint64_t>& l, QPair<QString, uint64_t>& r) 
            { return l.second > r.second; });
}

QVector<QPair<QString, uint64_t>> StrategyContext::getData()
{
    return m_data;
}

void StrategyContext::clearData()
{
    m_data.clear();
}

StrategyContext::~StrategyContext()
{
    if(m_strategy)
        delete m_strategy;
}
