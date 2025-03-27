#include "settings.h"
#include "ui_settings.h"

Settings::Settings(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->nav = nav;
}

Settings::~Settings()
{
    delete ui;
}


void Settings::setSettings(SettingsE table) {
    this->table = table;
    setHeader();
    setTable();
}


void Settings::on_backButton_clicked()
{
    nav->changeWindow(0);
}



void Settings::setHeader() {
    QString header = "header";
    switch (this->table) {
    case SettingsE::Clients:
        header = "Клиенты";
        ui->detailsButton->setDisabled(true);
        break;

    case SettingsE::PaymentTypes:
        header = "Тип оплаты";
        ui->detailsButton->setDisabled(true);
        break;

    case SettingsE::Users:
        header = "Пользователи";
        ui->detailsButton->setDisabled(true);
        break;
    default:
        break;
    }
    ui->EcoTaxi->setText(header);
}



void Settings::setTable() {
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->table) {
    case SettingsE::Clients:
        model->setHorizontalHeaderLabels({
            "id", "Имя", "Номер", "Должность", "Дата рождения",
            "Дата выдачи", "Паспорт", "Место жительства", "Описание"
        });

        for (const QVariant &client : Operations::selectAllClients()) {
            QVariantList values = client.toList();
            QList<QStandardItem *> row;

            for (const QVariant &value : values) {
                row.append(new QStandardItem(value.toString()));
            }

            model->appendRow(row);
        }
        break;
    
    case SettingsE::PaymentTypes:
        model->setHorizontalHeaderLabels({"id", "Название"});
        for (QVariant paymentType : Operations::selectAllPaymentTypes()) {
            QVariantList values = paymentType.toList();
            model->appendRow({
                new QStandardItem(values[0].toString()),
                new QStandardItem(values[1].toString())
            });
        }
        break;

    case SettingsE::Users:
        model->setHorizontalHeaderLabels({"id", "Пользователи", "Описание"});
        for (QVariant user : Operations::selectAllUsers()) {
            QVariantList values = user.toList();
            model->appendRow({
                new QStandardItem(values[0].toString()),
                new QStandardItem(values[1].toString()),
                new QStandardItem(values[4].toString())
            });
        }
    default:
        break;
    }

    ui->tableView->setModel(model);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &Settings::onSectionResized);


    switch (this->table)
    {
    case SettingsE::Clients:
    case SettingsE::Users:
        ui->tableView->setColumnWidth(0, 100);
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 200);
        break;
    }
}


void Settings::on_addButton_clicked()
{
    AddUpdateSettings *w;
    switch (this->table) {
    case SettingsE::Clients:
        w = new AddUpdateSettings(AddUpdateSettingsE::Clients, -1);
        break;
    case SettingsE::PaymentTypes:
        w = new AddUpdateSettings(AddUpdateSettingsE::PaymentTypes, -1);
        break;
    case SettingsE::Users:
        w = new AddUpdateSettings(AddUpdateSettingsE::Users, -1);
        break;
    default:
        w = new AddUpdateSettings(AddUpdateSettingsE::Clients, -1);
        break;
    }

    w->resize(w->minimumSizeHint());
    w->show();

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setTable();
}



void Settings::on_editButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        AddUpdateSettings *w;
        switch (this->table) {
        case SettingsE::Clients:
            w = new AddUpdateSettings(AddUpdateSettingsE::Clients, id);
            break;
        case SettingsE::PaymentTypes:
            w = new AddUpdateSettings(AddUpdateSettingsE::PaymentTypes, id);
            break;
        case SettingsE::Users:
            w = new AddUpdateSettings(AddUpdateSettingsE::Users, id);
            break;
        default:
            w = new AddUpdateSettings(AddUpdateSettingsE::Clients, id);
            break;
        }

        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}


void Settings::on_deleteButton_clicked() {
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();
        CustomDialog *d = nullptr;

        switch (this->table) {
            case SettingsE::Clients: {
                QVariantList client = Operations::getClient(id);
                QVariantList data = client.first().toList();
                QString name = data[1].toString();
                d = new CustomDialog("Вы уверены что хотите удалить клиента " + name + "?");
                break;
            }

            case SettingsE::PaymentTypes: {
                QVariantList paymentType = Operations::getPaymentType(id);
                QVariantList data = paymentType.first().toList();
                QString name = data[1].toString();
                d = new CustomDialog("Вы уверены что хотите удалить тип оплаты " + name + "?");
                break;
            }

            case SettingsE::Users: {
                QVariantList user = Operations::getUser(id);
                QVariantList data = user.first().toList();
                QString name = data[1].toString();
                d = new CustomDialog("Вы уверены что хотите удалить пользователя " + name + "?");
                break;
            }

            default:
                // Handle unexpected case
                break;
        }

        if (d)
        {
            connect(d, &CustomDialog::yesClicked, this, &Settings::onYes);
            connect(d, &CustomDialog::noClicked, this, &Settings::onNo);
            d->show();
        }
    }
}


void Settings::onYes()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();
        QString operationDescription;

        switch (this->table)
        {
        case SettingsE::Clients: {
            QVariantList client = Operations::getClient(id);
            if (!client.isEmpty()) {
                QVariantList data = client.first().toList();
                QString name = data[1].toString();
            }
            Operations::deleteClient(id);
            break;
        }

        case SettingsE::PaymentTypes: {
            QVariantList paymentType = Operations::getPaymentType(id);
            if (!paymentType.isEmpty()) {
                QVariantList data = paymentType.first().toList();
                QString name = data[1].toString();
            }
            Operations::deletePaymentType(id);
            break;
        }

        case SettingsE::Users: {
            QVariantList user = Operations::getUser(id);
            if (!user.isEmpty()) {
                QVariantList data = user.first().toList();
                QString name = data[1].toString();
            }
            Operations::deleteUser(id);
            break;
        }

        default:
            qDebug() << "Unknown table type for deletion";
            break;
        }
        setTable();
    }
}



void Settings::onNo()
{

}



void Settings::onSectionResized(int logicalIndex, int oldSize, int newSize) {
    if (logicalIndex != ui->tableView->model()->columnCount() - 1) {
        adjustColumnWidths();
    }
}

void Settings::adjustColumnWidths() {
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i) {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 1130 - totalWidth;
    if (lastColumnWidth > 150) {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    } else {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}



