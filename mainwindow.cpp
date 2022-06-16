#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QtSql/QSqlError>
#include <string>
#include "dialog.h"
#include <QtSql/QSqlRecord>
#include <typeinfo>
#include <QTreeView>
#include <QAbstractTableModel>
#include <info.h>
#include <QSortFilterProxyModel>
#include <QTranslator>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();
    model = new QSqlQueryModel;
    model1 = new QSqlQueryModel;
    ui->Connection->setEnabled(true);
    ui->Disconnection->setEnabled(false);
    ui->Edit->setEnabled(false);
    ui->Add->setEnabled(false);
    ui->Delete->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionsMovable(true);
    proxyModel = new QSortFilterProxyModel;




}

///Функция для чтения сохраненых настроек окна
void MainWindow::readSettings()
{
    QSettings st("Settings");
    st.beginGroup("Window");
    resize(st.value("size",QSize(400,400)).toSize());
    move(st.value("position",QPoint(200,200)).toPoint());
    QString lang = st.value("language","en_US").toString();

//	if(lang == "en_US")
//		on_action_Lang_English_triggered();
//	else if(lang == "ru_RU")
//		on_action_Lang_Russian_triggered();
//	else if(lang == "it_IT")
//		on_action_Lang_Italian_triggered();
//	else
//		on_action_Lang_English_triggered();
    st.endGroup();
}
///сохранение настроек окна, которые выставил пользователь
void MainWindow::writeSettings()
{
    QSettings st("Settings");
    st.beginGroup("Window");
    st.setValue("size",size());
    st.setValue("position",pos());
    st.setValue("language",Translator.language());
    st.endGroup();
}


///Деструктор
MainWindow::~MainWindow()
{
    delete model;
    delete model1;
    delete qry;
    writeSettings();
    delete ui;
}



