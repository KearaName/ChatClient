#ifndef CHATWINDOW_H
#define CHATWINDOW_H

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
#include <QDateTime>


class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(QSqlDatabase DataBase, QString Login, QWidget *parent = nullptr);
    ~ChatWindow();
signals:

    void ExitFromChat();

    void refresh();

public slots:
    void SetRoomSelected();

    void readPendingDatagrams();

private slots:
    void on_pushButtonExit_clicked();

    void on_pushButtonSendMessage_clicked();


public:

    QSqlDatabase db;
    QString YourLogin;
    QString SelectedRoom;

    QUdpSocket * udpSocket;
    quint16 port;

    QPushButton *pushButtonSendMessage;
    QPushButton *pushButtonExit;

    QLabel *labelRoomStatus;

    QGroupBox *groupBoxChatWindow;
    QLineEdit *lineEditSendMessage;
    QPlainTextEdit *plainTextEditChat;
    QList<QString> *listRoom;

};

#endif // CHATWINDOW_H
