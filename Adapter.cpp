#include "Adapter.h"

TableAdapter::TableAdapter(FileExplorerTableModel* model) :
   m_model(model)
{}

TableAdapter::~TableAdapter()
{
   delete m_model;
}

void TableAdapter::update(const QVector<QPair<QString, uint64_t>>& data, int sortIndex)
{
   m_model->setFilesData(data);
   m_model->sort(sortIndex);
}

const QVector<QPair<QString, uint64_t>>& TableAdapter::data()
{
   return m_model->getData();
}

void TableAdapter::sort(int sortIndex)
{
   m_model->sort(sortIndex);
}

ChartAdapter::ChartAdapter(QChart* chart, QChartView* view) :
   m_chart(chart),
   m_view(view)
{}

ChartAdapter::~ChartAdapter()
{
   delete m_chart;
}

void ChartAdapter::update(const QVector<QPair<QString, uint64_t>>& data, int sortIndex)
{
   m_data = data;
   sort(sortIndex);
}

const QVector<QPair<QString, uint64_t>>& ChartAdapter::data()
{
   return m_data;
}

void ChartAdapter::sort(int sortIndex)
{
   if (!m_data.empty())
   {
      std::sort(m_data.begin(), m_data.end() - 1, [sortIndex](const QPair<QString, uint64_t>& l, const QPair<QString, uint64_t>& r)
         {
            if (sortIndex == 0)
               return l.first.localeAwareCompare(r.first) == -1;
            else
               return l.second > r.second;
         });

      updateChart(m_data);
   }
}

StackedChartAdapter::StackedChartAdapter(QChartView* view) :
   ChartAdapter(new QChart, view)
{}

void StackedChartAdapter::updateChart(const QVector<QPair<QString, uint64_t>>& data)
{
   m_chart->removeAllSeries();

   if (!data.empty())
   {
      const auto series = new QStackedBarSeries;
      //суммарный процент
      float total_percent = 0.0;
      //лэйбл для диаграмм
      QString label;
      //процент текущего файла
      float percent = 0;
      bool finish = false;
      float totalSize = data[data.size() - 1].second;

      if(totalSize)
      {
         // Проходим по всем данным
         for (size_t i = 0; !finish && i < data.size() - 1; ++i)
         {
            //если суммарный процент меньше 95
            if (total_percent <= 95.0)
            {
               percent = float(data[i].second) / totalSize * 100;
               total_percent += percent;
               // Выбираем имена и процентное соотношение
               label = data[i].first + " - " + QString::number(percent, 'g', 3) + "%";
            }
            else
            {
               percent = 100.0 - total_percent;
               label = "others - " + QString::number(percent, 'g', 3) + "%";
               finish = true;
            }
            // Создаём set
            QBarSet* set = new QBarSet(label);
            // Заполняем значениями (процентными соотношениями)
            set->append(percent);
            // Добавляем в серию
            series->append(set);
         }

         // Добавляем серию в QChart
         m_chart->addSeries(series);
         // Выставляем тему
         m_chart->setTheme(QChart::ChartTheme::ChartThemeQt);
         // Говорим отображать легенду справа
         m_chart->legend()->setAlignment(Qt::AlignRight);
      }

      m_view->setChart(m_chart);
   }
}

BarChartAdapter::BarChartAdapter(QChartView* view) :
   ChartAdapter(new QChart, view)
{}

void BarChartAdapter::updateChart(const QVector<QPair<QString, uint64_t>>& data)
{
   m_chart->removeAllSeries();

   if (!data.empty())
   {
      const auto series = new QBarSeries;
      //суммарный процент
      float total_percent = 0.0;
      //лэйбл для диаграмм
      QString label;
      //процент текущего файла
      float percent = 0;
      bool finish = false;
      float totalSize = data[data.size() - 1].second;

      if(totalSize > 0)
      {
         // Проходим по всем данным
         for (size_t i = 0; !finish && i < data.size() - 1; ++i)
         {
            //если суммарный процент меньше 95
            if (total_percent <= 95.0)
            {
               percent = float(data[i].second) / totalSize * 100;
               total_percent += percent;
               // Выбираем имена и процентное соотношение
               label = data[i].first + " - " + QString::number(percent, 'g', 3) + "%";
            }
            else
            {
               percent = 100.0 - total_percent;
               label = "others - " + QString::number(percent, 'g', 3) + "%";
               finish = true;
            }
            // Создаём set
            QBarSet* set = new QBarSet(label);
            // Заполняем значениями (процентными соотношениями)
            set->append(percent);
            // Добавляем в серию
            series->append(set);
         }

         // Добавляем серию в QChart
         m_chart->addSeries(series);
         // Выставляем тему
         m_chart->setTheme(QChart::ChartTheme::ChartThemeQt);
         // Говорим отображать легенду справа
         m_chart->legend()->setAlignment(Qt::AlignRight);
      }

      m_view->setChart(m_chart);
   }
}

PieChartAdapter::PieChartAdapter(QChartView* view) :
   ChartAdapter(new QChart, view)
{}

void PieChartAdapter::updateChart(const QVector<QPair<QString, uint64_t>>& data)
{
   m_chart->removeAllSeries();

   if (!data.empty())
   {
      const auto series = new QPieSeries;
      //суммарный процент
      float total_percent = 0.0;
      //лэйбл для диаграмм
      QString label;
      //процент текущего файла
      float percent = 0;
      bool finish = false;
      float totalSize = data[data.size() - 1].second;

      if(totalSize > 0)
      {
         // Проходим по всем данным
         for (size_t i = 0; !finish && i < data.size() - 1; ++i)
         {
            //если суммарный процент меньше 95
            if (total_percent <= 95.0)
            {
               percent = float(data[i].second) / data[data.size() - 1].second * 100;
               total_percent += percent;
               // Выбираем имена и процентное соотношение
               label = data[i].first + " - " + QString::number(percent, 'g', 3) + "%";
            }
            else
            {
               percent = 100.0 - total_percent;
               label = "others - " + QString::number(percent, 'g', 3) + "%";
               finish = true;
            }
            // Создаём set
            QPieSlice* slice = new QPieSlice(label, percent);
            slice->setValue(percent);
            // Добавляем в серию
            series->append(slice);
         }

         // Добавляем серию в QChart
         m_chart->addSeries(series);
         // Выставляем тему
         m_chart->setTheme(QChart::ChartTheme::ChartThemeQt);
         // Говорим отображать легенду справа
         m_chart->legend()->setAlignment(Qt::AlignRight);
      }
      m_view->setChart(m_chart);
   }
}
