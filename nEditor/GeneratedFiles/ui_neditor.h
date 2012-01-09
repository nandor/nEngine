/********************************************************************************
** Form generated from reading UI file 'neditor.ui'
**
** Created: Mon Jan 9 22:43:02 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEDITOR_H
#define UI_NEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_nEditorClass
{
public:
    QAction *actionNew_Package;
    QAction *actionOpen;
    QAction *actionHelp;
    QAction *actionAbout;
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QTreeWidget *treeWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *nEditorClass)
    {
        if (nEditorClass->objectName().isEmpty())
            nEditorClass->setObjectName(QString::fromUtf8("nEditorClass"));
        nEditorClass->resize(600, 400);
        actionNew_Package = new QAction(nEditorClass);
        actionNew_Package->setObjectName(QString::fromUtf8("actionNew_Package"));
        actionOpen = new QAction(nEditorClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionHelp = new QAction(nEditorClass);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(nEditorClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralWidget = new QWidget(nEditorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(100, 80, 391, 201));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(horizontalLayoutWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        treeWidget = new QTreeWidget(frame);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setGeometry(QRect(0, 0, 401, 201));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(frame);

        nEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(nEditorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        nEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(nEditorClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        nEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(nEditorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        nEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew_Package);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuHelp->addSeparator();
        menuHelp->addAction(actionHelp);
        menuHelp->addAction(actionAbout);

        retranslateUi(nEditorClass);

        QMetaObject::connectSlotsByName(nEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *nEditorClass)
    {
        nEditorClass->setWindowTitle(QApplication::translate("nEditorClass", "nEditor", 0, QApplication::UnicodeUTF8));
        actionNew_Package->setText(QApplication::translate("nEditorClass", "New", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("nEditorClass", "Open", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("nEditorClass", "Help", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("nEditorClass", "About", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("nEditorClass", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("nEditorClass", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class nEditorClass: public Ui_nEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEDITOR_H
