#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include <QTextEdit>
#include <QtCore/qt_windows.h>
#include <dialogs.h>
#include <QMenu>
#include <QMouseEvent>
#include <QMessageBox>


Dialog::Dialog(QWidget *parent,QSqlDatabase db, QString stolb1,QString stolb2,QString stolb3,QString stolb4,QString stolb5,QString stolb6,bool vkl):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ///Задание размеров окна(Фиксированного)
    Dialog::setFixedSize(664,668);
    model1 = new QSqlQueryModel;
    activ(db);
    zapolnenie(stolb1,stolb2,stolb3,stolb4,stolb5,stolb6);
    strin1 = stolb1;
    strin2 = stolb2;
    strin3 = stolb3;
    strin4 = stolb4;
    strin5 = stolb5;
    strin6 = stolb6;
    check_ok();
    ui->pushButton->setVisible(vkl);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
///Задание слотов для создания контекстного меню
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord()));
        /// Подключаем СЛОТ вызова контекстного меню
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
    vkluck = vkl;


//    ui->obect->setInputMask("postcod Street");
}

Dialog::~Dialog()
{
    delete model1;
    delete ui;
}
///Функция определения введеного значения в поле
void Dialog::on_nameautocrane_editingFinished()
{
    strin1 = ui->nameautocrane->text();
}

///Функция определения введеного значения в поле
void Dialog::on_wisota_editingFinished()
{
    strin2 = ui->wisota->text();
    check_ok();
}

///Функция определения введеного значения в поле
void Dialog::on_wes_editingFinished()
{
    strin3 = ui->wes->text();
    check_ok();
}

///Функция определения введеного значения в поле
void Dialog::on_obect_editingFinished()
{

    strin4 = ui->obect->text();
    check_ok();
}

///Функция определения введеного значения в поле
void Dialog::on_lastname_editingFinished()
{
    strin5 = ui->lastname->text();
    check_ok();
}

///Функция определения выбранного радиобокса
void Dialog::on_arend_clicked(bool checked)
{
    if (ui->arend->isChecked())
        strin6 = "true";
    else
        strin6 = "false";
    check_ok();
}

///Проверка наличия заполненных полей
void Dialog::check_ok()
{
    if(ui->nameautocrane->text() != "" and ui->wisota->text() != "" and ui->wes->text() != "" and ui->obect->text() != "" and ui->lastname->text() != "")
        ui->Acce->setEnabled(true);
    else{
        ui->Acce->setEnabled(false);
    }

}

///Функция соединения всех введеных данных в строку
void Dialog::on_Acce_clicked()
{
    endedit.append(strin1);
    endedit.append(strin2);
    endedit.append(strin3);
    endedit.append(strin4);
    endedit.append(strin5);
    endedit.append(strin6);
}
///Функция для заполнения полей даннми, она нужна при вызове окна в режиме редактирования
void Dialog::zapolnenie(QString arg1,QString arg2,QString arg3,QString arg4,QString arg5,QString arg6)
{

    ui->nameautocrane->setText(arg1);
    ui->wisota->setValue(arg2.toInt());
    ui->wes->setValue(arg3.toInt());
    ui->obect->setText(arg4);
    ui->lastname->setText(arg5);
    if(arg6 != "false")
        ui->arend->setChecked(true);
}
///Заполнение второй таблицы с фамилиями
void Dialog::activ(QSqlDatabase db)
{
    if(db.open()){
    qry1 = new QSqlQuery(db);
    qry1->prepare("select last_name from staf ORDER BY last_name ASC");
    qry1->exec();

    model1->setQuery(*qry1);
    ui->tableView->setModel(model1);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model1->setHeaderData(0,Qt::Horizontal,tr("Фамилии"));
    }
}



///Открытие окна для создания нового сотркудника
void Dialog::on_pushButton_clicked()
{
    QSqlQuery qry1;
    Dialogs dialogs;
    if (dialogs.exec()) /// показываем диалог, здесь стоим, пока диалог не закроют
    {
        data1= dialogs.data1;
        data2= dialogs.data1;
        data3= dialogs.data1;
        data4= dialogs.data1;
        counter = "accepted";
        ui->Acce->clicked();
    }

}
///Слот контексного меню для удаления стортудника
void Dialog::slotRemoveRecord()
{

    delit = model1->data(model1->index(ui->tableView->currentIndex().row(), 0)).toString();
    counter = "2";
    ui->Acce->clicked();
}
///Создание контекстного меню
void Dialog::slotCustomMenuRequested(QPoint pos)
{
    /// Создаем объект контекстного меню
       QMenu * menu = new QMenu(this);
       /// Создаём действия для контекстного меню
       if (vkluck == true)
       {
       QAction * deleteDevice = new QAction(trUtf8("Удалить"), this);
        connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); /// Обработчик удаления записи
        menu->addAction(deleteDevice);
       }
       QAction * watchDevice = new QAction(trUtf8("Подробнее о сотруднике"), this);
       /// Подключаем СЛОТы обработчики для действий контекстного меню
       connect(watchDevice, SIGNAL(triggered()), this, SLOT(watch()));
       ///Устанавливаем действия в меню
       menu->addAction(watchDevice);
       ///Вызываем контекстное меню
       menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}
///Слот для вывода информации о сотруднике
void Dialog::watch()
{
    delit = model1->data(model1->index(ui->tableView->currentIndex().row(), 0)).toString();
    counter = "watch";
    QSqlQuery qry1;
    QSqlQuery query;
    query.prepare("select last_name,first_name,email,phone from staf where last_name = ?");
    query.addBindValue(delit);
    query.exec();
    query.first();
    QString NameResume = query.value(0).toString();
    QString NameResume2 = query.value(1).toString();
    QString NameResume3 = query.value(2).toString();
    QString NameResume4 = query.value(3).toString();
    QMessageBox::information(this, "Данные сотрудника","Фамилия сотрудника: "+NameResume+"\n Имя сотрудника: "+ NameResume2+"\n Email: "+ NameResume3+"\n Номер телефона: "+ NameResume4);
}
