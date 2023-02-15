#include "viewfilesystem.h"
#include <QDir>

ViewFileSystem::ViewFileSystem(QWidget* parent) : QWidget (parent), model(nullptr)
{
    layout = new QGridLayout(this);
    this->setLayout(layout);
    tree = new QTreeView(this);

    layout->addWidget(tree, 1,0,10,10);
    setMinimumSize(500,600);

    if(QSysInfo::productType() == "windows"){
        disckSelBox = new QComboBox(this);
        QFileInfoList list = QDir::drives();
        auto listdisk = list.begin();
        int size = list.size();
        for(int i=0; i<size; i++){
            disckSelBox->addItem(listdisk->path());
            listdisk++;
        }
        if(size>0){
            rebuildModel(list.at(0).path());
        }
        layout->addWidget(disckSelBox,0,0,1,2);
        connect(disckSelBox, SIGNAL(activated()), this, SLOT(goMainPath()));
    }
    else {
        mainPath = new QPushButton(this);
        mainPath->setText("/");
        layout->addWidget(mainPath,0,0,1,2);
        connect(mainPath,SIGNAL(clicked()), this, SLOT(goMainPath()));
        rebuildModel("/");
    }
}

void ViewFileSystem::chgDisk(int index){
    QFileInfoList list = QDir::drives();
    rebuildModel(list.at(index).path());
}

void ViewFileSystem::goMainPath(){
    rebuildModel("/");
}

void ViewFileSystem::setNewModel(QStandardItemModel *newModel){
    tree->setModel(newModel);
    model = newModel;
}

void ViewFileSystem::rebuildModel(QString str){
    currentPath = str;
    QStandardItemModel* model = new QStandardItemModel(this);
    QList<QStandardItem*> items;
    items.append(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon)), str));
    model->appendRow(items);

    QDir dir(str);
    dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs);
    QStringList list = dir.entryList();
    int size = list.count();
    QList<QStandardItem*>folders;
    for(int i=0; i<size; i++){
        QStandardItem* f = new
        QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)),
        list.at(i));
        folders.append(f);
    }

    items.at(0)->appendRow(folders);

    dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Files);
    size = list.size();
    QList<QStandardItem*>files;

    for(int i=0; i<size; i++){
        QStandardItem* it = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)),
                                              list.at(i));
        files.append(it);
    }
    items.at(0)->appendRow(files);
    setNewModel(model);
}