///Функция соединения формы с БД
void MainWindow::on_Connection_triggered()
{
    ///Соединение с БЖ
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setUserName("postgres");
    db.setPassword("parrol123");
    db.setDatabaseName("test_db");
///Проверка установленного соединения и если с ним все хорошо, то выводить в форму данные из БД
    if(db.open()){
        qry = new QSqlQuery(db);
        ///Данные первой табицы
        qry->prepare("select v.id,v.name_autocrane,v.max_height,v.max_weight,v.object,s.last_name,v.arenda from vedomost "
                        "v,staf s where v.id_stuf= s.id_s ORDER BY v.max_weight ASC");
        qry->exec();
///Данные второй таблицы
        qry1 = new QSqlQuery(db);
        qry1->prepare("select last_name from staf ORDER BY last_name ASC");
        qry1->exec();
///Перенос модели на форму
        model1->setQuery(*qry1);
        model->setQuery(*qry);
//        ui->tableView->setModel(model);
        proxyModel->setSourceModel(model);
            ui->tableView->setModel(proxyModel);
            ui->tableView->setSortingEnabled(true);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ///Задание имен столбцов
        model->setHeaderData(0,Qt::Horizontal,tr("Номер запроса"));
        model->setHeaderData(1,Qt::Horizontal,tr("Название крана"));
        model->setHeaderData(2,Qt::Horizontal,tr("Максимальная высота подъема"));
        model->setHeaderData(3,Qt::Horizontal,tr("Максимальная грузоподъемность"));
        model->setHeaderData(4,Qt::Horizontal,tr("На каком объекте находится"));
        model->setHeaderData(5,Qt::Horizontal,tr("Фамилия ответственного лица"));
        model->setHeaderData(6,Qt::Horizontal,tr("Находится ли кран в аренде"));

    }
    else{
        QMessageBox::critical(this,"Error",model->lastError().text());
        QMessageBox::warning(this, "Внимание","соединение не установлено, возможно сервер не активен");
    }
///Выставление флагов для кнопок, чтобы без соединения их нельзя было использовать
    ui->Connection->setEnabled(false);
    ui->Disconnection->setEnabled(true);
    ui->Edit->setEnabled(true);
    ui->Add->setEnabled(true);
    ui->Delete->setEnabled(true);
    ui->pushButton->setEnabled(true);

    ui->lineEdit->setEnabled(true);
//    ui->tableView->setSortingEnabled(true);
}
///Функция отключения программы от БД
void MainWindow::on_Disconnection_triggered()
{
    if(!db.isOpen())
        return;
    db.close();
    QSqlDatabase::removeDatabase("Connection");
    ui->tableView->setModel(0);
    ui->tableView->reset();
    ui->Connection->setEnabled(true);
    ui->Disconnection->setEnabled(false);
    ui->Edit->setEnabled(false);
    ui->Add->setEnabled(false);
    ui->Delete->setEnabled(false);
    ui->pushButton->setEnabled(false);

    ui->lineEdit->setEnabled(false);

//    ui->tableView->setSortingEnabled(false);
}
///Функция добавления записи в таблицу
void MainWindow::on_Add_triggered()
{

    Dialog dialog(this,db);
    if (dialog.exec()) /// показываем диалог, здесь стоим, пока диалог не закроют
    {
         QString strin1 = dialog.strin1;
         QString strin2 = dialog.strin2;
         QString strin3 = dialog.strin3;
         QString strin4 = dialog.strin4;
         QString strin5 = dialog.strin5;
         QString strin6 = dialog.strin6;
         QSqlQuery qry2;
         QSqlQuery qry3;
         QSqlQuery qry4;
         data1 = dialog.data1;
         data2 = dialog.data2;
         data3 = dialog.data3;
         data4 = dialog.data4;
         QString delit = dialog.delit;
        QString tex = dialog.counter;
        ///Использование запросов SQL для удаления в открытом окне добавления записи данных из второй таблицы
        if (tex == "2")
        {
            qry4.prepare("select count(name_autocrane) from vedomost v,staf s where s.id_s = v.id_stuf and s.last_name like ?");
            qry4.addBindValue(delit);
            qry4.exec();
            qry4.first();
            QString NameResume = "0";
            NameResume = qry4.value(0).toString();
            if (NameResume=="0")
            {
            qry3.prepare("DELETE FROM staf WHERE last_name=?");
            qry3.addBindValue(delit);
            qry3.exec();
            QMessageBox::information(this, "Успешно","Сотрудник был удален из реестра");
            MainWindow::on_Add_triggered();
            }
            else
            {
            QMessageBox::warning(this, "Внимание","Вы пытаетесь удалить сотрудника, который назначен на кран \nЧтобы удалить его - переназначте ответственного");
            }
        }
        ///При принятом значении "accepted" мы производим вставку данных в БД во вторую таблицу
        else{
         if (tex == "accepted")
         {
         qry2.prepare("insert into staf (last_name, first_name, email, phone) values (?, ?, ?, ?)");
         qry2.addBindValue(data1);
         qry2.addBindValue(data2);
         qry2.addBindValue(data3);
         qry2.addBindValue(data4);
         qry2.exec();
         ///Информационные сообщения об успешности добавления сотрудника в таблицу
         QMessageBox::information(this, "Успешно","Новый сотрудник был занесен в реестр");
         MainWindow::on_Add_triggered();
         }
         ///Если мы в другой форме не работаем с 2 таблицей, то мы собираемся доавлять знаечния в 1 таблицу
         else{
         QSqlQuery qry1;
         QSqlQuery query;
         query.prepare("select id_s from staf where last_name = ?");
         query.addBindValue(strin5);
         query.exec();
         query.first();
         QString NameResume = query.value(0).toString();
         qDebug()<<"Name"<<NameResume;

        if (NameResume!=""){
         qry1.prepare("insert into vedomost (name_autocrane, max_height, max_weight, object, id_stuf, arenda) values (?, ?, ?, ?,?, ?)");
         qry1.addBindValue(strin1);
         qry1.addBindValue(strin2);
         qry1.addBindValue(strin3);
         qry1.addBindValue(strin4);
         qry1.addBindValue(NameResume);
         qry1.addBindValue(strin6);
         qry1.exec();
         MainWindow::on_Disconnection_triggered();
         MainWindow::on_Connection_triggered();
        }
        else
        {
            QMessageBox::warning(this, "Внимание","Введеная вами фамилия сотрудника отсутствует в списке, запись не будет внесена в реестр");
        }
         }
}}
}

