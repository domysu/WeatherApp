#include "weatherapp.h"
#include "ui_weatherapp.h"
#include <iostream>
#include <sstream>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/DateTime.h>
#include <Poco/Timestamp.h>
#include <QDebug>
#include <ctime>
#include <chrono>


WeatherApp::WeatherApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WeatherApp) 


{
    ui->setupUi(this);
    ui->forecastInfo->hide();
    ui->tabWidget->hide();
    ui->tabWidgetLabel->hide();
    ui->forecastInfo->setWordWrap(true);
    


}

WeatherApp::~WeatherApp()
{
    delete ui;
}

void WeatherApp::on_forecastList_itemClicked(QListWidgetItem* item)
{
    ui->forecastInfo->setVisible(true);
    std::string latlon = item->data(Qt::UserRole).value<std::string>();

    double forecastTemperature = item->data(Qt::UserRole + 1).value<double>(); // Retrieve temperature from data
    int day = item->data(Qt::UserRole + 2).value<int>();
   // qDebug() << "In forecastList itemclicked function: " << latlon << " " << day << " " << forecastTemperature;
    // Extract the day of the week from the selected item's text
    QString forecastText = item->text();
    QStringList forecastParts = forecastText.split(":");
    if (forecastParts.size() > 1)
    {
        QString dayOfWeek = forecastParts[0].trimmed(); // Get the day of the week (e.g., "Sunday")

        getForecastInfo(latlon, day);
        

    }
    



}


void WeatherApp::on_searchButton_clicked()
{
    std::string location = ui->searchBar->text().toStdString();
    ui->searchBar->setFocus();
    originalSearchQuery = location;
    ui->searchBar->clear();
    displayTemperature(location);
    displayForecast(location);
    displayHourlyForecastTabs(location);
}
void WeatherApp::on_tabWidget_tabBarClicked(int index)
{
   
    hourlyForecastInfo(originalSearchQuery, index);

}
 

void WeatherApp::displayForecast(const std::string& location)
{
    std::string coordinates = getCoordinates(location);
    
   
    if (!coordinates.empty())
    {
        std::string latitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lat").toString();
        std::string longitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lon").toString();
        std::string latlon = "lat=" + latitude + "&lon=" + longitude;

        ui->forecastList->clear(); // Clear the forecast list before adding new items

        // Use a loop to get the forecast for each day (e.g., 5-day forecast)
        for (int day = 1; day < 5; ++day)
        {
            qDebug() << "Fetching forecast data for day" << day;
            double forecastTemperature = getForecast(latlon, day);

            if (forecastTemperature != -1.0)
            {
                std::string dayOfWeek = getDayOfWeek(day);

                // Create a string to represent the forecast information for the current day
                std::ostringstream oss;
                oss << dayOfWeek << ": " << forecastTemperature << " C";
                std::string forecastInfo = oss.str();

                // Add the forecast information as a new item in the QListWidget
                QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(forecastInfo));
               
                item->setData(Qt::UserRole, QVariant::fromValue(latlon)); // Save coordinates as data
                QString latlondq= item->data(Qt::UserRole).value<QString>();
                qDebug() << latlondq;
                item->setData(Qt::UserRole + 1, QVariant::fromValue(forecastTemperature)); // Save temperature as data
                item->setData(Qt::UserRole + 2, QVariant::fromValue(day));
                ui->forecastList->addItem(item);
            }
            else
            {
                // Handle the case when forecast data cannot be retrieved for the day
                ui->errorInfo->setText("Failed to retrieve forecast data.");
            }
        }
    }
    else
    {
        ui->searchInfo->setText("Failed to retrieve coordinates.");
    }
}


double WeatherApp::getForecast(const std::string& coordinates, int day)
{
    Poco::Net::HTTPClientSession session("api.openweathermap.org", 80);
    std::string url = "/data/2.5/forecast?" + coordinates + "&appid=5d313375c665cd03e039cf939e0327be&units=metric&cnt=5";
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, url, Poco::Net::HTTPMessage::HTTP_1_1);

    Poco::Net::HTTPResponse response;
    qDebug() << "in getForecast before sending request!";
    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);
    qDebug() << "in getForecast after sending request!" << response.getStatus();

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
    {
        qDebug() << "in getForecast if statement!";
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(rs);
        Poco::JSON::Object::Ptr rootObject = result.extract<Poco::JSON::Object::Ptr>();

        if (rootObject->has("list"))
        {
            Poco::JSON::Array::Ptr forecastArray = rootObject->getArray("list");
            if (day >= 0 && day < forecastArray->size())
            {
                Poco::JSON::Object::Ptr forecastObject = forecastArray->getObject(day);
                Poco::JSON::Object::Ptr mainObject = forecastObject->getObject("main");
                double forecastTemperature = mainObject->get("temp");
                return forecastTemperature;
            }
        }
    }

    // Handle the case when forecast data cannot be retrieved or is empty
    qDebug() << "Failed to retrieve forecast data!";
    return -1.0;
}


