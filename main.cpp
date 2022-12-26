#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/tr/QtLanguage_ru.qm");
    translator.installEventFilter(&translator);
    MainWindow w;
    w.show();

    return a.exec();
}
