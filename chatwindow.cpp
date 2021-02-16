#include "chatwindow.h"

ChatWindow::ChatWindow(QSqlDatabase DataBase, QString Login, QWidget *parent) : QWidget(parent)
{
    db = DataBase;
    YourLogin=Login;

    udpSocket = new QUdpSocket(this);
    port=0;

    udpSocket->bind(QHostAddress::Any, port);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    pushButtonSendMessage = new QPushButton;
    pushButtonSendMessage->setText("Отправить");
    pushButtonSendMessage->setEnabled(false);

    pushButtonExit = new QPushButton;
    pushButtonExit->setText("Выход");

    labelRoomStatus = new QLabel(tr("Комната: "));

    plainTextEditChat = new QPlainTextEdit;
    plainTextEditChat->setReadOnly(true);

    lineEditSendMessage = new QLineEdit;
    lineEditSendMessage->setEnabled(false);

    groupBoxChatWindow = new QGroupBox(tr("Чат"));

    QHBoxLayout *labelHBoxLayout1 = new QHBoxLayout;
    labelHBoxLayout1->addWidget(labelRoomStatus);
    labelHBoxLayout1->addWidget(pushButtonExit);

    QHBoxLayout *labelHBoxLayout2 = new QHBoxLayout;
    labelHBoxLayout2->addWidget(lineEditSendMessage);
    labelHBoxLayout2->addWidget(pushButtonSendMessage);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->addLayout(labelHBoxLayout1);
    boxLayout->addWidget(plainTextEditChat);
    boxLayout->addLayout(labelHBoxLayout2);
    groupBoxChatWindow->setLayout(boxLayout);

    QVBoxLayout *mainBoxLayout = new QVBoxLayout;

    mainBoxLayout->addWidget(groupBoxChatWindow);

    setLayout(mainBoxLayout);

    connect(pushButtonExit, SIGNAL(clicked()), this, SLOT(on_pushButtonExit_clicked()));
    connect(pushButtonSendMessage, SIGNAL(clicked()), this, SLOT(on_pushButtonSendMessage_clicked()));
    connect(lineEditSendMessage, SIGNAL(returnPressed()), this, SLOT(on_pushButtonSendMessage_clicked()));
}

ChatWindow::~ChatWindow()
{

    delete pushButtonSendMessage;
    delete pushButtonExit;
    delete labelRoomStatus;
    delete plainTextEditChat;
    delete lineEditSendMessage;
    delete groupBoxChatWindow;

}

void ChatWindow::readPendingDatagrams()
 {
     while (udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(udpSocket->pendingDatagramSize());
         QHostAddress sender;
         quint16 senderPort;

         udpSocket->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);

         if(QString(datagram).left(9)=="[refresh]") {
             emit refresh();
         }
         else
         if(QString(datagram).left(9)=="[message]")
         {
             plainTextEditChat->appendPlainText(QString(datagram).mid(9));
         }
    }

}

void ChatWindow::SetRoomSelected()
{
    if(db.open()) {
        plainTextEditChat->setPlainText(NULL);
        QString Message = "[exit]";
        udpSocket->writeDatagram(Message.toLatin1(),  QHostAddress("192.168.43.89"), 7755);

        Message = "[room]"+SelectedRoom;
        udpSocket->writeDatagram(Message.toLatin1(),  QHostAddress("192.168.43.89"), 7755);

        labelRoomStatus->setText(tr("Комната: ") + SelectedRoom);
        pushButtonSendMessage->setEnabled(true);
        lineEditSendMessage->setEnabled(true);


        QSqlQuery * querySQLroom = new QSqlQuery();
        querySQLroom->prepare("SELECT ID_room FROM tableRooms WHERE Room_name=:Room_name");
        querySQLroom->bindValue(":Room_name", SelectedRoom);
        querySQLroom->exec();
        querySQLroom->next();
        QString ID_room = querySQLroom->value(0).toString();
        delete querySQLroom;

        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("SELECT Message_datetime, ID_sender, Message_text FROM tableMessages "
                          "WHERE ID_room = :ID_room "
                          "ORDER BY Message_datetime ");
        querySQL->bindValue(":ID_room", ID_room);
        if(querySQL->exec()) {

            while(querySQL->next())
            {
                QSqlQuery * querySQLsender = new QSqlQuery();
                querySQLsender->prepare("SELECT User_login FROM tableUsers WHERE ID_user=:ID_user");
                querySQLsender->bindValue(":ID_user", querySQL->value(1).toString());
                querySQLsender->exec();
                querySQLsender->next();
                QString User_login = querySQLsender->value(0).toString();
                delete querySQLsender;

                plainTextEditChat->appendPlainText(querySQL->value(0).toDateTime().toString("[dd.MM.yyyy HH:mm:ss]")+" "+User_login+": " + querySQL->value(2).toString());
            }

            db.close();
            delete querySQL;

                    //emit RoomEnterDone();
         }
         else {
            QMessageBox::critical(this, tr("Ошибка!"),
                                  querySQL->lastError().text());
         }
    }
    else {
        QMessageBox::critical(this, tr("Ошибка!"),
                              tr("Соединение с сервером разорвано"));
    }
}

