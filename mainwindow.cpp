#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("DRIVER={ODBC Driver 17 for SQL Server};SERVER=192.168.43.89;Database=Chat;");
    db.setUserName("sa");
    db.setPassword("Krtu56ioutyi");

    layoutLogin = new Login(db);
    ui->Layout->addWidget(layoutLogin);

    this->resize(360,240);
    connect(layoutLogin, SIGNAL(LoginDone()), this, SLOT(ChatWindowAfterLogin()));
    connect(layoutLogin->pushButtonRegistration,SIGNAL(clicked()), this, SLOT(RegistrationAfterLoginUser()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ChatWindowAfterLogin()
{
    layoutChatWindow = new ChatWindow(db, layoutLogin->YourLogin);
    layoutChatWindowInfo = new ChatWindowInfo(db, layoutLogin->YourLogin);
    delete layoutLogin;
    ui->Layout->addWidget(layoutChatWindowInfo, 0,0,2,2);
    ui->Layout->addWidget(layoutChatWindow, 0,2,2,1);
    this->resize(720,480);

    connect(layoutChatWindowInfo, SIGNAL(RoomIsSelect()), this, SLOT(RoomName()));
    connect(this, SIGNAL(RoomSelectedDone()), layoutChatWindow, SLOT(SetRoomSelected()));

    connect(layoutChatWindowInfo->pushButtonCreateRoom, SIGNAL(clicked()), this, SLOT(CreateRoomAfterChatWindowInfo()));
    connect(layoutChatWindowInfo->pushButtonEnterRoom, SIGNAL(clicked()), this, SLOT(EnterRoomAfterChatWindowInfo()));
    connect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterChatWindowInfo()));
    connect(layoutChatWindow, SIGNAL(refresh()), layoutChatWindowInfo, SLOT(usersToRoomView()));
}

void MainWindow::RoomName()
{
    layoutChatWindow->SelectedRoom = layoutChatWindowInfo->SelectedRoom;
    emit RoomSelectedDone();
}

void MainWindow::EnterRoomAfterChatWindowInfo()
{
    disconnect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterChatWindowInfo()));
    layoutEnterRoom = new EnterRoom(db, layoutChatWindowInfo->YourLogin);
    layoutChatWindow->SelectedRoom="";
    layoutChatWindow->SetRoomSelected();
    delete layoutChatWindowInfo;
    ui->Layout->addWidget(layoutEnterRoom, 0,0,1,2);
    this->resize(720,480);

    connect(layoutEnterRoom->pushButtonBack, SIGNAL(clicked()), this, SLOT(ChatWindowInfoAfterEnterRoom()));
    connect(layoutEnterRoom, SIGNAL(RoomEnterDone()), this, SLOT(ChatWindowInfoAfterEnterRoom()));
    connect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterCreateRoom()));
}

void MainWindow::ChatWindowInfoAfterEnterRoom()
{
    disconnect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterCreateRoom()));
    layoutChatWindowInfo = new ChatWindowInfo(db, layoutChatWindow->YourLogin);
    delete layoutEnterRoom;
    ui->Layout->addWidget(layoutChatWindowInfo, 0,0,2,2);
    this->resize(720,480);

    connect(layoutChatWindowInfo, SIGNAL(RoomIsSelect()), this, SLOT(RoomName()));
    connect(this, SIGNAL(RoomSelectedDone()), layoutChatWindow, SLOT(SetRoomSelected()));

    connect(layoutChatWindowInfo->pushButtonCreateRoom, SIGNAL(clicked()), this, SLOT(CreateRoomAfterChatWindowInfo()));
    connect(layoutChatWindowInfo->pushButtonEnterRoom, SIGNAL(clicked()), this, SLOT(EnterRoomAfterChatWindowInfo()));
    connect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterChatWindowInfo()));
    connect(layoutChatWindow, SIGNAL(refresh()), layoutChatWindowInfo, SLOT(usersToRoomView()));
}

