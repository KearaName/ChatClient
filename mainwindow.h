#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QNetworkInterface>
#include "flat.h"
#include "login.h"
#include "registration.h"
#include "chatwindow.h"
#include "chatwindowinfo.h"
#include "createroom.h"
#include "enterroom.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void RoomSelectedDone();

private slots:

    void RegistrationAfterLoginUser();

    void LoginUserAfterRegistration();

    void ChatWindowAfterLogin();

    void LoginUserAfterChatWindowInfo();

    void CreateRoomAfterChatWindowInfo();

    void LoginUserAfterCreateRoom();

    void ChatWindowInfoAfterCreatRoom();

    void EnterRoomAfterChatWindowInfo();

    void ChatWindowInfoAfterEnterRoom();

    void RoomName();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    Registration * layoutRegistration;
    Login * layoutLogin;
    ChatWindow * layoutChatWindow;
    ChatWindowInfo * layoutChatWindowInfo;
    CreateRoom * layoutCreateRoom;
    EnterRoom * layoutEnterRoom;

};
#endif // MAINWINDOW_H
