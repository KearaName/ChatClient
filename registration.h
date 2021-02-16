#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QUdpSocket>
#include <QLabel>
#include <QLineEdit>
#include <QtGui>
#include <QtNetwork>
#include <QGridLayout>
#include <QtSql>
#include <QMessageBox>

class Registration : public QWidget
{
    Q_OBJECT
public:
    explicit Registration( QSqlDatabase DataBase, QWidget *parent = nullptr);
    ~Registration();

signals:
    void RegistrationDone();

private slots:
    void on_pushButtonRegistrationUser_clicked();

public:

    QSqlDatabase db;

    QLabel *labelLogin;
    QLabel *labelPassword;
    QLineEdit *lineEditLogin;
    QLineEdit *lineEditPassword;
    QPushButton *pushButtonRegistrationUser;
    QPushButton *pushButtonBack;

};

#endif // REGISTRATION_H
