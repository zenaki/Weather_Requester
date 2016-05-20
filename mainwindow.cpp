#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Weather Requester (by PT. Daun Biru Engineering)");

    NAM = new QNetworkAccessManager();
    connect(NAM, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReady(QNetworkReply*)));

    bottom_message = new QLabel(this);
    this->ui->statusBar->addPermanentWidget(bottom_message,1);

    database = connect_db();
    database.open();

    if (database.isOpen()) {
        bottom_message->setStyleSheet("QLabel { color : green; }");
        bottom_message->setText("Database is Connected ..");
    } else {
        bottom_message->setStyleSheet("QLabel { color : red; }");
        bottom_message->setText("Database is not Connected .. err: " + database.lastError().text());
    }
    mode = 0;

    if (ui->cb_weather == 0) {
        if (ui->cb_mode->currentIndex() == 2) {
            ui->label_46->setText("Start Date");
            ui->label_46->setHidden(false);
            ui->label_49->setHidden(false);
            ui->dateEdit_start->setHidden(false);
            ui->label_58->setText("End Date");
            ui->label_58->setHidden(false);
            ui->label_61->setHidden(false);
            ui->dateEdit_end->setHidden(false);
        } else {
            ui->label_46->setHidden(true);
            ui->label_49->setHidden(true);
            ui->dateEdit_start->setHidden(true);
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        }
    } else {
        if (ui->cb_mode->currentIndex() == 2) {
            ui->label_46->setText("Date");
            ui->label_46->setHidden(false);
            ui->label_49->setHidden(false);
            ui->dateEdit_start->setHidden(false);
            ui->label_58->setText("End Date");
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        } else {
            ui->label_46->setHidden(true);
            ui->label_49->setHidden(true);
            ui->dateEdit_start->setHidden(true);
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        }
    }

    set_interface();
}

MainWindow::~MainWindow()
{
    if (database.isOpen())
        database.close();
    delete ui;
}

QSqlDatabase MainWindow::connect_db()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("local_schema");
    db.setUserName("root");
    db.setPassword("root");

    return (QSqlDatabase) db;
}