void WeatherApp::displayTemperature(const std::string& location)
{
    std::string coordinates = getCoordinates(location);

    if (!coordinates.empty())
    {
        std::string latitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lat").toString();
        std::string longitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lon").toString();
        std::string country = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("country").toString();
        std::string latlon = "lat=" + latitude + "&lon=" + longitude;

        double temperature = getTemperature(latlon);
        if (temperature != -1.0)
        {
            std::ostringstream oss;
            oss << "Location: " << location << ", " << country << std::endl;
            oss << "Temperature: " << temperature << " C";
            ui->searchInfo->setText(QString::fromStdString(oss.str()));
            
        }
        else
        {
            ui->searchInfo->setText("Failed to retrieve temperature.");
        }
    }
    else
    {
        ui->searchInfo->setText("Failed to retrieve coordinates.");
    }
}


double WeatherApp::getTemperature(const std::string& coordinates)
{
    Poco::Net::HTTPClientSession session("api.openweathermap.org", 80);
    std::string url = "/data/2.5/weather?" + coordinates + "&appid=5d313375c665cd03e039cf939e0327be&units=metric";
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, url, Poco::Net::HTTPMessage::HTTP_1_1);

    Poco::Net::HTTPResponse response;

    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
    {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(rs);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
        Poco::Dynamic::Var temperature = object->get("main").extract<Poco::JSON::Object::Ptr>()->get("temp");

        return temperature.convert<float>();
    }
    else
    {
        return -1.0;
    }
}




std::string WeatherApp::getCoordinates(const std::string& location)
{
    Poco::Net::HTTPClientSession session("api.openweathermap.org", 80);
    std::string url = "/geo/1.0/direct?q=" + location + "&limit=5&appid=5d313375c665cd03e039cf939e0327be";
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, url, Poco::Net::HTTPMessage::HTTP_1_1);

    Poco::Net::HTTPResponse response;

    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
    {
        std::stringstream responseData;
        responseData << rs.rdbuf();
        return responseData.str();
    }
    else
    {
        return "";
    }

}
std::string WeatherApp::getDayOfWeek(int day)
{
    // Get the current date and time
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

   
    std::tm* date = std::localtime(&t);
    date->tm_mday += day;
    std::mktime(date);

    // Get the day of the week as a string
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%A", date);
    return std::string(buffer);
}
void WeatherApp::getForecastInfo(const std::string& coordinates, int day)
{
    Poco::Net::HTTPClientSession session("api.openweathermap.org", 80);
    std::string url = "/data/2.5/forecast/daily?" + coordinates + "&appid=5d313375c665cd03e039cf939e0327be&units=metric&cnt=5";
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, url, Poco::Net::HTTPMessage::HTTP_1_1);
    Poco::Net::HTTPResponse response;

    //qDebug() << "in getForecastInfo before sending request!";
    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);
    //qDebug() << "in getForecastInfo after sending request!" << response.getStatus() << coordinates;

    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
    {
        try
        {
            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(rs);
            Poco::JSON::Object::Ptr rootObject = result.extract<Poco::JSON::Object::Ptr>();
            if (rootObject->has("list"))
            {
                qDebug() << "In getForecastInfo if statement!";
                Poco::JSON::Array::Ptr forecastArray = rootObject->getArray("list");
                if (day >= 0 && day < forecastArray->size())
                {
                    qDebug() << "In getForecastInfo second if statement!";
                    Poco::JSON::Object::Ptr forecastObject = forecastArray->getObject(day);
                    Poco::JSON::Object::Ptr mainObject = forecastObject->getObject("temp");
                    Poco::JSON::Array::Ptr weatherArray = forecastObject->getArray("weather");
                    Poco::JSON::Object::Ptr weatherObject = weatherArray->getObject(0);
                    std::stringstream responseStream;
                    responseStream << rs.rdbuf();
                    std::string jsonResponse = responseStream.str();
                    qDebug() << "JSON Response:" << QString::fromStdString(jsonResponse);

                    double maxTemperature = mainObject->getValue<double>("max");
                    double minTemperature = mainObject->getValue<double>("min");
                    std::string rainInfo = weatherObject->getValue<std::string>("description");

                    std::ostringstream oss;
                    oss << "Maximum temperature: " << maxTemperature << " C\n";
                    oss << "Minimum temperature: " << minTemperature << " C\n";
                    oss << "Weather conditions: " << rainInfo;
                    ui->forecastInfo->setText(QString::fromStdString(oss.str()));
                }
            }
        }
        catch (Poco::Exception& ex)
        {
            qDebug() << "Exception occurred while parsing forecast data: " << ex.displayText().c_str();
            ui->forecastInfo->setText("Failed to parse forecast data.");
        }
    }
    else
    {
        qDebug() << "Failed to retrieve forecast data! Status code: " << response.getStatus();
        ui->forecastInfo->setText("Failed to retrieve forecast data.");
    }
}

