#include "weatherapp.h"
#include "ui_weatherapp.h"
#include <iostream>
#include <sstream>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

WeatherApp::WeatherApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WeatherApp)
{
    ui->setupUi(this);
    

}

WeatherApp::~WeatherApp()
{
    delete ui;
}

void WeatherApp::on_searchButton_clicked()
{
    std::string location = ui->searchBar->text().toStdString();
    displayTemperature(location);
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