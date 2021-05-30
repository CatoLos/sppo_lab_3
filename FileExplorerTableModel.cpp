#include <FileExplorerTableModel.h>
#include <QLocale>
#include <algorithm>

FileExplorerTableModel::FileExplorerTableModel(QObject* parent, QVector<QPair<QString, uint64_t>> const& Files_size) :
    QAbstractTableModel(parent),
    files_size(Files_size)
{}

//обновление данных модели
void FileExplorerTableModel::setFilesSize(QVector<QPair<QString, uint64_t>> const& Files_size)
{
    files_size = Files_size;
}

void FileExplorerTableModel::sort()
{
    if (files_size.size() > 1)
        std::sort(files_size.begin(), files_size.end() - 1, 
            [](const QPair<QString, uint64_t>& l, const QPair<QString, uint64_t>& r) 
            { 
                return l.second > r.second; 
            });
}

QVector<QPair<QString, uint64_t>>const& FileExplorerTableModel::getData()
{
    return files_size;
}

int FileExplorerTableModel::rowCount(const QModelIndex&) const
{
    //Если в папке были файлы, то в конец files_size всегда пишется общий размер папка
    //Если файлов и вложенных папок в выбранной папки не было, тогда в files_size не пишется итоговый размер папки
    return files_size.size();
}

int FileExplorerTableModel::columnCount(const QModelIndex&) const
{
    return 3;
}

QVariant FileExplorerTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            //если нужно название первой колонки
        case 0:
            return QString("Name");
            //если нужно название второй колонки
        case 1:
            return QString("Size");
            //если нужно название третьей колонки
        case 2:
            return QString("%");
        }
    }

    return QVariant();
}

QVariant FileExplorerTableModel::data(const QModelIndex& index, int role) const
{
    //проверка на корректность данных, по которым производится запрос
    if (role == Qt::DisplayRole && index.isValid() && index.row() <= files_size.size())
    {
        switch (index.column())
        {
            //если нужна первая колонка
        case 0:
            return QString(files_size[index.row()].first); //возвращаем название сущности
            //если нужна вторая колонка
        case 1:
            //если сущность не пустая
            return QLocale(QLocale::English).formattedDataSize(files_size[index.row()].second);
            //если нужна третья колонка
        case 2:
            //процентное соотношение 
            double percent = double(files_size[index.row()].second) / files_size[files_size.size() - 1].second * 100;

            //если сущность не пустая
            if (files_size[index.row()].second > 0)
                return (percent >= 1 ? QString::number(percent) : "< 1");
            else
                return QString();
        }
    }

    return QVariant();
}
