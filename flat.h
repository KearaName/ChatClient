#ifndef FLAT_H
#define FLAT_H

#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QUdpSocket>
#include <QLabel>
#include <QLineEdit>
#include <QtGui>
#include <QtNetwork>
#include <QGridLayout>


class Flat : public QWidget
{
    Q_OBJECT
public:
    explicit Flat(QWidget *parent = nullptr);
    ~Flat();
public slots:
    void readPendingDatagrams();

    QString localIP();

private slots:
    void on_pushButtonSendMessage_clicked();

public:
    QUdpSocket * udpSocket;
    QHostAddress IPAddress;
    quint16 port;

    QLabel *labelYourIP;
    QLabel *labelSendMessage;
    QLineEdit *lineEditSendMessage;
    QPushButton *pushButtonSendMessage;
    QPlainTextEdit *plainTextEditChat;

};

#endif // FLAT_H
