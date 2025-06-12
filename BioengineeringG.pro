QT       += core gui
QT += core gui widgets printsupport  # 确保包含printsupport模块
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Aligner.cpp \
    CSVLoader.cpp \
    DESeq2Analyzer.cpp \
    DNAsequence.cpp \
    FASTAparser.cpp \
    Painter.cpp \
    Resultloader.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Aligner.h \
    AlignmentResult.h \
    CSVLoader.h \
    CountData.h \
    DESeq2Analyzer.h \
    DESeq2Result.h \
    DNAsequence.h \
    FASTAparser.h \
    Painter.h \
    Resultloader.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    BioengineeringG_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# # OpenCV 头文件路径（保持原样）
# INCLUDEPATH += "D:/Opencv/opencv/build/include"

# # OpenCV 库配置（关键修改）
# LIBS += -L"D:/Opencv/opencv/build/x64/vc16/lib"\
# -lopencv_world4110d


INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtCore
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
