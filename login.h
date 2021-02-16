#ifndef LOGIN_H
#define LOGIN_H

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

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login( QSqlDatabase DataBase,  QWidget *parent = nullptr);
    ~Login();

signals:
    void LoginDone();

private slots:
     void on_pushButtonLogin_clicked();

public:
    QSqlDatabase db;
    QString YourLogin;

    QLabel *labelLogin;
    QLabel *labelPassword;
    QLineEdit *lineEditLogin;
    QLineEdit *lineEditPassword;
    QPushButton *pushButtonLogin;
    QPushButton *pushButtonRegistration;
};

#endif // LOGIN_H
