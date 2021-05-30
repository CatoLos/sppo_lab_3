#pragma once
#include <QAbstractTableModel>

//В качестве модели для QTableView был использован FileExplorerTableModel
//Это модель - наследник QAbstractTableModel.
//Обеспечивает нужное отображение данных 
//(названия колонок, количество строк, количество колонок, данных для каждой ячейки) 
//для QTableView, полученных от стратегий.

class FileExplorerTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    FileExplorerTableModel(QObject* parent = nullptr, QVector<QPair<QString, uint64_t>> const& Files_size = QVector<QPair<QString, uint64_t>>());

    //обновление данных, которые хранятся в модели
    void setFilesSize(QVector<QPair<QString, uint64_t>> const& Files_size);
    QVector<QPair<QString, uint64_t>> const& getData();
    void sort();

    //сколько строк нужно в таблице
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    //сколько колонок нужно в таблице
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    //название колонок
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    //предоставление данные для каждой ячейки в таблице
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    //данные модели
    QVector<QPair<QString, uint64_t>> files_size;
};

