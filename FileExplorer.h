#include <QWidget>
#include <QFileSystemModel>
#include <FileExplorerTableModel.h>
#include <QItemSelection>
#include "FilePercentageStrategy.h"

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
    ~FileExplorer();

public slots:
    //слот изменения стратегии
    void setPercentageStrategy(qint32 const& index);
    //слот изменения папки
    void folderChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:

    QFileSystemModel* m_fileSystem;
    //контекст стратегий
    FilePercentageStrategy* m_strategy;
    FileExplorerTableModel* m_tableModel;
    //тукущий путь до выбранной папки
    QString m_currentPath;
    Ui::FileExplorer* ui;
};
