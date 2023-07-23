#include "weatherapp.h"

#include <QApplication>
#include <sstream>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WeatherApp w;
    w.show();
    return a.exec();
}
