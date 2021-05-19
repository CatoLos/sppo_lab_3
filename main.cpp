#include <QTextStream>
#include "PercentageStrategyByFile.h"
#include "PercentageStrategyByType.h"
#include "StrategyContext.h"

int main(int argc, char* argv[])
{
    enum Status
    {
        OK,
        ERROR
    } status = OK;

    QTextStream qcout(stdout);
    QTextStream qcin(stdin);
    QString line;
    StrategyContext context(new PercentageStrategyByFile);

    while (status == OK)
    {
        qcout << "Enter line: " << flush;

        if (qcin.readLineInto(&line) && !line.isEmpty())
        {
            QStringList args = line.split(' ');

            QString arg = args.takeFirst();
            status = ERROR;

            if (arg == "set")
            {
                arg = args.takeFirst();

                if (arg == "ext")
                {
                    context.setStrategy(new PercentageStrategyByType);
                    status = OK;
                }
                else if (arg == "file")
                {
                    context.setStrategy(new PercentageStrategyByFile);
                    status = OK;
                }
            }
            else if (arg == "show")
            {
                arg = args.takeFirst();

                if (arg.startsWith('\"'))
                    arg.remove(0, 1);

                if (arg.endsWith('\"'))
                    arg.chop(1);

                context.calculate(arg);
                context.sort();
                auto data = context.getData();

                if (!data.isEmpty())
                {
                    qcout << endl;
                    double totalSize = data.last().second;

                    for (int i = 0, size = data.size() - 1; i < size; ++i)
                    {
                        qcout << data[i].first << " --- " << data[i].second << " bytes --- ";

                        if (totalSize > 0)
                        {
                            double percent = data[i].second / totalSize * 100;

                            qcout << (percent >= 0.01 ? QString::number(percent, 'f', 2) : "< 0.01") << '%';
                        }

                        qcout << endl;
                    }

                    qcout << "Total size --- " << data.last().second << " bytes" << endl << endl;
                }

                status = OK;
            }

            if (!args.isEmpty())
                status = ERROR;
        }
    }

    if (status == ERROR)
        qcout << "Error" << endl;

    return 0;
}
