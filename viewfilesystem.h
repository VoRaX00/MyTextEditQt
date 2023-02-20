#ifndef VIEWFILESYSTEM_H
#define VIEWFILESYSTEM_H

#include <QWidget>
#include <QPushButton>
#include <QTreeView>
#include <QApplication>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QComboBox>

Q_PROPERTY(QStandardItemModel *model READ getCurrentModel WRITE setNewModel)

class ViewFileSystem : public QWidget
{
    Q_OBJECT
public:
    explicit ViewFileSystem(QWidget* parent = nullptr);
    void createTree();
    QStandardItemModel* getCurrentModel() const { return model;}
    void setNewModel(QStandardItemModel* newModel);
    void rebuildModel(QString str);

private slots:
    void chgDisk(int index);
    void goMainPath();

private:
    QStandardItemModel* model;
    QGridLayout* layout;
    QTreeView* tree;
    QPushButton* mainPath;
    QComboBox *disckSelBox;
    QString currentPath;
};

#endif // VIEWFILESYSTEM_H
