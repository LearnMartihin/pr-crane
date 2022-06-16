#include "dialogs.h"
#include "ui_dialogs.h"
#include <QFile>
#include <QMessageBox>
#include <QtSql/QSqlError>
#include <string>
#include "dialog.h"
#include <QtSql/QSqlRecord>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QPushButton>
#include <QValidator>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QObject>
#include <QSortFilterProxyModel>
Dialogs::Dialogs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogs)
{
    ui->setupUi(this);
    check_ok();



}

Dialogs::~Dialogs()
{
    delete ui;
}
///Функция проверки введеных значений во все поля
void Dialogs::check_ok()
{
    QSqlQuery query;
    if(ui->FIO->text() !="" and ui->IMA->text()!="" and ui->EMAIL->text()!="" and ui->Phone->text()!="")
    {


        query.prepare("select last_name from staf where last_name = ?");
        query.addBindValue(data1);
        query.exec();
        query.first();
        QString NameResume = query.value(0).toString();
        qDebug()<<"Name"<<NameResume;
        if(data1!=NameResume)
        {
        ui->ok->setEnabled(true);
        }
        else
        {
            QMessageBox::warning(this, "Внимание","Введена уже существующая фамилия");
        }
    }
    else
    {
        ui->ok->setEnabled(false);
    }
}


///Функция передачи данных с поля в глобальную переменную
void Dialogs::on_FIO_textChanged(const QString &arg1)
{
    data1 = arg1;
    check_ok();
}

///Функция передачи данных с поля в глобальную переменную
void Dialogs::on_IMA_textChanged(const QString &arg1)
{

    data2 = arg1;
    check_ok();
}

///Функция передачи данных с поля в глобальную переменную
void Dialogs::on_EMAIL_textChanged(const QString &arg1)
{
    ///Задание валидатора для почты
    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                              QRegularExpression::CaseInsensitiveOption);
    ///Установка валидатора на поле
    ui->EMAIL->setValidator(new QRegularExpressionValidator(rx, this));
    ///Задание условного оператора для наглядности пользователю правильности ввода
    if(!ui->EMAIL->hasAcceptableInput())
    {
        ui->EMAIL->setStyleSheet("QLineEdit { color: red;}");
        ui->ok->setEnabled(false);
    }
    else
    {
        ui->EMAIL->setStyleSheet("QLineEdit { color: green;}");
    }
    data3 = arg1;
    check_ok();

}

///Функция передачи данных с поля в глобальную переменную
void Dialogs::on_Phone_textChanged(const QString &arg1)
{
    ///Задание валидатора для номера телефона
    QRegularExpression rx("\\b[0-9]{1}+\\-[0-9]{3}+\\-[0-9]{3}+\\-[0-9]{4}\\b",
                              QRegularExpression::CaseInsensitiveOption);
    ///Установка валидатора
    ui->Phone->setValidator(new QRegularExpressionValidator(rx, this));
    ///Задание условного оператора для наглядности пользователю правильности ввода
    if(!ui->Phone->hasAcceptableInput())
    {
        ui->Phone->setStyleSheet("QLineEdit { color: red;}");
        ui->ok->setEnabled(false);
    }
    else
    {
        ui->Phone->setStyleSheet("QLineEdit { color: green;}");
    }

    data4 = arg1;
    check_ok();
}

void Dialogs::on_ok_clicked()
{

}





void Dialogs::on_EMAIL_editingFinished()
{

}
