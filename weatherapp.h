#ifndef WEATHERAPP_H
#define WEATHERAPP_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class WeatherApp; }
QT_END_NAMESPACE

class WeatherApp : public QMainWindow
{
    Q_OBJECT

public:
    WeatherApp(QWidget *parent = nullptr);
    ~WeatherApp();

private slots:


    void on_searchButton_clicked();
    void displayTemperature(const std::string& location);
    double getTemperature(const std::string& coordinates);
    std::string getCoordinates(const std::string& location);

private:
    Ui::WeatherApp *ui;
};
#endif // WEATHERAPP_H