void MainWindow::CreateRoomAfterChatWindowInfo()
{
    disconnect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterChatWindowInfo()));
    layoutCreateRoom = new CreateRoom(db, layoutChatWindowInfo->YourLogin);
    layoutChatWindow->SelectedRoom="";
    layoutChatWindow->SetRoomSelected();
    delete layoutChatWindowInfo;
    ui->Layout->addWidget(layoutCreateRoom, 0,0,1,2);
    this->resize(720,480);

    connect(layoutCreateRoom->pushButtonBack, SIGNAL(clicked()), this, SLOT(ChatWindowInfoAfterCreatRoom()));
    connect(layoutCreateRoom, SIGNAL(RoomCreateDone()), this, SLOT(ChatWindowInfoAfterCreatRoom()));
    connect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterCreateRoom()));
}

void MainWindow::ChatWindowInfoAfterCreatRoom()
{
    disconnect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterCreateRoom()));
    layoutChatWindowInfo = new ChatWindowInfo(db, layoutChatWindow->YourLogin);
    delete layoutCreateRoom;
    ui->Layout->addWidget(layoutChatWindowInfo, 0,0,2,2);
    this->resize(720,480);

    connect(layoutChatWindowInfo, SIGNAL(RoomIsSelect()), this, SLOT(RoomName()));
    connect(this, SIGNAL(RoomSelectedDone()), layoutChatWindow, SLOT(SetRoomSelected()));

    connect(layoutChatWindowInfo->pushButtonEnterRoom, SIGNAL(clicked()), this, SLOT(EnterRoomAfterChatWindowInfo()));
    connect(layoutChatWindowInfo->pushButtonCreateRoom, SIGNAL(clicked()), this, SLOT(CreateRoomAfterChatWindowInfo()));
    connect(layoutChatWindow, SIGNAL(ExitFromChat()), this, SLOT(LoginUserAfterChatWindowInfo()));
    connect(layoutChatWindow, SIGNAL(refresh()), layoutChatWindowInfo, SLOT(usersToRoomView()));
}

void MainWindow::LoginUserAfterCreateRoom()
{
    layoutLogin = new Login(db);
    delete layoutCreateRoom;
    delete layoutChatWindow;
    ui->Layout->addWidget(layoutLogin);
    this->resize(360,240);

    connect(layoutLogin, SIGNAL(LoginDone()), this, SLOT(ChatWindowAfterLogin()));
    connect(layoutLogin->pushButtonRegistration,SIGNAL(clicked()), this, SLOT(RegistrationAfterLoginUser()));
}

void MainWindow::LoginUserAfterChatWindowInfo()
{
    layoutLogin = new Login(db);
    delete layoutChatWindow;
    delete layoutChatWindowInfo;
    ui->Layout->addWidget(layoutLogin);
    this->resize(360,240);

    connect(layoutLogin, SIGNAL(LoginDone()), this, SLOT(ChatWindowAfterLogin()));
    connect(layoutLogin->pushButtonRegistration,SIGNAL(clicked()), this, SLOT(RegistrationAfterLoginUser()));
}

void MainWindow::LoginUserAfterRegistration()
{
    layoutLogin = new Login(db);
    delete layoutRegistration;
    ui->Layout->addWidget(layoutLogin);
    this->resize(360,240);

    connect(layoutLogin, SIGNAL(LoginDone()), this, SLOT(ChatWindowAfterLogin()));
    connect(layoutLogin->pushButtonRegistration,SIGNAL(clicked()), this, SLOT(RegistrationAfterLoginUser()));
}

void MainWindow::RegistrationAfterLoginUser()
{
    layoutRegistration = new Registration(db);
    delete layoutLogin;
    ui->Layout->addWidget(layoutRegistration);
    this->resize(360,240);

    connect(layoutRegistration, SIGNAL(RegistrationDone()), this, SLOT(LoginUserAfterRegistration()));
    connect(layoutRegistration->pushButtonBack, SIGNAL(clicked()), this, SLOT(LoginUserAfterRegistration()));
}

