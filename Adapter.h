#pragma once
#include "Observer.h"

//адаптер для таблицы
class TableAdapter : public FilesObserver
{
public:

   TableAdapter(FileExplorerTableModel* model);
   virtual ~TableAdapter();
   //обновить данные
   void update(const QVector<QPair<QString, uint64_t>>& data, int sortIndex) override;
   //получить текущие данные
   const QVector<QPair<QString, uint64_t>>& data() override;
   //сортировать данные
   void sort(int sortIndex) override;

private:

   FileExplorerTableModel* m_model;
};


//адапттер для графиков
class ChartAdapter : public FilesObserver
{
public:

   ChartAdapter(QChart* chart, QChartView* view);
   virtual ~ChartAdapter();
   void update(const QVector<QPair<QString, uint64_t>>& data, int sortIndex) override;
   const QVector<QPair<QString, uint64_t>>& data() override;
   void sort(int sortIndex) override;

protected:

   //обновить график (главный шаблонный метод)
   void updateChart(const QVector<QPair<QString, uint64_t>>& data);
   virtual QAbstractSeries* makeSeries() = 0;
   virtual void appendToSeries(QAbstractSeries* series, const QString& label, float percent) = 0;

   QChart* m_chart;
   QChartView* m_view;
   QVector<QPair<QString, uint64_t>> m_data;
};

class StackedChartAdapter : public ChartAdapter
{
public:

   StackedChartAdapter(QChartView* view);

private:

   QAbstractSeries* makeSeries() override;
   void appendToSeries(QAbstractSeries* series, const QString& label, float percent) override;
};

class BarChartAdapter : public ChartAdapter
{
public:

   BarChartAdapter(QChartView* view);

private:

   QAbstractSeries* makeSeries() override;
   void appendToSeries(QAbstractSeries* series, const QString& label, float percent) override;
};

class PieChartAdapter : public ChartAdapter
{
public:

   PieChartAdapter(QChartView* view);

private:

   QAbstractSeries* makeSeries() override;
   void appendToSeries(QAbstractSeries* series, const QString& label, float percent) override;
};
