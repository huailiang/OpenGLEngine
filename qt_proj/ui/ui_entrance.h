/********************************************************************************
** Form generated from reading UI file 'ui_project.ui'
**
** Created by: Qt User Interface Compiler version 5.11.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECT_H
#define UI_PROJECT_H

#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <qobject.h>
#include <string>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <QDir>
#include <vector>


using namespace std;

QT_BEGIN_NAMESPACE


struct TreeNode
{
    string path;
    QTreeWidgetItem* item;
    string* childs;
    int child_num;


    TreeNode(string path, QTreeWidgetItem* parent)
    {
        this->path = path;
        string name = getName();
        this->item = new QTreeWidgetItem(parent, QStringList(QString(name.c_str())));
        setup();
    }

    TreeNode(string path, QTreeWidget* parent)
    {
        this->path = path;
        string name = getName();
        this->item = new QTreeWidgetItem(parent, QStringList(QString(name.c_str())));
        setup();
    }

    ~TreeNode()
    {
        delete [] childs;
        child_num = 0;
    }

    void setup()
    {
        QDir dir(path.c_str());
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList list =dir.entryInfoList();
        child_num = list.count();

        for(int i =0; i<child_num; i++)
        {
            QTreeWidgetItem *it = new QTreeWidgetItem(this->item, QStringList(list[i].fileName()));
            this->item->addChild(it);

            QObject::connect(it->treeWidget(),SIGNAL(itemClicked(QTreeWidgetItem*,int)), window,SLOT(checkself(QTreeWidgetItem* ,int)));
        }
        dir.setFilter(QDir::Dirs);
        list =dir.entryInfoList();
        vector<string> vec;

        for(int i=0;i<list.count();i++)
        {
            string name = list[i].filePath().toStdString();
            if(!name.empty() )
            {
                string t = name.substr(name.rfind("/"));
                if(t!="/." && t!= "/..")
                    vec.push_back(name);
            }
        }
        this->child_num = (int)vec.size();
        this->childs = new string[child_num];
        for(int i=0;i<child_num;i++)
        {
            childs[i] = vec[i];
        }
    }

    string getName()
    {
        size_t indx = path.rfind("/");
        string name =  path.substr(indx);
        if(name[0] == '/') return name.substr(1);
        return name;
    }

};

class Ui_Project
{
public:
     void setupTree(QTreeWidget *treeWidget);

private:

    void FillDirectory(string path);

    void FillTree();

    void DrawChild(TreeNode *parent);

private:

    QTreeWidget *tree;

};



QT_END_NAMESPACE

#endif // UI_PROJECT_H
