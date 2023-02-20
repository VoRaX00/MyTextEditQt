#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QKeyEvent>
#include <QShortcut>
#include <QMdiArea>
#include <QList>
#include <viewfilesystem.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString language = "ENG";
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_saveButton_clicked();

    void on_openButton_clicked();

    void on_refButton_clicked();

    void ChangeLanguage(const QString&);

    void ChangeStyle(const QString&);

    void on_onlyReadButton_clicked();

    void on_printing();

    void ShortCutCTRL_O();
    void ShortCutCTRL_S();
    void ShortCutCTRL_N();
    void ShortCutCTRL_Q();

    void on_sizeBox_valueChanged(int arg1);

    void on_createNewDocument();

private:
    void createHotKey();
    void createMenu();

    Ui::MainWindow *ui;
    QTranslator translator;
    bool ok = false;
    QMdiArea* mdiArea;
    int indexWindow=0;
    QString path="";
    QShortcut *keyCtrlO, *keyCtrlS, *keyCtrlN, *keyCtrlQ;
    const int DEFAULT_TEXT_SIZE = 14;
};

#endif // MAINWINDOW_H
