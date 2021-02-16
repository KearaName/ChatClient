#ifndef CREATEROOM_H
#define CREATEROOM_H

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
#include <QListView>
#include <QGroupBox>
#include <QMessageBox>

class CreateRoom : public QWidget
{
    Q_OBJECT
public:
    explicit CreateRoom( QSqlDatabase DataBase, QString Login, QWidget *parent = nullptr);
    ~CreateRoom();

signals:
    void RoomCreateDone();

private slots:
    void on_pushButtonCreateRoom_clicked();

public:

    QSqlDatabase db;
    QString YourLogin;

    QLabel *labelRoomName;
    QLabel *labelRoomPassword;
    QLineEdit *lineEditRoomName;
    QLineEdit *lineEditRoomPassword;
    QPushButton *pushButtonCreateRoom;
    QPushButton *pushButtonBack;

    //CreateRoom * layoutCreateRoom;
};

#endif // CREATEROOM_H
