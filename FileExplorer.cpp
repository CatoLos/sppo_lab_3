#include <FileExplorer.h>
#include "PercentageStrategyByFile.h"
#include "PercentageStrategyByType.h"
#include "ui_FileExplorer.h"


FileExplorer::FileExplorer(QWidget* parent, FileExplorer::StrategyType strat_type) :
    QWidget(parent),
    ui(new Ui::FileExplorer),
    m_fileSystem(new QFileSystemModel(this)),
    m_tableModel(new FileExplorerTableModel(this))
{
    //настраиваем UI
    ui->setupUi(this);
    //устанавливаем корневой путь в файловой модели
    m_fileSystem->setRootPath(QDir::homePath());
    //устанавливаем модель
    ui->treeView->setModel(m_fileSystem);
    //устанавливаем ратяжение колонок в qtreeview по размеру содержимого
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //устанавливаем модель
    ui->tableView->setModel(m_tableModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //устанавливаем соответсвующую стратегию
    if (strat_type == StrategyType::byFolder)
        m_strategy = new PercentageStrategyByFile;
    else
        m_strategy = new PercentageStrategyByType;

    //настраиваем сигнально-слотный механизм
    connect(ui->strategyComBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileExplorer::setPercentageStrategy);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileExplorer::folderChanged);
    connect(ui->sortComBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileExplorer::processFileSorting);
}

FileExplorer::~FileExplorer()
{
    delete ui;
}

void FileExplorer::folderChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
    //обновляем текущий путь до новой выбранной папки
    m_currentPath = m_fileSystem->filePath(selected.indexes()[0]);
    //обновляем данные в модели
    auto data = m_strategy->calculate(m_currentPath);
    m_tableModel->setFilesData(data);
    //обновляем отображение данных модели
    processFileSorting(ui->sortComBox->currentIndex());
}

bool FileExplorer::processFileSorting(int index)
{
    switch (index) {
    case 0:
        m_tableModel->sort([](const QPair<QString, uint64_t>& l, const QPair<QString, uint64_t>& r)
                           {
                               return l.second > r.second;
                           });
        return true;
    case 1:
        m_tableModel->sort([](const QPair<QString, uint64_t>& l, const QPair<QString, uint64_t>& r)
                           {
                               return l.first > r.first;
                           });
        return true;
    }

    return false;
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
    m_tableModel->setFilesData(data);
    //обновляем содержимое
    processFileSorting(ui->sortComBox->currentIndex());
}