void ChatWindow::on_pushButtonSendMessage_clicked()
{

    if(db.open()) {

        QDateTime Date = QDateTime::currentDateTime();
        QString Message = "[message]" + Date.toString("[dd.MM.yyyy HH:mm:ss] ")+ YourLogin +": " + lineEditSendMessage->text();
        udpSocket->writeDatagram(Message.toLatin1(),  QHostAddress("192.168.43.89"), 7755);

        QSqlQuery * querySQLsender = new QSqlQuery();
        querySQLsender->prepare("SELECT ID_user FROM tableUsers WHERE User_login=:User_login");
        querySQLsender->bindValue(":User_login", YourLogin);
        querySQLsender->exec();
        querySQLsender->next();
        QString ID_sender = querySQLsender->value(0).toString();
        delete querySQLsender;

        QSqlQuery * querySQLroom = new QSqlQuery();
        querySQLroom->prepare("SELECT ID_room FROM tableRooms WHERE Room_name=:Room_name");
        querySQLroom->bindValue(":Room_name", SelectedRoom);
        querySQLroom->exec();
        querySQLroom->next();
        QString  ID_room = querySQLroom->value(0).toString();
        delete querySQLroom;

        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("INSERT INTO tableMessages(Message_text, Message_datetime, ID_sender, ID_room) "
                   "VALUES (:Message_text, :Message_datetime, :ID_sender, :ID_room)");

        querySQL->bindValue(":Message_text", lineEditSendMessage->text() == NULL ? NULL : lineEditSendMessage->text());
        querySQL->bindValue(":Message_datetime", Date);
        querySQL->bindValue(":ID_sender", ID_sender);
        querySQL->bindValue(":ID_room", ID_room);


        if(querySQL->exec()) {
            delete querySQL;
            db.close();
            lineEditSendMessage->setText(NULL);
        }
        else {
            QMessageBox::critical(this, tr("Ошибка!"),
                                     tr("Вы пытаетесь отправить пустое сообщение\n")+querySQL->lastError().text());
        }
    }
    else {
         QMessageBox::critical(this, tr("Ошибка!"),
                                  tr("Соединение потеряно\n") + db.lastError().text());
    }
}

void ChatWindow::on_pushButtonExit_clicked()
{
    if(db.open()) {
        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("UPDATE tableUsers SET User_status = :User_status WHERE User_login = :User_login");
        querySQL->bindValue(":User_status", "offline");
        querySQL->bindValue(":User_login", YourLogin);

        if(querySQL->exec()) {

            delete querySQL;
            db.close();
            QString Message = "[exit]";
            udpSocket->writeDatagram(Message.toLatin1(),  QHostAddress("192.168.43.89"), 7755);

            emit ExitFromChat();
        }
        else {
            QMessageBox::critical(this, tr("Ошибка!"),
                                     querySQL->lastError().text());
            emit ExitFromChat();
        }
    }
    else {
        QMessageBox::critical(this, tr("Ошибка!"),
                                 tr("Соединение с сервером разорвано"));
        emit ExitFromChat();
    }
}
