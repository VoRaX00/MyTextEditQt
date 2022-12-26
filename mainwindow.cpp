#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QIODevice>
#include <QMessageBox>
#include <QPushButton>
#include <QCursor>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*setStyleSheet("QMainWindow { background-color: grease; }"
                  "QRadioButton { color: white }"
                  "QPushButton { background-color: grease; color: white;border-radius: 10px; font: bold 10px; min-width: 5em; padding: 3px;}"
                  "QComboBox {background-color: grease; color: white; border-radius: 5px; padding: 1px 4px 1px 3px; min-width: 3em;}"
                  "QTextEdit { background: DimGray; color: white}"
                  "QCursor {color : white}");
*/

    ui->saveButton->setText(tr("Save"));
    ui->openButton->setText(tr("Open"));
    ui->onlyReadButton->setText(tr("Read-only"));
    ui->refButton->setText(tr("Reference"));

    keyCtrlO = new QShortcut(this);
    keyCtrlO->setKey(Qt::CTRL + Qt::Key_O);
    connect(keyCtrlO, SIGNAL(activated()), this, SLOT(ShortCutCTRL_O()));

    keyCtrlS = new QShortcut(this);
    keyCtrlS->setKey(Qt::CTRL + Qt::Key_S);
    connect(keyCtrlS,SIGNAL(activated()), this, SLOT(ShortCutCTRL_S()));

    keyCtrlN = new QShortcut(this);
    keyCtrlN->setKey(Qt::CTRL + Qt::Key_N);
    connect(keyCtrlN,SIGNAL(activated()), this, SLOT(ShortCutCTRL_N()));

    keyCtrlQ = new QShortcut(this);
    keyCtrlQ->setKey(Qt::CTRL + Qt::Key_Q);
    connect(keyCtrlQ,SIGNAL(activated()), this, SLOT(ShortCutCTRL_Q()));


    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(ChangeLanguage(const QString&)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(ChangeStyle(const QString&)));
}


void MainWindow::ChangeStyle(const QString &style){

    if(style == "Dark"){
       setStyleSheet("QMainWindow { background-color: grease; }"
                      "QRadioButton { color: white }"
                      "QPushButton { background-color: grease; color: white;border-radius: 10px; font: bold 10px; min-width: 5em; padding: 3px;}"
                      "QComboBox {background-color: grease; color: white; border-radius: 5px; padding: 1px 4px 1px 3px; min-width: 3em;}"
                      "QTextEdit { background: DimGray; color: white}"
                      "QCursor {color : white}");
    }
    else{
        setStyleSheet("QMainWindow { background-color: white; }"
                  "QRadioButton { color: black }"
                  "QPushButton { background-color: white; color: black;border-radius: 10px; font: bold 10px; min-width: 5em; padding: 3px;}"
                  "QComboBox {background-color: white; color: black; border-radius: 5px; padding: 1px 4px 1px 3px; min-width: 3em;}"
                  "QTextEdit { background: white; color: black}"
                  "QCursor {color : black}");
    }

}


void MainWindow::ChangeLanguage(const QString &lang)
{
    if (lang == "RUS") {
        translator.load(":/tr/QtLanguage_ru.qm");
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    }
    else {
        translator.load(":/tr/QtLanguage_en.qm");
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_saveButton_clicked()
{
    if(path=="")
    {
        path = QFileDialog::getSaveFileName(this, "Сохранить");
    }

    if(path.isEmpty())
    {
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::WriteOnly)){
        QString text = ui->textEdit->toPlainText();
        QByteArray ba = text.toUtf8();
        file.write(ba, ba.length());
    }
}

void MainWindow::on_openButton_clicked()
{
    path = QFileDialog::getOpenFileName();
    if(path.isEmpty())
    {
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray ba = file.readAll();
        QString text(ba);
        ui->textEdit->setPlainText(text);
    }
}

void MainWindow::on_refButton_clicked()
{
    QFile file(":/ref/reference.txt");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray ba = file.readAll();
        QString text(ba);
        QMessageBox box;
        box.setWindowTitle("Reference");
        box.setText(text);
        box.exec();
    }
}

void MainWindow::on_onlyReadButton_clicked(bool checked)
{
    ui->textEdit->setReadOnly(checked);
}

void MainWindow::ShortCutCTRL_O()
{
    on_openButton_clicked();
}

void MainWindow::ShortCutCTRL_S(){
    path = QFileDialog::getSaveFileName(this, "Сохранить как");

    if(path.isEmpty())
    {
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::WriteOnly)){
        QString text = ui->textEdit->toPlainText();
        QByteArray ba = text.toUtf8();
        file.write(ba, ba.length());
    }
}

void MainWindow::ShortCutCTRL_N(){
    if(path!=""){
        on_saveButton_clicked();
    }
    path = "";
    ui->textEdit->clear();
}

void MainWindow::ShortCutCTRL_Q(){
    qApp->exit();
}

