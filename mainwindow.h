#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
#include <QSettings>
#include <QTranslator>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include "dialog.h"
#include <QSortFilterProxyModel>
#include <QTranslator>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlQueryModel *model1;///<Параметр модели 2 таблицы
    QString data1;
    QString data2;
    QString data3;
    QString data4;
    int colomnnum;///<параметр выбранного столбца
    int i;
    QString typeind;
    QString soderz;///<Параметр для столбцов, содержащих строки
    int soderzint;///<параметр значений по столбцу, содержащему числа
private:
    QTranslator Translator; ///<Создание перевода
    QSortFilterProxyModel *proxyModel;///<Прокси модель для сортировки по столбцам

private slots:
    void on_Connection_triggered();

    void on_Disconnection_triggered();

    void on_Add_triggered();

    void on_Delete_triggered();

    void on_Edit_triggered();


    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_action_triggered();

    void changeEvent(QEvent *event);

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
    void readSettings();
    void writeSettings();
    QSqlDatabase db;///<База данных
    QSqlQueryModel *model;

    QSqlQuery *qry;///<Переменная для ввода запросов к SQL
    QSqlQuery *qry1;///<Переменная для ввода запросов к SQL
    QSqlQuery *qry4;///<Переменная для ввода запросов к SQL
    QTranslator qtLanguageTranslator;

};
#endif // MAINWINDOW_H
