#include <QApplication>
#include "FileExplorer.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    FileExplorer exp;

    exp.show();

    return a.exec();
}
