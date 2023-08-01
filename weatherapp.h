#ifndef WEATHERAPP_H
#define WEATHERAPP_H

#include <QMainWindow>
#include <QListWidgetItem>

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
    void displayForecast(const std::string& location);
    double getForecast(const std::string& coordinates, int day);
    double getTemperature(const std::string& coordinates);
    std::string getCoordinates(const std::string& location);
    std::string getDayOfWeek(int day);
    void getForecastInfo(const std::string& coordinates, int day);
    

    void on_forecastList_itemClicked(QListWidgetItem *item);

private:
    Ui::WeatherApp *ui;
};
#endif // WEATHERAPP_H
