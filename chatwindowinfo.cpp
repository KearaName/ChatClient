#include "chatwindowinfo.h"

ChatWindowInfo::ChatWindowInfo(QSqlDatabase DataBase, QString Login, QWidget *parent) : QWidget(parent)
{
    db = DataBase;
    YourLogin = Login;

    pushButtonCreateRoom = new QPushButton;
    pushButtonCreateRoom->setText("Создать комнату");

    pushButtonEnterRoom = new QPushButton;
    pushButtonEnterRoom->setText("Войти в комнату");

    labelYourRoom = new QLabel(tr("Комнаты, в которых\nвы состоите"));
    labelUsesrOnline = new QLabel(tr("Пользователи в комнате"));

    listViewRoom = new QListView;
    listViewRoom->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(db.open()) {
        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("DECLARE @ID_user INT "
                          "SET @ID_user = (SELECT ID_user FROM tableUsers WHERE User_login = :User_login) "
                          "SELECT Room_name FROM tableRooms A "
                          "INNER JOIN tableUserToRoom B ON ID_user = @ID_user AND A.ID_room = B.ID_room "
                          "ORDER BY B.ID_room");
        querySQL->bindValue(":User_login", YourLogin);
        if(querySQL->exec()) {

            QStringList listRoom;
            while(querySQL->next())
                listRoom.append(querySQL->value(0).toString());

            listModelRoom = new QStringListModel;
            listModelRoom->setStringList(listRoom);
            listViewRoom->setModel(listModelRoom);

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


    listViewUsersOnline = new QListView;
    listViewUsersOnline->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listViewUsersOnline->setSelectionMode(QAbstractItemView::NoSelection);
   // listViewUsersOnline->setEnabled(false);

    listModelUsersOnline = new QStringListModel;

    QVBoxLayout * vBoxLayout1 = new QVBoxLayout;

    vBoxLayout1->addWidget(pushButtonCreateRoom);
    vBoxLayout1->addWidget(pushButtonEnterRoom);
    vBoxLayout1->addWidget(labelYourRoom);
    vBoxLayout1->addWidget(listViewRoom);
    vBoxLayout1->addWidget(labelUsesrOnline);
    vBoxLayout1->addWidget(listViewUsersOnline);

    setLayout(vBoxLayout1);
    connect(listViewRoom, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(roomselect()));
    //connect(pushButtonCreateRoom, SIGNAL(clicked()), this, SLOT(on_pushButtonCreateRoom_clicked()));
}

ChatWindowInfo::~ChatWindowInfo()
{
    delete labelYourRoom;
    delete labelUsesrOnline;
    delete listViewRoom;
    delete listModelRoom;
    delete listViewUsersOnline;
    delete pushButtonEnterRoom;
    delete pushButtonCreateRoom;
    delete listModelUsersOnline;
}


void ChatWindowInfo::roomselect()
{
    QModelIndex index = listViewRoom->currentIndex();
    SelectedRoom = index.data(Qt::DisplayRole).toString();
    usersToRoomView();
    emit RoomIsSelect();
}

void ChatWindowInfo::usersToRoomView()
{

    if(db.open()) {

        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("DECLARE @ID_room INT "
                          "SET @ID_room = (SELECT ID_room FROM tableRooms WHERE Room_name = :Room_name) "
                          "SELECT User_login+' ('+User_status+')' FROM tableUsers A "
                          "INNER JOIN tableUserToRoom B ON ID_room = @ID_room AND A.ID_user=B.ID_user "
                          "ORDER BY User_login");
        querySQL->bindValue(":Room_name", SelectedRoom);
        if(querySQL->exec()) {

            QStringList listUsersOnline;
            while(querySQL->next())
                listUsersOnline.append(querySQL->value(0).toString());
            listModelUsersOnline = new QStringListModel;
            listModelUsersOnline->setStringList(listUsersOnline);
            listViewUsersOnline->setModel(listModelUsersOnline);

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
  //  connect(listViewRoom, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(usersToRoomView()));
}
