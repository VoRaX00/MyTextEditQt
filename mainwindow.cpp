#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QIODevice>
#include <QMessageBox>
#include <QPushButton>
#include <QCursor>
#include <QDebug>
#include <QMenu>
#include <QMenuBar>
#include <QPrinter>
#include <QPrintDialog>
#include <QToolBar>
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mdiArea = ui->mdiArea;
    setCentralWidget(mdiArea);
    setWindowIcon(QPixmap(":/icon/pen.png"));

    QToolBar* toolBar = ui->mainToolBar;
    toolBar->setMovable(false);

    QAction* comBox = toolBar->addWidget(ui->comboBox);
    QAction* comBox2 = toolBar->addWidget(ui->comboBox_2);
    QAction* sizeLab = toolBar->addWidget(ui->labelSize);
    QAction* boxSize = toolBar->addWidget(ui->sizeBox);

    setStyleSheet("QMainWindow { background-color: white; }"
              "QPushButton { background-color: white; color: black;border-radius: 10px; font: bold 10px; min-width: 5em; padding: 3px;}"
              "QComboBox {background-color: white; color: black; border-radius: 5px; padding: 1px 4px 1px 3px; min-width: 3em;}"
              "QTextEdit { background: white; color: black}"
              "QCursor {color : black}");


    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(ChangeLanguage(const QString&)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(ChangeStyle(const QString&)));

    createMenu();
    createHotKey();

}

void MainWindow::createHotKey()
{
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
}

void MainWindow::createMenu()
{
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* pSave = fileMenu->addAction(QPixmap(":/icon/save.png"),"Save");
    connect(pSave, SIGNAL(triggered()), this, SLOT(on_saveButton_clicked()));

    QAction* pOpen = fileMenu->addAction(QPixmap(":/icon/open.png"),"Open");
    connect(pOpen, SIGNAL(triggered()), this, SLOT(on_openButton_clicked()));

    QAction* pReadOnly = fileMenu->addAction(QPixmap(":/icon/open_book.png"),"Read only");
    connect(pReadOnly, SIGNAL(triggered()), this, SLOT(on_onlyReadButton_clicked()));

    QAction* pPrint = fileMenu->addAction("Printing");
    pPrint->setIcon(QPixmap(":/icon/print.png"));
    connect(pPrint, SIGNAL(triggered()), this, SLOT(on_printing()));

    fileMenu->addSeparator();
    QAction* pQuit = fileMenu->addAction(QPixmap(":/icon/exit.png"),"Exit");
    connect(pQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu* editMenu = menuBar()->addMenu("Edit");
    QAction* pNewDoc = editMenu->addAction(QPixmap(":/icon/docs.png"),"New Document");
    connect(pNewDoc, SIGNAL(triggered()), this, SLOT(on_createNewDocument()));


    QMenu* aboutMenu = menuBar()->addMenu("About");
    QAction* pRef = aboutMenu->addAction(QPixmap(":/icon/ref.png"),"Reference");
    connect(pRef, SIGNAL(triggered()), this, SLOT(on_refButton_clicked()));

}

void MainWindow::ChangeStyle(const QString &style){
    if(style == "Dark"){
       setStyleSheet("QMainWindow { background-color: grease; }"
                      "QRadioButton { color: white }"
                      "QPushButton { background-color: grease; color: white;border-radius: 10px; font: bold 10px; min-width: 5em; padding: 3px;}"
                      "QComboBox {background-color: grease; color: white; border-radius: 5px; padding: 1px 4px 1px 3px; min-width: 3em;}"
                      "QTextEdit { background: DimGray; color: white}"
                     "QLabel {color : white;}"
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

void MainWindow::on_printing(){
    QPrinter printer;

    QMdiSubWindow* currentWindow = mdiArea->activeSubWindow();
    if (currentWindow==nullptr)
        return;

    QPrintDialog dlg(&printer, this);

    dlg.setWindowTitle("Print");
    if(dlg.exec()!=QDialog::Accepted)
        return;
    else {
        QWidget* widget = currentWindow->widget();
        QTextEdit* textEdit = (QTextEdit*)widget;
        if(textEdit!=nullptr)
        {
            textEdit->print(&printer);
        }
    }
}

void MainWindow::on_saveButton_clicked()
{
    QMdiSubWindow* currentWindow = mdiArea->activeSubWindow();
    if (currentWindow==nullptr)
        return;

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
        QWidget* widget = currentWindow->widget();
        QTextEdit* textEdit = (QTextEdit*)widget;
        if(textEdit!=nullptr)
        {
            QString text = textEdit->toPlainText();
            QByteArray ba = text.toUtf8();
            file.write(ba, ba.length());
        }
        else{
            return;
        }
    }
}

void MainWindow::on_openButton_clicked()
{
    QTextEdit* textEdit = new QTextEdit();
    textEdit->setReadOnly(false);
    path = QFileDialog::getOpenFileName(this, "Открыть");
    if(path.isEmpty())
    {
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray ba = file.readAll();
        QString text(ba);
        textEdit->setPlainText(text);
        QMdiSubWindow* newWindow = mdiArea->addSubWindow(textEdit);
        newWindow->setWindowIcon(QPixmap(":/icon/pen.png"));
        if(indexWindow==0)
            newWindow->setWindowTitle(QString("Document"));
        else
            newWindow->setWindowTitle(QString("Document_")+QString::number(indexWindow));
        indexWindow++;
        newWindow->show();
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

void MainWindow::on_onlyReadButton_clicked()
{
    QTextEdit* textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    path = QFileDialog::getOpenFileName(this,"Только чтение");
    if(path.isEmpty())
    {
        return;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray ba = file.readAll();
        QString text(ba);
        textEdit->setPlainText(text);
        QMdiSubWindow* newWindow = mdiArea->addSubWindow(textEdit);
        newWindow->setWindowIcon(QPixmap(":/icon/pen.png"));
        if(indexWindow==0)
            newWindow->setWindowTitle(QString("Document"));
        else
            newWindow->setWindowTitle(QString("Document_")+QString::number(indexWindow));
        indexWindow++;
        newWindow->show();
    }
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


void MainWindow::on_sizeBox_valueChanged(int arg1)
{
    QMdiSubWindow* currentWindow = mdiArea->activeSubWindow();
    if(currentWindow!=nullptr)
    {
        QWidget* widget = currentWindow->widget();
        QTextEdit* textEdit = (QTextEdit*)widget;
        textEdit->setFontPointSize(arg1);
    }
}

void MainWindow::on_createNewDocument()
{
    QTextEdit* textEdit = new QTextEdit();
    textEdit->setFontPointSize(DEFAULT_TEXT_SIZE);
    QMdiSubWindow* newWindow = mdiArea->addSubWindow(textEdit);
    newWindow->setWindowIcon(QPixmap(":/icon/pen.png"));
    if(indexWindow==0)
        newWindow->setWindowTitle("Document");
    else
        newWindow->setWindowTitle(QString("Document_")+QString::number(indexWindow));

    indexWindow++;
    newWindow->show();
}