void MainWindow::processing_XML(QString data)
{
    QXmlStreamReader xml;
//    QXmlStreamAttributes attributes;
    xml.clear();
    xml.addData(data);
    QStringList column; QStringList field; int x = 0; QString temp;
    column_value.clear(); field_value.clear();
    int count = 0;

    while(!xml.atEnd() &&  !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (mode == 1) {
                if (xml.name() == "current_condition" || xml.name() == "weather") {
                    column_value.clear(); column.clear();
                    field_value.clear(); field.clear();
                    column_value.insert(count, "longitude"); column.insert(count, "longitude");
                    field_value.insert(count, lng); field.insert(count, lng); count++;
                    column_value.insert(count, "latitude"); column.insert(count, "latitude");
                    field_value.insert(count, lat); field.insert(count, lat); count++;
                }
                if (xml.name() == "observation_time") {column_value.insert(count, "observation_time");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "temp_C") {column_value.insert(count, "temp_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "temp_F") {column_value.insert(count, "temp_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherCode") {column_value.insert(count, "weather_code");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherIconUrl") {column_value.insert(count, "weather_icon");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherDesc") {column_value.insert(count, "weather_desc");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedMiles") {column_value.insert(count, "wind_speed_miles");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedKmph") {column_value.insert(count, "wind_speed_kmph");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddirDegree") {column_value.insert(count, "wind_dir_degree");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddir16Point") {column_value.insert(count, "wind_dir_16point");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "precipMM") {column_value.insert(count, "precip_mm");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "humidity") {column_value.insert(count, "humidity");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "visibility") {column_value.insert(count, "visibility");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "pressure") {column_value.insert(count, "pressure");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "cloudcover") {column_value.insert(count, "cloud_cover");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeC") {column_value.insert(count, "feels_like_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeF") {
                    column_value.insert(count, "feels_like_f");field_value.insert(count, xml.readElementText()); count++;
                    column_value.insert(count, "date");
                    field_value.insert(count, QDate::currentDate().toString("yyyy-MM-dd")); count++;
                    column_value.insert(count, "time");
                    field_value.insert(count, QTime::currentTime().toString("hh:mm:ss")); count++;
                    save_to_database(column_value,field_value);
                }
            } else if (mode == 2) {
                if (xml.name() == "weather") {
                    column_value.clear(); column.clear();
                    field_value.clear(); field.clear();
                    column_value.insert(count, "longitude"); column.insert(count, "longitude");
                    field_value.insert(count, lng); field.insert(count, lng); count++;
                    column_value.insert(count, "latitude"); column.insert(count, "latitude");
                    field_value.insert(count, lat); field.insert(count, lat); count++;
                }
                if (xml.name() == "date") {column_value.insert(count, "date");column.insert(count, "date");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "sunrise") {column_value.insert(count, "sunrise");column.insert(count, "sunrise");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "sunset") {column_value.insert(count, "sunset");column.insert(count, "sunset");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "moonrise") {column_value.insert(count, "moonrise");column.insert(count, "moonrise");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "moonset") {column_value.insert(count, "moonset");column.insert(count, "moonset");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "maxtempC") {column_value.insert(count, "max_temp_c");column.insert(count, "max_temp_c");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "maxtempF") {column_value.insert(count, "max_temp_f");column.insert(count, "max_temp_f");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "mintempC") {column_value.insert(count, "min_temp_c");column.insert(count, "min_temp_c");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "mintempF") {column_value.insert(count, "min_temp_f");column.insert(count, "min_temp_f");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "uvIndex") {column_value.insert(count, "uv_index");column.insert(count, "uv_index");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}

                if (xml.name() == "time") {column_value.insert(count, "time");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "tempC") {column_value.insert(count, "temp_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "tempF") {column_value.insert(count, "temp_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedMiles") {column_value.insert(count, "wind_speed_miles");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedKmph") {column_value.insert(count, "wind_speed_kmph");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddirDegree") {column_value.insert(count, "wind_dir_degree");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddir16Point") {column_value.insert(count, "wind_dir_16point");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherCode") {column_value.insert(count, "weather_code");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherIconUrl") {column_value.insert(count, "weather_icon");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherDesc") {column_value.insert(count, "weather_desc");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "precipMM") {column_value.insert(count, "precip_mm");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "humidity") {column_value.insert(count, "humidity");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "visibility") {column_value.insert(count, "visibility");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "pressure") {column_value.insert(count, "pressure");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "cloudcover") {column_value.insert(count, "cloud_cover");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "HeatIndexC") {column_value.insert(count, "heat_index_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "HeatIndexF") {column_value.insert(count, "heat_index_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "DewPointC") {column_value.insert(count, "dew_point_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "DewPointF") {column_value.insert(count, "dew_point_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindChillC") {column_value.insert(count, "wind_chill_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindChillF") {column_value.insert(count, "wind_chill_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindGustMiles") {column_value.insert(count, "wind_gust_miles");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindGustKmph") {column_value.insert(count, "wind_gust_kmph");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeC") {column_value.insert(count, "feels_like_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeF") {column_value.insert(count, "feels_like_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofrain") {column_value.insert(count, "chance_of_rain");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofremdry") {column_value.insert(count, "chance_of_remdry");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofwindy") {column_value.insert(count, "chance_of_windy");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofovercast") {column_value.insert(count, "chance_of_overcast");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofsunshine") {column_value.insert(count, "chance_of_sunshine");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceoffrost") {column_value.insert(count, "chance_of_frost");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofhightemp") {column_value.insert(count, "chance_of_hightemp");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceoffog") {column_value.insert(count, "chance_of_fog");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofsnow") {column_value.insert(count, "chance_of_snow");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "chanceofthunder") {
                    column_value.insert(count, "chance_of_thunder");field_value.insert(count, xml.readElementText()); count++;
                    if (x != 1) {
                        for (int i = column.length()-1; i >= 0; i--) {
                            column_value.insert(0,column.at(i));
                        }
                        for (int i = field.length()-1; i >= 0; i--) {
                            field_value.insert(0,field.at(i));
                        }
                    }
                    save_to_database(column_value,field_value);

                    column_value.clear();
                    field_value.clear();
                    count = 0; x = 0;
                }
            } else if (mode == 3) {
                if (xml.name() == "weather") {
                    column_value.clear(); column.clear();
                    field_value.clear(); field.clear();
                    column_value.insert(count, "longitude"); column.insert(count, "longitude");
                    field_value.insert(count, lng); field.insert(count, lng); count++;
                    column_value.insert(count, "latitude"); column.insert(count, "latitude");
                    field_value.insert(count, lat); field.insert(count, lat); count++;
                }
                if (xml.name() == "date") {column_value.insert(count, "date");column.insert(count, "date");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "sunrise") {column_value.insert(count, "sunrise");column.insert(count, "sunrise");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "sunset") {column_value.insert(count, "sunset");column.insert(count, "sunset");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "moonrise") {column_value.insert(count, "moonrise");column.insert(count, "moonrise");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "moonset") {column_value.insert(count, "moonset");column.insert(count, "moonset");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "maxtempC") {column_value.insert(count, "max_temp_c");column.insert(count, "max_temp_c");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "maxtempF") {column_value.insert(count, "max_temp_f");column.insert(count, "max_temp_f");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "mintempC") {column_value.insert(count, "min_temp_c");column.insert(count, "min_temp_c");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "mintempF") {column_value.insert(count, "min_temp_f");column.insert(count, "min_temp_f");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "uvIndex") {column_value.insert(count, "uv_index");column.insert(count, "uv_index");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}

                if (xml.name() == "time") {column_value.insert(count, "time");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "tempC") {column_value.insert(count, "temp_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "tempF") {column_value.insert(count, "temp_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedMiles") {column_value.insert(count, "wind_speed_miles");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedKmph") {column_value.insert(count, "wind_speed_kmph");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddirDegree") {column_value.insert(count, "wind_dir_degree");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddir16Point") {column_value.insert(count, "wind_dir_16point");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherCode") {column_value.insert(count, "weather_code");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherIconUrl") {column_value.insert(count, "weather_icon");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherDesc") {column_value.insert(count, "weather_desc");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "precipMM") {column_value.insert(count, "precip_mm");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "humidity") {column_value.insert(count, "humidity");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "visibility") {column_value.insert(count, "visibility");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "pressure") {column_value.insert(count, "pressure");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "cloudcover") {column_value.insert(count, "cloud_cover");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "HeatIndexC") {column_value.insert(count, "heat_index_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "HeatIndexF") {column_value.insert(count, "heat_index_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "DewPointC") {column_value.insert(count, "dew_point_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "DewPointF") {column_value.insert(count, "dew_point_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindChillC") {column_value.insert(count, "wind_chill_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindChillF") {column_value.insert(count, "wind_chill_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindGustMiles") {column_value.insert(count, "wind_gust_miles");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindGustKmph") {column_value.insert(count, "wind_gust_kmph");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeC") {column_value.insert(count, "feels_like_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeF") {
                    column_value.insert(count, "feels_like_f");field_value.insert(count, xml.readElementText()); count++;
                    if (x != 1) {
                        for (int i = column.length()-1; i >= 0; i--) {
                            column_value.insert(0,column.at(i));
                        }
                        for (int i = field.length()-1; i >= 0; i--) {
                            field_value.insert(0,field.at(i));
                        }
                    }
                    save_to_database(column_value,field_value);

                    column_value.clear();
                    field_value.clear();
                    count = 0; x = 0;
                }
            } else if (mode == 4 || mode == 5) {
                if (xml.name() == "weather") {
                    column_value.insert(count, "longitude"); column.insert(count, "longitude");
                    field_value.insert(count, lng); field.insert(count, lng); count++;
                    column_value.insert(count, "latitude"); column.insert(count, "latitude");
                    field_value.insert(count, lat); field.insert(count, lat); count++;
                }
                if (xml.name() == "date") {column_value.insert(count, "date");column.insert(count, "date");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "sunrise") {column_value.insert(count, "sunrise");column.insert(count, "sunrise");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "sunset") {column_value.insert(count, "sunset");column.insert(count, "sunset");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "moonrise") {column_value.insert(count, "moonrise");column.insert(count, "moonrise");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "moonset") {column_value.insert(count, "moonset");column.insert(count, "moonset");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "maxtempC") {column_value.insert(count, "max_temp_c");column.insert(count, "max_temp_c");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "maxtempF") {column_value.insert(count, "max_temp_f");column.insert(count, "max_temp_f");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "mintempC") {column_value.insert(count, "min_temp_c");column.insert(count, "min_temp_c");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}
                if (xml.name() == "mintempF") {column_value.insert(count, "min_temp_f");column.insert(count, "min_temp_f");
                    temp = xml.readElementText();field_value.insert(count, temp);field.insert(count, temp); count++; x = 1;}

                if (xml.name() == "time") {column_value.insert(count, "time");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "tempC") {column_value.insert(count, "temp_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "tempF") {column_value.insert(count, "temp_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedMiles") {column_value.insert(count, "wind_speed_miles");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "windspeedKmph") {column_value.insert(count, "wind_speed_kmph");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddirDegree") {column_value.insert(count, "wind_dir_degree");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "winddir16Point") {column_value.insert(count, "wind_dir_16point");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherCode") {column_value.insert(count, "weather_code");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherIconUrl") {column_value.insert(count, "weather_icon");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "weatherDesc") {column_value.insert(count, "weather_desc");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "precipMM") {column_value.insert(count, "precip_mm");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "humidity") {column_value.insert(count, "humidity");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "visibility") {column_value.insert(count, "visibility");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "pressure") {column_value.insert(count, "pressure");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "cloudcover") {column_value.insert(count, "cloud_cover");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "HeatIndexC") {column_value.insert(count, "heat_index_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "HeatIndexF") {column_value.insert(count, "heat_index_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "DewPointC") {column_value.insert(count, "dew_point_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "DewPointF") {column_value.insert(count, "dew_point_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindChillC") {column_value.insert(count, "wind_chill_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindChillF") {column_value.insert(count, "wind_chill_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindGustMiles") {column_value.insert(count, "wind_gust_miles");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "WindGustKmph") {column_value.insert(count, "wind_gust_kmph");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeC") {column_value.insert(count, "feels_like_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "FeelsLikeF") {column_value.insert(count, "feels_like_f");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "sigHeight_m") {column_value.insert(count, "sig_height_m");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "swellHeight_m") {column_value.insert(count, "swell_height_m");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "swellHeight_ft") {column_value.insert(count, "swell_height_ft");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "swellDir") {column_value.insert(count, "swell_dir");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "swellDir16Point") {column_value.insert(count, "swell_dir_16point");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "swellPeriod_secs") {column_value.insert(count, "swell_period_secs");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "waterTemp_C") {column_value.insert(count, "water_temp_c");field_value.insert(count, xml.readElementText()); count++;}
                if (xml.name() == "waterTemp_F") {
                    column_value.insert(count, "water_temp_f");field_value.insert(count, xml.readElementText()); count++;
                    if (x != 1) {
                        for (int i = column.length()-1; i >= 0; i--) {
                            column_value.insert(0,column.at(i));
                        }
                        for (int i = field.length()-1; i >= 0; i--) {
                            field_value.insert(0,field.at(i));
                        }
                    }
                    save_to_database(column_value,field_value);

                    column_value.clear();
                    field_value.clear();
                    count = 0; x = 0;
                }
            }
        }
    }

    column_value.clear();
    field_value.clear();
    mode = 0;
    set_interface();
    QMessageBox::information(this, "Requester Message", "Request Success ..!!\n");
}

void MainWindow::write_XML(QByteArray data)
{
    QString FileName;
    FileName.sprintf("DBE_XML_Weather/[%s]_[%s]_[%s].xml",
                     ui->cb_weather->currentText().toUtf8().data(),
                     ui->cb_mode->currentText().toUtf8().data(),
                     QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8().data());
    QFile File(FileName);
    if (!QDir("DBE_XML_Weather").exists()) {
        QDir().mkdir("DBE_XML_Weather");
    }
    if (File.open(QIODevice::ReadWrite|QIODevice::Text)) {
        File.write(data);
    }
}

void MainWindow::save_to_database(QStringList list_column_data, QStringList list_field_data)
{
    if (!database.isOpen()) {
//        QMessageBox::critical(this, "Database Error", "Database is not open ..\nCannot save request to database ..\n");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Database Error !!", "Database is not open ..\nRetry ??\n",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            database.close();
            database = connect_db();
            database.open();
        } else {
            if (database.isOpen())
                database.close();
            this->close();
        }
    }
    QSqlQuery query(database);
    QString column_string;
    for (int i = 0; i < list_column_data.length(); i++) {
        column_string = column_string + list_column_data.at(i) + ",";
    }
    column_string = column_string + "mode";
    QString field_string = "'";
    for (int i = 0; i < list_field_data.length(); i++) {
        field_string = field_string + list_field_data.at(i) + "','";
    }
    field_string = field_string + QString::number(mode) + "'";
    QString query_string;
    query_string.sprintf("replace into daunbiru_weather (%s) values (%s)", column_string.toUtf8().data(), field_string.toUtf8().data());
    qDebug() << query_string;
    query.prepare(query_string);
    query.exec();
}

void MainWindow::get_from_database(int m)
{
    if (!database.isOpen()) {
//        QMessageBox::critical(this, "Database Error", "Database is not open ..\nCannot save request to database ..\n");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Database Error !!", "Database is not open ..\nRetry ??\n",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            database.close();
            database = connect_db();
            database.open();
        } else {
            if (database.isOpen())
                database.close();
            this->close();
        }
    }
    QSqlQuery query(database);
    QString query_string;
    if (m == 1) {
        query_string = "select "
                       "    longitude,"
                       "    latitude,"
                       "    update_date,"
                       "    observation_time,"
                       "    temp_c,"
                       "    temp_f,"
                       "    weather_code,"
                       "    weather_icon,"
                       "    weather_desc,"
                       "    wind_speed_miles,"
                       "    wind_speed_kmph,"
                       "    wind_dir_degree,"
                       "    wind_dir_16point,"
                       "    precip_mm,"
                       "    humidity,"
                       "    visibility,"
                       "    pressure,"
                       "    cloud_cover,"
                       "    feels_like_c,"
                       "    feels_like_f "
                       "from daunbiru_weather "
                       "where mode = 1 and"
                       "      update_date = (select max(update_date) from daunbiru_weather where mode = 1);";
    } else if (m == 2) {
        query_string = "select "
                       "    b.description,"
                       "    a.longitude,"
                       "    a.latitude,"
                       "    a.update_date,"
                       "    a.time,"
                       "    a.temp_c,"
                       "    a.temp_f,"
                       "    a.weather_code,"
                       "    a.weather_icon,"
                       "    a.weather_desc,"
                       "    a.wind_speed_miles,"
                       "    a.wind_speed_kmph,"
                       "    a.wind_dir_degree,"
                       "    a.wind_dir_16point,"
                       "    a.precip_mm,"
                       "    a.humidity,"
                       "    a.visibility,"
                       "    a.pressure,"
                       "    a.cloud_cover,"
                       "    a.heat_index_c,"
                       "    a.heat_index_f,"
                       "    a.dew_point_c,"
                       "    a.dew_point_f,"
                       "    a.wind_chill_c,"
                       "    a.wind_chill_f,"
                       "    a.wind_gust_miles,"
                       "    a.wind_gust_kmph,"
                       "    a.feels_like_c,"
                       "    a.feels_like_f,"
                       "    a.chance_of_rain,"
                       "    a.chance_of_remdry,"
                       "    a.chance_of_windy,"
                       "    a.chance_of_overcast,"
                       "    a.chance_of_sunshine,"
                       "    a.chance_of_frost,"
                       "    a.chance_of_hightemp,"
                       "    a.chance_of_fog,"
                       "    a.chance_of_snow,"
                       "    a.chance_of_thunder "
                       "from daunbiru_weather a, daunbiru_flag_weather b "
                       "where (a.mode = 2 or a.mode = 3) and"
                       "      a.update_date <= now() and a.update_date >= now() - interval 1 year and"
                       "      a.mode = b.id "
                       "order by a.update_date desc;";
    } else if (m == 3) {
        query_string = "select "
                       "    b.description,"
                       "    a.longitude,"
                       "    a.latitude,"
                       "    a.update_date,"
                       "    a.time,"
                       "    a.temp_c,"
                       "    a.temp_f,"
                       "    a.wind_speed_miles,"
                       "    a.wind_speed_kmph,"
                       "    a.wind_dir_degree,"
                       "    a.wind_dir_16point,"
                       "    a.weather_code,"
                       "    a.weather_icon,"
                       "    a.weather_desc,"
                       "    a.humidity,"
                       "    a.visibility,"
                       "    a.pressure,"
                       "    a.cloud_cover,"
                       "    a.heat_index_c,"
                       "    a.heat_index_f,"
                       "    a.dew_point_c,"
                       "    a.dew_point_f,"
                       "    a.wind_chill_c,"
                       "    a.wind_chill_f,"
                       "    a.wind_gust_miles,"
                       "    a.wind_gust_kmph,"
                       "    a.feels_like_c,"
                       "    a.feels_like_f,"
                       "    a.sig_height_m,"
                       "    a.swell_height_m,"
                       "    a.swell_height_ft,"
                       "    a.swell_dir,"
                       "    a.swell_dir_16point,"
                       "    a.swell_period_secs,"
                       "    a.water_temp_c,"
                       "    a.water_temp_f "
                       "from daunbiru_weather a, daunbiru_flag_weather b "
                       "where (a.mode = 4 or a.mode = 5) and"
                       "      a.update_date <= now() and a.update_date  >= now() - interval 1 year and"
                       "      a.mode = b.id "
                       "order by a.update_date desc;";
    }
    qDebug() << query_string;
    query.prepare(query_string); int count = 0;
    column_value.clear(); field_value.clear(); QString temp;
    if (query.exec()) {
        if (query.first()) {
            if (m == 1) {
                do {
                    for (int i = 0; i < 20; i++) {
                        field_value.insert(count, query.value(i).toString());
                        count++;
                    }
                } while(query.next());
            } else if (m == 2) {
                do {
                    for (int i = 0; i < 39; i++) {
                        temp = temp + query.value(i).toString();
                        if (i != 38) temp = temp + ";*;";
                    }
                    field_value.insert(count, temp);
                    temp.clear();
                    count++;
                } while(query.next());
            } else if (m == 3) {
                do {
                    for (int i = 0; i < 36; i++) {
                        temp = temp + query.value(i).toString();
                        if (i != 35) temp = temp + ";*;";
                    }
                    field_value.insert(count, temp);
                    temp.clear();
                    count++;
                } while(query.next());
            }
        }
    }
}

void MainWindow::set_interface()
{
    set_current_weather();
    column_value.clear();
    field_value.clear();
    mode = 0;

    set_table_local();
    column_value.clear();
    field_value.clear();
    mode = 0;

    set_table_marine();
    column_value.clear();
    field_value.clear();
    mode = 0;

    ui->pb_request->setEnabled(true);
}

void MainWindow::set_current_weather()
{
    get_from_database(1);

    if (field_value.length() == 20) {
        ui->val_long_local->setText(field_value.at(0));
        ui->val_lat_local->setText(field_value.at(1));
        ui->val_date_local->setText(field_value.at(2));

        ui->val_obs_time->setText(field_value.at(3));
        ui->val_temp_c->setText(field_value.at(4));
        ui->val_temp_f->setText(field_value.at(5));
        ui->val_code->setText(field_value.at(6));
        ui->val_icon->setText(field_value.at(7));
        ui->val_desc->setText(field_value.at(8));
        ui->val_wind_speed_miles->setText(field_value.at(9));
        ui->val_wind_speed_kmph->setText(field_value.at(10));
        ui->val_wind_dir_degree->setText(field_value.at(11));
        ui->val_wind_dir_16point->setText(field_value.at(12));
        ui->val_precip_mm->setText(field_value.at(13));
        ui->val_humidity->setText(field_value.at(14));
        ui->val_visibility->setText(field_value.at(15));
        ui->val_pressure->setText(field_value.at(16));
        ui->val_cloud_cover->setText(field_value.at(17));
        ui->val_feels_like_c->setText(field_value.at(18));
        ui->val_feels_like_f->setText(field_value.at(19));
    }
}

void MainWindow::set_table_local()
{
    get_from_database(2);

    if (field_value.length() > 0) {
        QString str;
        QStringList list;
        ui->tabel_local->setRowCount(field_value.length());
        for (int i = 0; i < field_value.length(); i++) {
            str = field_value.at(i);
            list = str.split(";*;");
            for (int j = 0; j < list.length(); j++) {
                ui->tabel_local->setItem(i, j, new QTableWidgetItem(list.at(j)));
            }
        }
        ui->tabel_local->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void MainWindow::set_table_marine()
{
    get_from_database(3);

    if (field_value.length() > 0) {
        QString str;
        QStringList list;
        ui->tabel_marine->setRowCount(field_value.length());
        for (int i = 0; i < field_value.length(); i++) {
            str = field_value.at(i);
            list = str.split(";*;");
            for (int j = 0; j < list.length(); j++) {
                ui->tabel_marine->setItem(i, j, new QTableWidgetItem(list.at(j)));
            }
        }
        ui->tabel_marine->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void MainWindow::fileIsReady(QNetworkReply *reply)
{
    ui->pb_request->setEnabled(false);
    QByteArray xmlStr;
    xmlStr.clear();

    xmlStr = reply->readAll();
//    qDebug() << reply->readAll();
//    qDebug() << xmlStr;
    QDomDocument document;
    if (!document.setContent(xmlStr)) {
//        qDebug() << "Failed to open xml file";
        QMessageBox::critical(this, "Reqest Message", "Request Failed ..\n");
        ui->pb_request->setEnabled(true);
    } else {
//        QDomElement root = document.firstChildElement();
//        qDebug() << xmlStr;
        write_XML(xmlStr);
        processing_XML(xmlStr);
    }
}

void MainWindow::on_pb_request_clicked()
{
    QString Key = "key";
    QString Longitude = QString::number(ui->dbl_longitude->value());
    Longitude.replace(Longitude.indexOf(",")-1,1,".");
    QString Latitude = QString::number(ui->dbl_latitude->value());
    Latitude.replace(Longitude.indexOf(",")-1,1,".");
    QString StartDate = ui->dateEdit_start->date().toString("yyyy-MM-dd");
    QString EndDate = ui->dateEdit_end->date().toString("yyyy-MM-dd");
    QString xml_url;
    if (ui->cb_weather->currentIndex() == 0) {
        if (ui->cb_mode->currentIndex() == 0) {
            xml_url.sprintf("http://api.worldweatheronline.com/premium/v1/weather.ashx?key=%s&q=%s,%s&format=xml&fx=no&cc=yes&tp=3",
                            Key.toUtf8().data(),
                            Latitude.toUtf8().data(),
                            Longitude.toUtf8().data());
            mode = 1;
        } else if (ui->cb_mode->currentIndex() == 1) {
            xml_url.sprintf("http://api.worldweatheronline.com/premium/v1/weather.ashx?key=%s&q=%s,%s&format=xml&fx=yes&cc=no&tp=3",
                            Key.toUtf8().data(),
                            Latitude.toUtf8().data(),
                            Longitude.toUtf8().data());
            mode = 2;
        } else if (ui->cb_mode->currentIndex() == 2) {
            xml_url.sprintf("http://api.worldweatheronline.com/premium/v1/past-weather.ashx?key=%s&q=%s,%s&format=xml&date=%s&enddate=%s&tp=3",
                            Key.toUtf8().data(),
                            Latitude.toUtf8().data(),
                            Longitude.toUtf8().data(),
                            StartDate.toUtf8().data(),
                            EndDate.toUtf8().data());
            mode = 3;
        }
    } else {
        if (ui->cb_mode->currentIndex() == 1) {
            xml_url.sprintf("http://api.worldweatheronline.com/premium/v1/marine.ashx?key=%s&q=%s,%s&format=xml&tide=no&tp=3",
                            Key.toUtf8().data(),
                            Latitude.toUtf8().data(),
                            Longitude.toUtf8().data());
            mode = 4;
        } else if (ui->cb_mode->currentIndex() == 2) {
            xml_url.sprintf("http://api.worldweatheronline.com/premium/v1/past-marine.ashx?key=%s&q=%s,%s&format=xml&date=%s&tp=3",
                            Key.toUtf8().data(),
                            Latitude.toUtf8().data(),
                            Longitude.toUtf8().data(),
                            StartDate.toUtf8().data());
            mode = 5;
        } else {
            QMessageBox::critical(this, "Weather Error", "Cannot get data from current Marine ..");
            mode = 0;
        }
    }
    lng = Longitude;
    lat = Latitude;
    qDebug() << xml_url;
    QUrl url =  QUrl::fromEncoded(xml_url.toLocal8Bit().data());
    QNetworkRequest request(url);
    NAM->get(request);
}

void MainWindow::on_cb_mode_currentIndexChanged(int index)
{
    if (ui->cb_weather->currentIndex() == 0) {
        if (index == 2) {
            ui->label_46->setText("Start Date");
            ui->label_46->setHidden(false);
            ui->label_49->setHidden(false);
            ui->dateEdit_start->setHidden(false);
            ui->label_58->setText("End Date");
            ui->label_58->setHidden(false);
            ui->label_61->setHidden(false);
            ui->dateEdit_end->setHidden(false);
        } else {
            ui->label_46->setHidden(true);
            ui->label_49->setHidden(true);
            ui->dateEdit_start->setHidden(true);
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        }
    } else {
        if (index == 2) {
            ui->label_46->setText("Date");
            ui->label_46->setHidden(false);
            ui->label_49->setHidden(false);
            ui->dateEdit_start->setHidden(false);
            ui->label_58->setText("End Date");
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        } else {
            ui->label_46->setHidden(true);
            ui->label_49->setHidden(true);
            ui->dateEdit_start->setHidden(true);
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        }
    }
}

void MainWindow::on_cb_weather_currentIndexChanged(int index)
{
    if (index == 0) {
        if (ui->cb_mode->currentIndex() == 2) {
            ui->label_46->setText("Start Date");
            ui->label_46->setHidden(false);
            ui->label_49->setHidden(false);
            ui->dateEdit_start->setHidden(false);
            ui->label_58->setText("End Date");
            ui->label_58->setHidden(false);
            ui->label_61->setHidden(false);
            ui->dateEdit_end->setHidden(false);
        } else {
            ui->label_46->setHidden(true);
            ui->label_49->setHidden(true);
            ui->dateEdit_start->setHidden(true);
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        }
    } else {
        if (ui->cb_mode->currentIndex() == 2) {
            ui->label_46->setText("Date");
            ui->label_46->setHidden(false);
            ui->label_49->setHidden(false);
            ui->dateEdit_start->setHidden(false);
            ui->label_58->setText("End Date");
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        } else {
            ui->label_46->setHidden(true);
            ui->label_49->setHidden(true);
            ui->dateEdit_start->setHidden(true);
            ui->label_58->setHidden(true);
            ui->label_61->setHidden(true);
            ui->dateEdit_end->setHidden(true);
        }
    }
}
