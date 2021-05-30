
QT += core widgets gui
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        FileExplorer.cpp \
        FileExplorerTableModel.cpp \
        FilePercentageStrategy.cpp \
        PercentageStrategyByFile.cpp \
        PercentageStrategyByType.cpp \
        StrategyContext.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    FileExplorer.h \
    FileExplorerTableModel.h \
    FilePercentageStrategy.h \
    PercentageStrategyByFile.h \
    PercentageStrategyByType.h \
    StrategyContext.h

FORMS += \
    FileExplorer.ui