void WeatherApp::displayHourlyForecastTabs(const std::string& location)
{
    ui->tabWidget->show();
    std::string coordinates = getCoordinates(location);
    
    if (!coordinates.empty()) {
        std::string latitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lat").toString();
        std::string longitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lon").toString();
        std::string country = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("country").toString();
        std::string latlon = "lat=" + latitude + "&lon=" + longitude;

        Poco::Net::HTTPClientSession session("pro.openweathermap.org", 80);
        std::string url = "/data/2.5/forecast/hourly?" + latlon + "&appid=03e6e9c064ef314d8013524db009b12c&units=metric&cnt=4";
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, url, Poco::Net::HTTPMessage::HTTP_1_1);
        qDebug() << "in displayHourlyForecastTabs function";
        Poco::Net::HTTPResponse response;

        session.sendRequest(request);
        std::istream& rs = session.receiveResponse(response);
        qDebug() << "not in list forecasta" << response.getStatus();
        if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            qDebug() << "not in list forecastaaaaaaaa";
            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(rs);
            Poco::JSON::Object::Ptr rootObject = result.extract<Poco::JSON::Object::Ptr>();


            if (rootObject->has("list"))
            {
                qDebug() << "In list forecast";
                Poco::JSON::Array::Ptr forecastArray = rootObject->getArray("list");

                // Clear existing tabs before adding new ones
                ui->tabWidget->clear();

                // Get current time
                std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
                std::time_t t = std::chrono::system_clock::to_time_t(now);
                std::tm* date = std::localtime(&t);
                int currentHour = date->tm_hour;

                // Define time ranges for tabs (adjust as needed)
                int startHour = currentHour;
                int endHour = startHour + 5;
            

                // Create tabs and widgets for each time range
                for (int i = startHour; i < endHour; ++i)
                {

                    // Create a widget for the tab
                    QWidget* tabWidget = new QWidget;
                    
                    
            ui->tabWidget->addTab(tabWidget, QString::fromStdString(std::to_string(i) + ":00"));
            ui->tabWidgetLabel->show();
            


                }
            }
        }
    }
}
void WeatherApp::hourlyForecastInfo(const std::string& location, int index)
{
    std::string coordinates = getCoordinates(location);
    qDebug() << "In hourlyForecastInfo before checking if coordinates empty";
    if (!coordinates.empty()) {
        //qDebug() << "In hourlyForecastInfo after checking if coordinates empty";
        std::string latitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lat").toString();
        std::string longitude = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("lon").toString();
        std::string country = Poco::JSON::Parser().parse(coordinates).extract<Poco::JSON::Array::Ptr>()->getObject(0)->get("country").toString();
        std::string latlon = "lat=" + latitude + "&lon=" + longitude;

        Poco::Net::HTTPClientSession session("pro.openweathermap.org", 80);
        std::string url = "/data/2.5/forecast/hourly?" + latlon + "&appid=03e6e9c064ef314d8013524db009b12c&units=metric&cnt=8";
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, url, Poco::Net::HTTPMessage::HTTP_1_1);
   
        Poco::Net::HTTPResponse response;

        session.sendRequest(request);
        std::istream& rs = session.receiveResponse(response);
        //qDebug() << "In hourlyForecastInfo before checking if request is good" << response.getStatus();
        if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
        {
            
            Poco::JSON::Parser parser;
            Poco::Dynamic::Var result = parser.parse(rs);
            Poco::JSON::Object::Ptr rootObject = result.extract<Poco::JSON::Object::Ptr>();


            if (rootObject->has("list"))
            {
               
                Poco::JSON::Array::Ptr forecastArray = rootObject->getArray("list");
                Poco::JSON::Object::Ptr forecastObject = forecastArray->getObject(index);
                Poco::JSON::Object::Ptr mainObject = forecastObject->getObject("main");
                Poco::JSON::Array::Ptr weatherArray = forecastObject->getArray("weather");
                Poco::JSON::Object::Ptr weatherObject = weatherArray->getObject(0);
                double Temperature = mainObject->getValue<double>("temp");
                std::stringstream responseStream;
                responseStream << rs.rdbuf();
                std::string jsonResponse = responseStream.str();

                //qDebug() << "in HourlyForecastInfo function" << response.getStatus();
                std::ostringstream oss;
                

                switch (index)
                {
                case 0:
                    oss << Temperature;
                    ui->tabWidgetLabel->setText("Temperature: " + QString::fromStdString(oss.str()));
                    break;
                case 1:
                    oss << Temperature;
                    ui->tabWidgetLabel->setText("Temperature: " + QString::fromStdString(oss.str()));
                    break;
                case 2:
                    oss << Temperature;
                    ui->tabWidgetLabel->setText("Temperature: " + QString::fromStdString(oss.str()));
                    break;
                case 3:
                    oss << Temperature;
                    ui->tabWidgetLabel->setText("Temperature: " + QString::fromStdString(oss.str()));
                    break;
                case 4:
                    oss << Temperature;
                    ui->tabWidgetLabel->setText("Temperature: " + QString::fromStdString(oss.str()));
                    break;
                default:
                    ui->tabWidgetLabel->setText("Error");
                    break;



                }

            }
        }
    }


}




