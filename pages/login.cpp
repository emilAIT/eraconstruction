#include "login.h"
#include "pages/ui_login.h"
// #include "ui_login.h"

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowTitle("Авторизация | EraConstruction");
    this->setWindowIcon(QIcon());
    ui->password->setProperty("color", "white");
    ui->username->setProperty("color", "white");
    // ui->groupBox->setStyleSheet("background: transparent");
}

login::~login()
{
    delete ui;
}


// void login::logIn()
// {
//     if (ui->passwordEdit->text() == "123")
//     {
//         navigationManager *w = new navigationManager();
//         this->hide();
//         w->showMaximized();
//         // w->show();
//     }
//     else
//     {
//         QApplication::quit();
//     }
// }

void login::logIn()
{
    userSession &u = userSession::getInstance(); // Получаем сессию пользователя
    QVariantMap loginResult = u.logMe(ui->usernameEdit->text(), ui->passwordEdit->text());

    if (loginResult["success"].toBool()) // Пытаемся войти
    {
        navigationManager *w = new navigationManager(); // Создаем главное окно
        this->hide();
        w->showMaximized();
        w->disableReportsButton(loginResult["isAdmin"].toBool());

    }
    else
    {
        QApplication::quit(); // Закрываем приложение при неудачном входе
    }
}



void login::on_loginButton_clicked() {
    logIn();
}

void login::on_passwordEdit_returnPressed() {
    logIn();
}
