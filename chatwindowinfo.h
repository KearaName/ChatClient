#ifndef CHATWINDOWINFO_H
#define CHATWINDOWINFO_H

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

class ChatWindowInfo : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindowInfo(QSqlDatabase DataBase, QString Login, QWidget *parent = nullptr);
    ~ChatWindowInfo();
signals:
    void RoomIsSelect();

private slots:
    void usersToRoomView();

    void roomselect();

public:

    QSqlDatabase db;
    QString YourLogin;
    QString SelectedRoom;

    QPushButton *pushButtonCreateRoom;
    QPushButton *pushButtonEnterRoom;

    QLabel *labelUsesrOnline;
    QLabel *labelYourRoom;

    QListView *listViewRoom;
    QListView *listViewUsersOnline;

    QStringListModel * listModelRoom;
    QStringListModel * listModelUsersOnline;

};

#endif // CHATWINDOWINFO_H
