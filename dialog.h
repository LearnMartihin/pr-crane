#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSettings>
#include <QTranslator>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr,QSqlDatabase db = QSqlDatabase(),QString stolb1 = "",QString stolb2 = "0",QString stolb3 = "0",QString stolb4 = "",QString stolb5 = "",QString stolb6 = "false",bool vkl=true);
    ~Dialog();
    QString strin1;///<Переменная получающая данные, передающиеся в форму
    QString strin2;///<Переменная получающая данные, передающиеся в форму
    QString strin3;///<Переменная получающая данные, передающиеся в форму
    QString strin4;///<Переменная получающая данные, передающиеся в форму
    QString strin5;///<Переменная получающая данные, передающиеся в форму
    QString strin6 = "false";
    QString endedit = "";
    QString delit;///<Переменная содержащая фамилию на удаление из БД

    QString data1;
    QString data2;
    QString data3;
    QString data4;
    QString counter;
    bool vkluck;///<Переменная определяющая включен ли режим редактирования или добавления строки




private slots:
    void on_nameautocrane_editingFinished();

    void on_wisota_editingFinished();

    void on_wes_editingFinished();

    void on_obect_editingFinished();

    void on_lastname_editingFinished();

    void on_arend_clicked(bool checked);

    void check_ok();

    void on_Acce_clicked();

    void zapolnenie(QString arg1,QString arg2,QString arg3,QString arg4,QString arg5,QString arg6);

    void activ(QSqlDatabase db);

    void on_pushButton_clicked();

    void slotRemoveRecord();
    void slotCustomMenuRequested(QPoint pos);
    void watch();

private:
    Ui::Dialog *ui;
    QSqlQueryModel *model1;
    QSqlQuery *qry1;
};

#endif // DIALOG_H
