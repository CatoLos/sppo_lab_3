#include <FileExplorerTableModel.h>
#include <QLocale>
#include <algorithm>

FileExplorerTableModel::FileExplorerTableModel(QObject* parent, QVector<QPair<QString, uint64_t>> const& filesData) :
    QAbstractTableModel(parent),
    m_filesData(filesData)
{}

//обновление данных модели
void FileExplorerTableModel::setFilesData(QVector<QPair<QString, uint64_t>> const& filesData)
{
    clear();

    beginInsertRows(QModelIndex(), 0, filesData.size() - 1);
    m_filesData = filesData;
    endInsertRows();
}

void FileExplorerTableModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(order);

    if (m_filesData.size() > 1)
    {
       switch (column)
       {
       case 0:
          std::sort(m_filesData.begin(), m_filesData.end() - 1,
             [](const QPair<QString, uint64_t>& l, const QPair<QString, uint64_t>& r)
             {
                return l.first > r.first;
             });
          break;
       case 1:
          std::sort(m_filesData.begin(), m_filesData.end() - 1,
             [](const QPair<QString, uint64_t>& l, const QPair<QString, uint64_t>& r)
             {
                return l.second > r.second;
             });
          break;
       }

        emit dataChanged(createIndex(0,0), createIndex(m_filesData.size() - 2, 2));
    }
}

void FileExplorerTableModel::clear()
{
    if(!m_filesData.empty())
    {
        beginRemoveRows(QModelIndex(), 0, m_filesData.size() - 1);
        m_filesData.clear();
        endRemoveRows();
    }
}

QVector<QPair<QString, uint64_t>>const& FileExplorerTableModel::getData()
{
    return m_filesData;
}

int FileExplorerTableModel::rowCount(const QModelIndex&) const
{
    //Если в папке были файлы, то в конец m_filesData всегда пишется общий размер папка
    //Если файлов и вложенных папок в выбранной папки не было, тогда в m_filesData не пишется итоговый размер папки
    return m_filesData.size();
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
    if (role == Qt::DisplayRole && index.isValid() && index.row() < m_filesData.size())
    {
        switch (index.column())
        {
            //если нужна первая колонка
        case 0:
            return QString(m_filesData[index.row()].first); //возвращаем название сущности
            //если нужна вторая колонка
        case 1:
            //если сущность не пустая
            return QLocale(QLocale::English).formattedDataSize(m_filesData[index.row()].second);
            //если нужна третья колонка
        case 2:
            //процентное соотношение

            double totalSize = m_filesData[m_filesData.size() - 1].second;

            if(totalSize > 0)
            {
                double percent = double(m_filesData[index.row()].second) / totalSize * 100;
                return (percent >= 0.01 ? QString::number(percent, 'f', 2) : "< 0.01");
            }

            return QString();
        }
    }

    return QVariant();
}
