#ifndef ENTERROOM_H
#define ENTERROOM_H

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

class EnterRoom : public QWidget
{
    Q_OBJECT
public:
    explicit EnterRoom(QSqlDatabase DataBase, QString Login, QWidget *parent = nullptr);
    ~EnterRoom();

signals:

    void RoomEnterDone();

private slots:
      void on_pushButtonEnterRoom_clicked();

public:

    QSqlDatabase db;
    QString YourLogin;

    QLabel *labelRoomName;
    QLabel *labelRoomPassword;
    QLineEdit *lineEditRoomName;
    QLineEdit *lineEditRoomPassword;
    QPushButton *pushButtonEnterRoom;
    QPushButton *pushButtonBack;

};

#endif // ENTERROOM_H