///Функция удаления данных из 1 таблицы
void MainWindow::on_Delete_triggered()
{
    QSqlQuery qry1;
    if(ui->tableView->selectionModel()->selectedIndexes().isEmpty()) return;
    QString st = model->data(model->index(ui->tableView->currentIndex().row(), 0)).toString();
    QString st2 = model->data(model->index(ui->tableView->currentIndex().row(), 1)).toString();
    QString st3 = model->data(model->index(ui->tableView->currentIndex().row(), 4)).toString();
//    qry1.exec("delete * from `vedomost` where `id`="+st+"and `name_autocrane`="+st2+"and `object`="+st3);
    qry1.prepare("DELETE FROM vedomost WHERE id=? AND name_autocrane=? and object=?");
    qry1.addBindValue(st);
    qry1.addBindValue(st2);
    qry1.addBindValue(st3);
    qry1.exec();
    MainWindow::on_Disconnection_triggered();
    MainWindow::on_Connection_triggered();
}
///Функция для редактирования данных 1 таблица(в ней у нас открывается форма добавления, но с установленными значениями)
void MainWindow::on_Edit_triggered()
{
    QItemSelectionModel *selectModel = ui->tableView->selectionModel();
    ///Сбор данных для передачи в окно редактирования
    if(!selectModel->selectedIndexes().isEmpty()){
    QString stolb1 = model->data(model->index(ui->tableView->currentIndex().row(), 1)).toString();
    QString stolb2 = model->data(model->index(ui->tableView->currentIndex().row(), 2)).toString();
    QString stolb3 = model->data(model->index(ui->tableView->currentIndex().row(), 3)).toString();
    QString stolb4 = model->data(model->index(ui->tableView->currentIndex().row(), 4)).toString();
    QString stolb5 = model->data(model->index(ui->tableView->currentIndex().row(), 5)).toString();
    QString stolb6 = model->data(model->index(ui->tableView->currentIndex().row(), 6)).toString();

    Dialog dialog(this,db,stolb1,stolb2,stolb3,stolb4,stolb5,stolb6,false);

    if (dialog.exec()) /// показываем диалог, здесь стоим, пока диалог не закроют
    {

         QString strin1 = dialog.strin1;
         QString strin2 = dialog.strin2;
         QString strin3 = dialog.strin3;
         QString strin4 = dialog.strin4;
         QString strin5 = dialog.strin5;
         QString strin6 = dialog.strin6;
         QSqlQuery query;
         query.prepare("select id_s from staf where last_name = ?");
         query.addBindValue(strin5);
         query.exec();
         query.first();
         QString NameResume = query.value(0).toString();
         qDebug()<<"Name"<<NameResume;
        ///Проверка вводимого значения
        if (NameResume!=""){
         QSqlQuery qry2;
         QString str = model->data(model->index(ui->tableView->currentIndex().row(), 0)).toString();
         QString str2 = model->data(model->index(ui->tableView->currentIndex().row(), 1)).toString();
         QString str3 = model->data(model->index(ui->tableView->currentIndex().row(), 4)).toString();
         qry2.prepare("update vedomost set name_autocrane = ?, max_height = ?, max_weight = ?, object = ?, id_stuf = ?, arenda =? where id=? AND name_autocrane=? and object=?");
         qry2.addBindValue(strin1);
         qry2.addBindValue(strin2);
         qry2.addBindValue(strin3);
         qry2.addBindValue(strin4);
         qry2.addBindValue(NameResume);
         qry2.addBindValue(strin6);
         qry2.addBindValue(str);
         qry2.addBindValue(str2);
         qry2.addBindValue(str3);
         qry2.exec();
         MainWindow::on_Disconnection_triggered();
         MainWindow::on_Connection_triggered();
        }
        else
        {
            QMessageBox::warning(this, "Внимание","Введеная вами фамилия сотрудника отсутствует в списке, запись не будет внесена в реестр");
        }
    }
    else
    {
        //код, если диалог закрыт reject()
    }
    }
}




