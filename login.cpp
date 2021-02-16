#include "login.h"

Login::Login(QSqlDatabase DataBase, QWidget *parent) : QWidget(parent)
{

    db = DataBase;

    labelLogin = new QLabel(tr("Логин:"));
    labelPassword = new QLabel(tr("Пароль:"));

    lineEditLogin = new QLineEdit;
    lineEditPassword = new QLineEdit;

    lineEditPassword -> setEchoMode(QLineEdit::Password);

    pushButtonLogin = new QPushButton;
    pushButtonLogin->setText("Войти");

    pushButtonRegistration = new QPushButton;
    pushButtonRegistration->setText("Регистрация");

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(pushButtonLogin);
    hBoxLayout->addWidget(pushButtonRegistration);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(labelLogin);
    vBoxLayout->addWidget(lineEditLogin);
    vBoxLayout->addWidget(labelPassword);
    vBoxLayout->addWidget(lineEditPassword);

    QVBoxLayout *vMainBoxLayout = new QVBoxLayout;
    vMainBoxLayout->addLayout(vBoxLayout);
    vMainBoxLayout->addLayout(hBoxLayout);
    setLayout(vMainBoxLayout);

    connect(pushButtonLogin, SIGNAL(clicked()), this, SLOT(on_pushButtonLogin_clicked()));
}

Login::~Login()
{
    delete labelLogin;
    delete labelPassword;
    delete lineEditLogin;
    delete lineEditPassword;
    delete pushButtonLogin;
    delete pushButtonRegistration;
}

void Login::on_pushButtonLogin_clicked()
{
    if(db.open()) {

        QSqlQuery * querySQL = new QSqlQuery();
        querySQL->prepare("SELECT COUNT(User_login) FROM tableUsers WHERE User_login = :User_login AND User_password = :User_password");

        querySQL->bindValue(":User_login", lineEditLogin->text() == NULL ? NULL : lineEditLogin->text());
        querySQL->bindValue(":User_password", lineEditPassword->text() == NULL ? NULL : lineEditPassword->text());

        if(querySQL->exec()) {

            querySQL->first();
            QString Authetification = querySQL->value(0).toString();
            if(Authetification=="1") {
                QSqlQuery * querySQLLogin = new QSqlQuery();
                querySQLLogin->prepare("UPDATE tableUsers SET User_status = :User_status WHERE User_login = :User_login AND User_password = :User_password");

                querySQLLogin->bindValue(":User_status", "online");
                querySQLLogin->bindValue(":User_login", lineEditLogin->text() == NULL ? NULL : lineEditLogin->text());
                querySQLLogin->bindValue(":User_password", lineEditPassword->text() == NULL ? NULL : lineEditPassword->text());

                if(querySQLLogin->exec()) {
                    YourLogin=lineEditLogin->text();
                    delete querySQL;
                    delete querySQLLogin;
                    db.close();
                    emit LoginDone();
                }
                else {
                    QMessageBox::critical(this, tr("Ошибка!"),
                                             querySQLLogin->lastError().text());
                }
            }
            else {
                QMessageBox::critical(this, tr("Ошибка!"),
                                         tr("Неверно введен логин или пароль!"));
            }
        }
        else {
            QMessageBox::critical(this, tr("Ошибка!"),
                                     querySQL->lastError().text());
        }
    }
    else {
        QMessageBox::critical(this, tr("Ошибка!"),
                                 tr("Вы не можете войти, приложение не имеет доступа к серверу ") + db.lastError().text());
    }
}


