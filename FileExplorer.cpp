#include <FileExplorer.h>
#include "PercentageStrategyByFile.h"
#include "PercentageStrategyByType.h"
#include <ui_FileExplorer.h>


FileExplorer::FileExplorer(QWidget* parent, FileExplorer::StrategyType strat_type) :
    QWidget(parent),
    ui(new Ui::FileExplorer),
    file_sistem(new QFileSystemModel(this)),
    table_model(new FileExplorerTableModel(this))
{
    //настраиваем UI
    ui->setupUi(this);
    //устанавливаем корневой путь в файловой модели
    file_sistem->setRootPath(QDir::homePath());
    //устанавливаем модель
    ui->treeView->setModel(file_sistem);
    //устанавливаем ратяжение колонок в qtreeview по размеру содержимого
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //устанавливаем модель
    ui->tableView->setModel(table_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->sortBox->setCheckState(Qt::CheckState::Checked);

    //устанавливаем соответсвующую стратегию
    if (strat_type == StrategyType::byFolder)
        strat_context.setStrategy(new PercentageStrategyByFile);
    else
        strat_context.setStrategy(new PercentageStrategyByType);

    //настраиваем сигнально-слотный механизм
    connect(ui->Strategy_cbox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FileExplorer::setGroupingStrategy);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &FileExplorer::folderChanged);
    connect(ui->sortBox, &QCheckBox::stateChanged, this, &FileExplorer::processFileSorting);
}

void FileExplorer::folderChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    //обновляем текущий путь до новой выбранной папки
    current_path = file_sistem->filePath(selected.indexes()[0]);
    //обновляем данные в модели
    strat_context.calculate(current_path);
    table_model->setFilesSize(strat_context.getData());
    //обновляем отображение данных модели
    if (ui->sortBox->checkState() == Qt::CheckState::Checked)
        table_model->sort();

    //обновляем layout
    emit table_model->layoutChanged();
}

bool FileExplorer::processFileSorting(int state)
{
    if (state == Qt::CheckState::Checked)
    {
        table_model->sort();
        table_model->layoutChanged();
        return true;
    }

    return false;
}

void FileExplorer::setGroupingStrategy(qint32 const& index)
{
    //устанавливаем соответсвующую страте
    switch (index)
    {
    case 0:
        strat_context.setStrategy(new PercentageStrategyByFile());
        break;
    case 1:
        strat_context.setStrategy(new PercentageStrategyByType());
        break;
    }

    //если папка не выбрана, то выходим
    if (current_path.isEmpty())
        return;

    //обновляем данные модели
    strat_context.calculate(current_path);
    table_model->setFilesSize(strat_context.getData());
    //обновляем содержимое
    if (ui->sortBox->checkState() == Qt::CheckState::Checked)
        table_model->sort();

    //обновляем layout
    emit table_model->layoutChanged();
}