///Функция поиска  по нажатию кнопки
void MainWindow::on_pushButton_clicked()
{
//    QString sortcolomn = ui->comboBox->currentText();
    QStringList myOptions;
    QString kolonka;
    QString find;
    QString text = ui->lineEdit->text();
    int intere = text.toInt();
    int znak;
    QString opred = "";
    if( intere != 0)
    {
    znak = text.toInt();
    opred = "int";
    soderzint = znak;
    soderz ="";
    }
    else
    {
    opred = "text";
    find = find.append(text);
    find = find.append("%");
    soderz =find;
    soderzint = 0;
    }
//    myOptions << "Название крана" <<"Адрес объекта"<<"Фамилия ответственного"<<"Состояние крана";

//    switch(myOptions.indexOf(sortcolomn)){
//      case 0:
//        colomnnum = 0;
//        kolonka = "v.name_autocrane";
//        break;

//      case 1:
//        colomnnum = 1;
//        kolonka = "v.object";
//        break;

//      case 2:
//        colomnnum = 2;
//        kolonka = "s.id_stuf";
//        break;
//    case 3:
//      colomnnum = 3;
//      kolonka = "v.arenda";
//      break;


//      default:
//        break;
//    }

    if (text == "" ){
        on_Disconnection_triggered();
        on_Connection_triggered();
        typeind = "text";
        soderz = "";
    }
    else
    {

        qry1 = new QSqlQuery(db);
        QSqlQuery qry3;

        if (opred == "text")
        {
        qry1->prepare("select v.id, v.name_autocrane,v.max_height,v.max_weight,v.object,s.last_name,v.arenda from vedomost v, staf s where v.id_stuf = id_s and (s.last_name like ? or v.name_autocrane like ? or v.object like ?) order by ? ASC");
        qry1->addBindValue(find);
        qry1->addBindValue(find);
        qry1->addBindValue(find);
        qry1->addBindValue("v.name_autocrane");
        qry1->exec();
        qry3.prepare("select Count(*) from vedomost v, staf s where v.id_stuf = id_s and (s.last_name like ? or v.name_autocrane like ? or v.object like ?)");
        qry3.addBindValue(find);
        qry3.addBindValue(find);
        qry3.addBindValue(find);
        qry3.exec();
        qry3.first();
        typeind = "text";
        QString NameResume = qry3.value(0).toString();
        }
        else
        {
            qry1->prepare("select v.id, v.name_autocrane,v.max_height,v.max_weight,v.object,s.last_name,v.arenda from vedomost v, staf s where v.id_stuf = id_s and (v.id >= ? or v.max_height >= ? or v.max_weight >= ?) order by v.name_autocrane ASC");
            qry1->addBindValue(znak);
            qry1->addBindValue(znak);
            qry1->addBindValue(znak);
            qry1->exec();
            qry3.prepare("select Count(*) from vedomost v, staf s where v.id_stuf = id_s and (v.id = ? or v.max_height >= ? or v.max_weight >= ?)");
            qry3.addBindValue(znak);
            qry3.addBindValue(znak);
            qry3.addBindValue(znak);
            qry3.exec();
            qry3.first();
            typeind = "int";
            QString NameResume = qry3.value(0).toString();
        }


        QString NameResume = qry3.value(0).toString();
        qDebug()<<"Name"<<NameResume;
        if (NameResume!="0")
        {
        model->setQuery(*qry1);
//        ui->tableView->setModel(model);
///Задание сортировки по столбцам в прокси моделе
        proxyModel->setSourceModel(model);
            ui->tableView->setModel(proxyModel);
            ui->tableView->setSortingEnabled(true);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        model->setHeaderData(0,Qt::Horizontal,tr("Номер запроса"));
        model->setHeaderData(1,Qt::Horizontal,tr("Название крана"));
        model->setHeaderData(2,Qt::Horizontal,tr("Максимальная высота подъема"));
        model->setHeaderData(3,Qt::Horizontal,tr("Максимальная грузоподъемность"));
        model->setHeaderData(4,Qt::Horizontal,tr("На каком объекте находится"));
        model->setHeaderData(5,Qt::Horizontal,tr("Фамилия ответственного лица"));
        model->setHeaderData(6,Qt::Horizontal,tr("Находится ли кран в аренде"));
        ui->lineEdit->setText("");
        }
        else{
            QMessageBox::information(this, "Поиск","По вашему запросу ничего не найдено");
        }
    }
    ui->pushButton_2->setEnabled(true);
}








