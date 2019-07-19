#include "ui_project.h"

#include <QCoreApplication>
#include <QDebug>
#include <QtWidgets/QApplication>



void Ui_Project::setupTree(MainWindow* w,QTreeWidget *treeWidget)
{
    this->tree = treeWidget;
    FillTree();
}

void Ui_Project::FillTree()
{
    tree->clear();
    tree->setHeaderLabel("project");
    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();

    QString applicationDirPath = QCoreApplication::applicationDirPath();
    string path =  applicationDirPath.toStdString();
    size_t index = path.find("qt_proj/");
    path = path.substr(0, index);
    path += "resources";
    FillDirectory(path);
}

void Ui_Project::FillDirectory(string path)
{
    TreeNode *root = new TreeNode(path,tree);
    DrawChild(root);
    tree->expandAll();
    delete root;
}


void Ui_Project::DrawChild(TreeNode *parent)
{
    if(parent->childs)
    {
        for(int i=0;i<parent->child_num;i++)
        {
             auto node =  new TreeNode(parent->childs[i],parent->item);
             DrawChild(node);
        }
    }
}
