#pragma once
#include <QVector>
#include <QPair>
#include "QtCharts"
#include "FileExplorerTableModel.h"

class FilesObserver
{
public:

   virtual ~FilesObserver() {};

   virtual void update(const QVector<QPair<QString, uint64_t>>& data, int sortIndex) = 0;
   virtual const QVector<QPair<QString, uint64_t>>& data() = 0;
   virtual void sort(int sortIndex) = 0;
};