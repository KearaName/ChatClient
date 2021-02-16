#include "flat.h"

#define yourIPAddress QHostAddress::Any
#define yourport 7755

Flat::Flat(QWidget *parent) : QWidget(parent)
{

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress(yourIPAddress), yourport);

    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));

    labelYourIP = new QLabel(tr("Ваш IP: ")+ localIP());
    labelSendMessage = new QLabel(tr("Отправить сообщение"));

    lineEditSendMessage = new QLineEdit;

    plainTextEditChat = new QPlainTextEdit;
    plainTextEditChat->setReadOnly(true);

    pushButtonSendMessage = new QPushButton;
    pushButtonSendMessage->setText("Отправить");

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(labelYourIP, 0,0,1,-1);
    gridLayout->addWidget(plainTextEditChat,1,0,1,-1);
    gridLayout->addWidget(labelSendMessage,2,0,1,-1);
    gridLayout->addWidget(lineEditSendMessage, 3,0,1,1);
    gridLayout->addWidget(pushButtonSendMessage,3,1,1,1);
    setLayout(gridLayout);

    connect(pushButtonSendMessage, SIGNAL(clicked()), this, SLOT(on_pushButtonSendMessage_clicked()));
}

Flat::~Flat()
{

}

QString Flat::localIP()
{
  QString locIP;
  QList<QHostAddress> addr = QNetworkInterface::allAddresses();

  for(int nIter=0; nIter<addr.count(); nIter++)
  {
    if(!addr[nIter].isLoopback())
    if (addr[nIter].protocol() == QAbstractSocket::IPv4Protocol )
    if (addr[nIter].toString().left(11)=="192.168.43.")
        locIP = addr[nIter].toString();
  }
  return locIP;
}

void Flat::readPendingDatagrams()
 {
     while (udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;
         datagram.resize(udpSocket->pendingDatagramSize());
         QHostAddress sender;
         quint16 senderPort;

         udpSocket->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);

         plainTextEditChat->appendPlainText("( ip -> " + sender.toString() + "; port -> " +
                                            QString("%1").arg(senderPort)+ ";) " + QString(datagram));
     }
 }

void Flat::on_pushButtonSendMessage_clicked()
{
    plainTextEditChat->appendPlainText("( ip -> " + localIP() + "; port -> " +
                                     QString("%1").arg(port) + ";) " + lineEditSendMessage->text());
    udpSocket->writeDatagram(lineEditSendMessage->text().toLatin1(), IPAddress, port);
}
