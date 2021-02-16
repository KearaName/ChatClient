#include "createroom.h"


CreateRoom::CreateRoom(QSqlDatabase DataBase, QString Login, QWidget *parent) : QWidget(parent)
{
    db = DataBase;
    YourLogin = Login;

    labelRoomName = new QLabel(tr("Название комнаты"));
    labelRoomPassword = new QLabel(tr("Пароль комнаты"));

    lineEditRoomName = new QLineEdit;
    lineEditRoomPassword = new QLineEdit;

    lineEditRoomPassword -> setEchoMode(QLineEdit::Password);

    pushButtonCreateRoom = new QPushButton;
    pushButtonBack = new QPushButton;

    pushButtonCreateRoom->setText("Создать комнату");
    pushButtonBack->setText("Назад");


    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(labelRoomName,1,0,1,-1);
    gridLayout->addWidget(lineEditRoomName,2,0,1,-1);
    gridLayout->addWidget(labelRoomPassword,3,0,1,-1);
    gridLayout->addWidget(lineEditRoomPassword,4,0,1,-1);
    gridLayout->addWidget(pushButtonCreateRoom,5,0);
    gridLayout->addWidget(pushButtonBack, 5,1);
    setLayout(gridLayout);

    connect(pushButtonCreateRoom, SIGNAL(clicked()), this, SLOT(on_pushButtonCreateRoom_clicked()));
}

CreateRoom::~CreateRoom()
{
    delete labelRoomName;
    delete labelRoomPassword;
    delete lineEditRoomName;
    delete lineEditRoomPassword;
    delete pushButtonCreateRoom;
    delete pushButtonBack;
}

void CreateRoom::on_pushButtonCreateRoom_clicked()
{
    if(db.open()) {
        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("INSERT INTO tableRooms(Room_name, Room_password) "
                          "VALUES (:Room_name, :Room_password)");

        querySQL->bindValue(":Room_name", lineEditRoomName->text());
        querySQL->bindValue(":Room_password", lineEditRoomPassword->text());

        if(querySQL->exec()) {
            QSqlQuery * querySQLUsersToRoom = new QSqlQuery();
            querySQLUsersToRoom->prepare("DECLARE @ID_room INT, @ID_user INT "
                                         "SET @ID_room = (SELECT ID_room FROM tableRooms WHERE Room_name = :Room_name)"
                                         "SET @ID_user = (SELECT ID_user FROM tableUsers WHERE User_login = :User_login)"
                                         "INSERT INTO tableUserToRoom(ID_room, ID_user)"
                                         "VALUES (@ID_room, @ID_user)");

            querySQLUsersToRoom->bindValue(":Room_name", lineEditRoomName->text());
            querySQLUsersToRoom->bindValue(":User_login", YourLogin);

            if(querySQLUsersToRoom->exec()) {
                db.close();
                delete querySQL;
                delete querySQLUsersToRoom;
                emit RoomCreateDone();
            }
            else {
                QMessageBox::critical(this, tr("Ошибка!"),
                                         querySQL->lastError().text());
            }
        }
        else {
            QMessageBox::critical(this, tr("Ошибка!"),
                                     tr("Такая комната уже существует!"));
        }
    }
    else {
        QMessageBox::critical(this, tr("Ошибка!"),
                                 tr("Соединение с сервером разорвано"));
    }
}
