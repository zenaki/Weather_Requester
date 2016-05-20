#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QtWebKitWidgets/QtWebKitWidgets>
#include <QSettings>
#include <QtXml>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSqlDatabase connect_db();
    void processing_XML(QString data);
    void write_XML(QByteArray data);
    void save_to_database(QStringList list_column_data, QStringList list_field_data);
    void get_from_database(int m);

    void set_interface();
    void set_current_weather();
    void set_table_local();
    void set_table_marine();

private slots:
    void fileIsReady(QNetworkReply *reply);

    void on_pb_request_clicked();
    void on_cb_mode_currentIndexChanged(int index);
    void on_cb_weather_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *NAM;
    QSqlDatabase database;
    int mode;
    QString lng; QString lat;
    QStringList column_value;
    QStringList field_value;

    QLabel *bottom_message;
};

#endif // MAINWINDOW_H
