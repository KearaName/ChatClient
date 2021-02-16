#include "registration.h"

Registration::Registration(QSqlDatabase DataBase, QWidget *parent) : QWidget(parent)
{
    db = DataBase;

    labelLogin = new QLabel(tr("Придумайте логин:"));
    labelPassword = new QLabel(tr("Придумайте пароль:"));

    lineEditLogin = new QLineEdit;
    lineEditPassword = new QLineEdit;

    lineEditPassword -> setEchoMode(QLineEdit::Password);

    pushButtonRegistrationUser = new QPushButton;
    pushButtonBack = new QPushButton;

    pushButtonRegistrationUser->setText("Создать аккаунт");
    pushButtonBack->setText("Назад");

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(labelLogin, 0,0,1,-1);
    gridLayout->addWidget(lineEditLogin,1,0,1,-1);
    gridLayout->addWidget(labelPassword,2,0,1,-1);
    gridLayout->addWidget(lineEditPassword, 3,0,1,-1);
    gridLayout->addWidget(pushButtonRegistrationUser,4,0,1,1);
    gridLayout->addWidget(pushButtonBack, 4,1,1,1);
    setLayout(gridLayout);

    connect(pushButtonRegistrationUser, SIGNAL(clicked()), this, SLOT(on_pushButtonRegistrationUser_clicked()));
}

Registration::~Registration()
{
    delete labelLogin;
    delete lineEditLogin;
    delete labelPassword;
    delete lineEditPassword;
    delete pushButtonRegistrationUser;
}

void Registration::on_pushButtonRegistrationUser_clicked()
{
    if(db.open()) {

        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("INSERT INTO tableUsers(User_login, User_password, User_status) "
                   "VALUES (:User_login, :User_password, :User_status)");

        querySQL->bindValue(":User_login", lineEditLogin->text() == NULL ? NULL : lineEditLogin->text());
        querySQL->bindValue(":User_password", lineEditPassword->text() == NULL ? NULL : lineEditPassword->text());
        querySQL->bindValue(":User_status", "offline");

        if(querySQL->exec()) {
            QMessageBox::warning(this, tr("Успешно!"),
                                 tr("Вы зарегистрировались!"));
            delete querySQL;
            db.close();
            emit RegistrationDone();
        }
        else {
            QMessageBox::critical(this, tr("Ошибка!"),
                                     tr("Вы не можете быть зарегистрированы\n")+querySQL->lastError().text());
        }
    }
    else {
         QMessageBox::critical(this, tr("Ошибка!"),
                                  tr("Вы не можете быть зарегистрированы, приложение не имеет доступа к серверу\n") + db.lastError().text());
    }
}
