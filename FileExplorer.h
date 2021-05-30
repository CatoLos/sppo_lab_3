#include <QWidget>
#include <QFileSystemModel>
#include <StrategyContext.h>
#include <FileExplorerTableModel.h>
#include <QItemSelection>

namespace Ui
{
    class FileExplorer;
}

//класс, отвечающий за главное окно, здесь хрантяся все модели и все виды
class FileExplorer : public QWidget
{
    Q_OBJECT

public:

    //тип выбранной стратегии
    enum class StrategyType
    {
        byFolder = 0,
        byType = 2
    };

    FileExplorer(QWidget* parent = nullptr, FileExplorer::StrategyType strat_type = StrategyType::byFolder);

public slots:
    //слот изменени¤ стратегии
    void setGroupingStrategy(qint32 const& index);
    //слот изменени¤ папки
    void folderChanged(const QItemSelection& selected, const QItemSelection& deselected);
    bool processFileSorting(int state);

private:

    QFileSystemModel* file_sistem;
    //контекст стратегий
    StrategyContext strat_context;
    FileExplorerTableModel* table_model;
    //тукущий путь до выбранной папки
    QString current_path;
    Ui::FileExplorer* ui;
};