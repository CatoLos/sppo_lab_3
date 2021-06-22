#include <FileExplorer.h>
#include "PercentageStrategyByFile.h"
#include "PercentageStrategyByType.h"
#include "Adapter.h"
#include "ui_FileExplorer.h"


FileExplorer::FileExplorer(QWidget* parent, FileExplorer::StrategyType strat_type) :
    QWidget(parent),
    ui(new Ui::FileExplorer),
    m_fileSystem(new QFileSystemModel(this))
{
    //настраиваем UI
    ui->setupUi(this);
    //устанавливаем корневой путь в файловой модели
    m_fileSystem->setRootPath(QDir::homePath());
    //устанавливаем модель
    ui->treeView->setModel(m_fileSystem);
    //устанавливаем ратяжение колонок в qtreeview по размеру содержимого
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->chartView->setStyleSheet("border: 1px solid gray");
    //устанавливаем модель
    auto tableModel = new FileExplorerTableModel(this);
    ui->tableView->setModel(tableModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_observers.push_back(new TableAdapter(tableModel));
    m_observers.push_back(new BarChartAdapter(ui->chartView));
    m_observers.push_back(new PieChartAdapter(ui->chartView));
    m_observers.push_back(new StackedChartAdapter(ui->chartView));
    m_observer = m_observers.front();

    //устанавливаем соответсвующую стратегию
    if (strat_type == StrategyType::byFolder)
        m_strategy = new PercentageStrategyByFile;
    else
        m_strategy = new PercentageStrategyByType;

    //настраиваем сигнально-слотный механизм
    connect(ui->strategyComBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileExplorer::setPercentageStrategy);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileExplorer::folderChanged);
    connect(ui->displayComBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileExplorer::changeDisplayView);
    connect(ui->sortComBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileExplorer::changeSorting);
}

FileExplorer::~FileExplorer()
{
    delete ui;
    delete m_strategy;
    delete m_fileSystem;
    qDeleteAll(m_observers);
}

void FileExplorer::folderChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
    //обновляем текущий путь до новой выбранной папки
    m_currentPath = m_fileSystem->filePath(selected.indexes()[0]);
    //обновляем данные в модели
    auto data = m_strategy->calculate(m_currentPath);
    m_observer->update(data, ui->sortComBox->currentIndex());
}

void FileExplorer::changeDisplayView(int index)
{
   if (index >= 0 && index < m_observers.size())
   {
      auto const& data = m_observer->data();
      m_observer = m_observers[index];
      m_observer->update(data, ui->sortComBox->currentIndex());
      ui->stackedWidget->setCurrentIndex(index == 0 ? 0 : 1);
   }
}

void FileExplorer::changeSorting(int index)
{
   if (index >= 0 && index < 2)
   {
      m_observer->sort(index);
   }
}

void FileExplorer::setPercentageStrategy(qint32 const& index)
{
    delete m_strategy;
    //устанавливаем соответсвующую страте
    switch (index)
    {
    case 0:
        m_strategy = new PercentageStrategyByFile();
        break;
    case 1:
        m_strategy = new PercentageStrategyByType();
        break;
    }

    //если папка не выбрана, то выходим
    if (m_currentPath.isEmpty())
        return;

    //обновляем данные модели
    auto data = m_strategy->calculate(m_currentPath);
    m_observer->update(data, ui->sortComBox->currentIndex());
}
