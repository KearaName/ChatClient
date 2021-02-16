#include "enterroom.h"

EnterRoom::EnterRoom( QSqlDatabase DataBase, QString Login, QWidget *parent) : QWidget(parent)
{
    db = DataBase;
    YourLogin = Login;

    labelRoomName = new QLabel(tr("Название комнаты"));
    labelRoomPassword = new QLabel(tr("Пароль комнаты"));

    lineEditRoomName = new QLineEdit;
    lineEditRoomPassword = new QLineEdit;

    lineEditRoomPassword -> setEchoMode(QLineEdit::Password);

    pushButtonEnterRoom = new QPushButton;
    pushButtonBack = new QPushButton;

    pushButtonEnterRoom->setText("Войти в комнату");
    pushButtonBack->setText("Назад");

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(labelRoomName,1,0,1,-1);
    gridLayout->addWidget(lineEditRoomName,2,0,1,-1);
    gridLayout->addWidget(labelRoomPassword,3,0,1,-1);
    gridLayout->addWidget(lineEditRoomPassword,4,0,1,-1);
    gridLayout->addWidget(pushButtonEnterRoom,5,0);
    gridLayout->addWidget(pushButtonBack, 5,1);
    setLayout(gridLayout);

    connect(pushButtonEnterRoom, SIGNAL(clicked()), this, SLOT(on_pushButtonEnterRoom_clicked()));
}

EnterRoom::~EnterRoom()
{
    delete labelRoomName;
    delete labelRoomPassword;
    delete lineEditRoomName;
    delete lineEditRoomPassword;
    delete pushButtonEnterRoom;
    delete pushButtonBack;
}

void EnterRoom::on_pushButtonEnterRoom_clicked()
{
    if(db.open()) {
        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("DECLARE @ID_room INT, @ID_user INT "
                          "SET @ID_room = (SELECT ID_room FROM tableRooms WHERE Room_name = :Room_name AND Room_password = :Room_password)"
                          "SET @ID_user = (SELECT ID_user FROM tableUsers WHERE User_login = :User_login)"
                          "INSERT INTO tableUserToRoom(ID_room, ID_user)"
                          "VALUES (@ID_room, @ID_user)");

        querySQL->bindValue(":Room_name", lineEditRoomName->text());
        querySQL->bindValue(":Room_password", lineEditRoomPassword->text());
        querySQL->bindValue(":User_login", YourLogin);

        if(querySQL->exec()) {
                db.close();
                delete querySQL;
                emit RoomEnterDone();
        }
        else {
            QMessageBox::critical(this, tr("Ошибка!"),
                                     tr("Неверно имя комнаты или пароль!"
                                        "Либо вы уже состоите в ней"));
        }
    }
    else {
        QMessageBox::critical(this, tr("Ошибка!"),
                                 tr("Соединение с сервером разорвано"));
    }
}
