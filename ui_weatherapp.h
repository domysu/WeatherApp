/********************************************************************************
** Form generated from reading UI file 'weatherapp.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEATHERAPP_H
#define UI_WEATHERAPP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WeatherApp
{
public:
    QAction *actionAsile;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QPushButton *searchButton;
    QLineEdit *searchBar;
    QLabel *searchInfo;
    QMenuBar *menubar;
    QMenu *menuTemperatura;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WeatherApp)
    {
        if (WeatherApp->objectName().isEmpty())
            WeatherApp->setObjectName("WeatherApp");
        WeatherApp->resize(565, 593);
        actionAsile = new QAction(WeatherApp);
        actionAsile->setObjectName("actionAsile");
        centralwidget = new QWidget(WeatherApp);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        searchButton = new QPushButton(centralwidget);
        searchButton->setObjectName("searchButton");

        verticalLayout->addWidget(searchButton);

        searchBar = new QLineEdit(centralwidget);
        searchBar->setObjectName("searchBar");
        searchBar->setFrame(true);

        verticalLayout->addWidget(searchBar);

        searchInfo = new QLabel(centralwidget);
        searchInfo->setObjectName("searchInfo");

        verticalLayout->addWidget(searchInfo);

        WeatherApp->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WeatherApp);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 565, 22));
        menuTemperatura = new QMenu(menubar);
        menuTemperatura->setObjectName("menuTemperatura");
        WeatherApp->setMenuBar(menubar);
        statusbar = new QStatusBar(WeatherApp);
        statusbar->setObjectName("statusbar");
        WeatherApp->setStatusBar(statusbar);

        menubar->addAction(menuTemperatura->menuAction());
        menuTemperatura->addAction(actionAsile);

        retranslateUi(WeatherApp);

        QMetaObject::connectSlotsByName(WeatherApp);
    } // setupUi

    void retranslateUi(QMainWindow *WeatherApp)
    {
        WeatherApp->setWindowTitle(QCoreApplication::translate("WeatherApp", "WeatherApp", nullptr));
        actionAsile->setText(QCoreApplication::translate("WeatherApp", "Asile", nullptr));
        searchButton->setText(QCoreApplication::translate("WeatherApp", "Search", nullptr));
        searchBar->setText(QString());
        searchInfo->setText(QString());
        menuTemperatura->setTitle(QCoreApplication::translate("WeatherApp", "TempCheck", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeatherApp: public Ui_WeatherApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEATHERAPP_H