///Функция сортировки по нажатию кнопки
void MainWindow::on_pushButton_2_clicked()
{

    qry1 = new QSqlQuery(db);
    QSqlQuery qry3;
    QString idi ="v.name_autocrane";
    int index = ui->comboBox->currentIndex();
    if(index == 0)
    {
        idi = "v.id";
    }
    if(index == 1)
    {
        idi = "v.name_autocrane";
    }
    if(index == 2)
    {
        idi = "v.max_height";
    }
    if(index == 3)
    {
        idi = "v.max_weight";
    }
    if(index == 4)
    {
        idi = "v.object";
    }
    if(index == 5)
    {
        idi = "s.last_name";
    }
    if(index == 6)
    {
        idi = "v.arenda";
    }

    if (typeind == "int" and soderz!="")
    {
    QString domoi3 ="";
    domoi3.append("select v.id, v.name_autocrane,v.max_height,v.max_weight,v.object,s.last_name,v.arenda from vedomost v, staf s where v.id_stuf = id_s and (v.id >=");
    domoi3.append(soderzint);
    domoi3.append(" or v.max_height >= ");
    domoi3.append(soderzint);
    domoi3.append(" or v.max_weight >= ");
    domoi3.append(soderzint);
    domoi3.append(") order by ");
    domoi3.append(idi+" ASC");
    qry1->prepare(domoi3);
    qry1->exec();

    }
    if(typeind == "text" and soderz!="")
    {
//        QString domoi2 = "select v.id, v.name_autocrane,v.max_height,v.max_weight,v.object,s.last_name,v.arenda from vedomost v, staf s where v.id_stuf = id_s and (s.last_name like "+soderz+" or v.name_autocrane like "+soderz+" or v.object like "+soderz+") order by "+idi+" ASC";
//        qry4->prepare(domoi2);
        qry1->prepare("select v.id, v.name_autocrane,v.max_height,v.max_weight,v.object,s.last_name,v.arenda from vedomost v, staf s where v.id_stuf = id_s and (s.last_name like ? or v.name_autocrane like ? or v.object like ?) order by ? ASC");
        qry1->addBindValue(soderz);
        printf("AAAAAAAAAAAAA");
        qry1->addBindValue(soderz);
        qry1->addBindValue(soderz);
        qry1->addBindValue(idi);
        qry1->exec();

    }
    if (soderz=="")
    {
        QString domoi = "select v.id, v.name_autocrane,v.max_height,v.max_weight,v.object,s.last_name,v.arenda from vedomost v, staf s where v.id_stuf = id_s order by " + idi +" ASC";
        qry1->prepare(domoi);
//        qry1->addBindValue(idi);
        qry1->exec();

    }
    model->setQuery(*qry1);
//    ui->tableView->setModel(model);
    proxyModel->setSourceModel(model);
        ui->tableView->setModel(proxyModel);
        ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model->setHeaderData(0,Qt::Horizontal,tr("Номер запроса"));
    model->setHeaderData(1,Qt::Horizontal,tr("Название крана"));
    model->setHeaderData(2,Qt::Horizontal,tr("Максимальная высота подъема"));
    model->setHeaderData(3,Qt::Horizontal,tr("Максимальная грузоподъемность"));
    model->setHeaderData(4,Qt::Horizontal,tr("На каком объекте находится"));
    model->setHeaderData(5,Qt::Horizontal,tr("Фамилия ответственного лица"));
    model->setHeaderData(6,Qt::Horizontal,tr("Находится ли кран в аренде"));
    ui->pushButton_2->setEnabled(false);
}
///Функция определения смены выбранного столбца для сортировки, она активирует кнопку
void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->pushButton_2->setEnabled(true);
}
///Функция вывода информации об авторе
void MainWindow::on_action_triggered()
{
    Info info;
    info.exec();
}
///Функция проверки смены языка и его перевода
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
            ui->retranslateUi(this);    // переведём окно заново
        }
}


///Изменение языка по нажатию на клавишу с Русским языком
void MainWindow::on_action_2_triggered()
{
        qApp->removeTranslator(&qtLanguageTranslator);
        qtLanguageTranslator.load(":/kurs_ru_RU");
        qApp->installTranslator(&qtLanguageTranslator);
}

///Изменение языка по нажатию на клавишу с Английским языком
void MainWindow::on_action_3_triggered()
{

    qApp->removeTranslator(&qtLanguageTranslator);
        qtLanguageTranslator.load(":/kurs_en_US");
        qApp->installTranslator(&qtLanguageTranslator);
}

///Изменение языка по нажатию на клавишу с Итальянским языком
void MainWindow::on_action_4_triggered()
{
    qApp->removeTranslator(&qtLanguageTranslator);
        qtLanguageTranslator.load(":/kurs_it_IT");
        qApp->installTranslator(&qtLanguageTranslator);
}
