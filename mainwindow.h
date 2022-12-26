#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QKeyEvent>
#include <QShortcut>

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

    void on_onlyReadButton_clicked(bool checked);

    void ShortCutCTRL_O();
    void ShortCutCTRL_S();
    void ShortCutCTRL_N();
    void ShortCutCTRL_Q();

private:
    Ui::MainWindow *ui;
    QTranslator translator;
    bool ok = false;
    QString path="";
    QShortcut *keyCtrlO, *keyCtrlS, *keyCtrlN, *keyCtrlQ;
};

#endif // MAINWINDOW_H
