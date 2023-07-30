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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WeatherApp
{
public:
    QAction *actionAsile;
    QWidget *centralwidget;
    QPushButton *searchButton;
    QLineEdit *searchBar;
    QLabel *searchInfo;
    QListWidget *forecastList;
    QLabel *errorInfo;
    QLabel *forecastInfo;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *WeatherApp)
    {
        if (WeatherApp->objectName().isEmpty())
            WeatherApp->setObjectName("WeatherApp");
        WeatherApp->resize(510, 523);
        actionAsile = new QAction(WeatherApp);
        actionAsile->setObjectName("actionAsile");
        centralwidget = new QWidget(WeatherApp);
        centralwidget->setObjectName("centralwidget");
        searchButton = new QPushButton(centralwidget);
        searchButton->setObjectName("searchButton");
        searchButton->setGeometry(QRect(10, 50, 491, 24));
        searchBar = new QLineEdit(centralwidget);
        searchBar->setObjectName("searchBar");
        searchBar->setGeometry(QRect(10, 20, 491, 21));
        searchBar->setFrame(true);
        searchInfo = new QLabel(centralwidget);
        searchInfo->setObjectName("searchInfo");
        searchInfo->setGeometry(QRect(10, 90, 491, 121));
        forecastList = new QListWidget(centralwidget);
        forecastList->setObjectName("forecastList");
        forecastList->setGeometry(QRect(10, 260, 491, 192));
        errorInfo = new QLabel(centralwidget);
        errorInfo->setObjectName("errorInfo");
        errorInfo->setGeometry(QRect(30, 460, 71, 16));
        forecastInfo = new QLabel(centralwidget);
        forecastInfo->setObjectName("forecastInfo");
        forecastInfo->setGeometry(QRect(340, 310, 151, 111));
        WeatherApp->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(WeatherApp);
        statusbar->setObjectName("statusbar");
        WeatherApp->setStatusBar(statusbar);

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
        errorInfo->setText(QString());
        forecastInfo->setText(QCoreApplication::translate("WeatherApp", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeatherApp: public Ui_WeatherApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEATHERAPP_H
