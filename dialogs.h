#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class Dialogs;
}

class Dialogs : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogs(QWidget *parent = nullptr);
    ~Dialogs();
    void check_ok();
    QString data1;
    QString data2;
    QString data3;
    QString data4;



private slots:
    void on_FIO_textChanged(const QString &arg1);

    void on_IMA_textChanged(const QString &arg1);

    void on_EMAIL_textChanged(const QString &arg1);

    void on_Phone_textChanged(const QString &arg1);

    void on_ok_clicked();


    void on_EMAIL_editingFinished();

private:
    Ui::Dialogs *ui;

};

#endif // DIALOGS_H
