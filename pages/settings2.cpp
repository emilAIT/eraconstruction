#include "settings2.h"
#include "ui_settings2.h"

Settings2::Settings2(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Settings2)
{
    ui->setupUi(this);
    this->nav = nav;
}

Settings2::~Settings2()
{
    delete ui;
}


void Settings2::setSettings(SettingsE table) {
    this->table = table;
    setHeader();
    setSideTable();
    setTable();
}


void Settings2::setHeader() {
    QString header = "header";
    switch (this->table) {
    case SettingsE::Objects:
        header = "Объекты";
        ui->addSubButton->setText("Добавить квартиру");
        ui->AddButton->setText("Добавить объект");
        break;

    case SettingsE::Expenses:
        header = "Расходы";
        ui->addSubButton->setText("Добавить кто/что");
        ui->AddButton->setText("Добавить тип");
        break;
    default:
        break;
    }
    ui->EcoTaxi->setText(header);
}



void Settings2::setSideTable() {
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->table)
    {
    case SettingsE::Expenses:
        model->setHorizontalHeaderLabels({"id", "Тип"});
        for (QVariant type : Operations::selectAllTypes()) {
            QVariantList values = type.toList();
            model->appendRow({
                new QStandardItem(values[0].toString()),
                new QStandardItem(values[1].toString())
            });
        }
        break;
    case SettingsE::Objects:
        model->setHorizontalHeaderLabels({"id", "Объект"});
        for (QVariant type : Operations::selectAllObjects()) {
            QVariantList values = type.toList();
            model->appendRow({
                new QStandardItem(values[0].toString()),
                new QStandardItem(values[1].toString())
            });
        }
        break;

    default:
        break;
    }

    ui->sideTable->setModel(model);

    ui->sideTable->setColumnHidden(0, true);

    ui->sideTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    switch (this->table)
    {
    case SettingsE::Expenses:
        ui->tableView->setColumnWidth(1, 400);
        break;
    }

    connect(ui->sideTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Settings2::selectionChanged);
}




void Settings2::setTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);

    switch (this->table) {
    case SettingsE::Expenses:
        model->setHorizontalHeaderLabels({"id", "Кто/Что"});
        for (QVariant whoWhat : Operations::selectAllWhoWhats(id)) {
            QVariantList values = whoWhat.toList();
            model->appendRow({
                new QStandardItem(values[0].toString()),
                new QStandardItem(values[2].toString()),
            });
        }
        break;

    case SettingsE::Objects:
        model->setHorizontalHeaderLabels({"id", "Квартира", "Площадь м²", "Кол-во комнат", "Кол-во санузлов", "Этаж", "Цена м²", "Итого", "Валюта"});
        for (QVariant apartment : Operations::selectAllApartments(id)) {
            QVariantList values = apartment.toList();
            QList<QStandardItem*> row;

            row << new QStandardItem(values[0].toString());  // id
            row << new QStandardItem(values[2].toString());  // apartmentNumber (Квартира)
            // row << new QStandardItem(values[3].toString());  // Площадь м²

            // Площадь м²
            double ploshad = values[3].toDouble();
            QString formattedPloshad = locale.toString(ploshad, 'f', 2);
            QStandardItem *item = new QStandardItem();
            item->setData(formattedPloshad, Qt::DisplayRole);
            item->setData(ploshad, Qt::UserRole);
            row << item;

            // Кол-во комнат
            int rooms = values[4].toInt();
            QStandardItem *itemRooms = new QStandardItem();
            itemRooms->setData(locale.toString(rooms), Qt::DisplayRole);
            itemRooms->setData(rooms, Qt::UserRole);
            row << itemRooms;

            // Кол-во санузлов
            int bathrooms = values[5].toInt();
            QStandardItem *itemBathrooms = new QStandardItem();
            itemBathrooms->setData(locale.toString(bathrooms), Qt::DisplayRole);
            itemBathrooms->setData(bathrooms, Qt::UserRole);
            row << itemBathrooms;

            // Другое значение
            int otherValue = values[6].toInt();
            QStandardItem *itemOther = new QStandardItem();
            itemOther->setData(locale.toString(otherValue), Qt::DisplayRole);
            itemOther->setData(otherValue, Qt::UserRole);
            row << itemOther;

            // Форматирование для "Цена м²"
            double pricePerSquare = values[7].toDouble();
            QString formattedPricePerSquare = locale.toString(pricePerSquare, 'f', 2);
            QStandardItem *priceItem = new QStandardItem();
            priceItem->setData(formattedPricePerSquare, Qt::DisplayRole);
            priceItem->setData(pricePerSquare, Qt::UserRole);
            row << priceItem;

            // Форматирование для "Итого"
            double totalAmount = values[8].toDouble();
            QString formattedTotalAmount = locale.toString(totalAmount, 'f', 2);
            QStandardItem *totalItem = new QStandardItem();
            totalItem->setData(formattedTotalAmount, Qt::DisplayRole);
            totalItem->setData(totalAmount, Qt::UserRole);
            row << totalItem;

            row << new QStandardItem(values[9].toString());

            model->appendRow(row);
        }
        break;

    default:
        break;
    }

    // Подключение сортировки
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::UserRole);
    ui->tableView->setModel(proxyModel);

    // ui->tableView->setModel(model);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &Settings2::onSectionResized);


    switch (this->table)
    {
    case SettingsE::Expenses:
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 400);
        break;
    case SettingsE::Objects:
        ui->tableView->setColumnWidth(1, 150);
        ui->tableView->setColumnWidth(2, 150);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 100);
        ui->tableView->setColumnWidth(6, 150);
        ui->tableView->setColumnWidth(7, 150);
        break;
    }
}


void Settings2::on_AddButton_clicked()
{
    AddUpdateSettings *w;
    switch (this->table) {
    case SettingsE::Expenses:
        w = new AddUpdateSettings(AddUpdateSettingsE::Type, -1);
        break;
    case SettingsE::Objects:
        w = new AddUpdateSettings(AddUpdateSettingsE::Objects, -1);
        break;
    default:
        w = new AddUpdateSettings(AddUpdateSettingsE::Type, -1);
        break;
    }

    w->resize(w->minimumSizeHint());
    w->show();

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setSideTable();
}


void Settings2::on_addSubButton_clicked()
{
    AddUpdateSettings *w;
    switch (this->table) {
    case SettingsE::Expenses:
        w = new AddUpdateSettings(AddUpdateSettingsE::WhoWhat, -1);
        break;
    case SettingsE::Objects:
        w = new AddUpdateSettings(AddUpdateSettingsE::Apartments, -1);
        break;
    default:
        w = new AddUpdateSettings(AddUpdateSettingsE::WhoWhat, -1);
        break;
    }

    w->resize(w->minimumSizeHint());
    w->show();

    connect(w, &AddUpdateSettings::objectAdded, this, &Settings2::onItemAdded); // Подключение сигнала

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setTable();
}


void Settings2::on_editButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    AddUpdateSettings *w;

    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->table) {
        case SettingsE::Expenses:
            w = new AddUpdateSettings(AddUpdateSettingsE::WhoWhat, id);
            break;

        case SettingsE::Objects:
            w = new AddUpdateSettings(AddUpdateSettingsE::Apartments, id);
            break;

        default:
            break;
        }
    } else {
        select = ui->sideTable->selectionModel();

        if (select->hasSelection())
        {
            int row = select->selectedRows().at(0).row();

            int id = ui->sideTable->model()->index(row, 0).data().toLongLong();

            switch (this->table) {
            case SettingsE::Expenses:
                w = new AddUpdateSettings(AddUpdateSettingsE::Type, id);
                break;
            case SettingsE::Objects:
                w = new AddUpdateSettings(AddUpdateSettingsE::Objects, id);
                break;
            default:
                break;
            }
        }
        else {
            return;
        }
    }

    w->resize(w->minimumSizeHint());
    w->show();

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setTable();
    setSideTable();
}

void Settings2::on_deleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection()) {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        CustomDialog *d = nullptr;

        switch (this->table) {
        case SettingsE::Expenses: {
            QVariantList whoWhat = Operations::getWhoWhat(id);
            if (!whoWhat.isEmpty()) {
                QVariantList whoWhats = whoWhat.first().toList();
                if (whoWhats.size() > 1) {
                    QString name = whoWhats[1].toString();
                    qDebug() << "WhoWhat name:" << name;
                    d = new CustomDialog("Вы уверены что хотите удалить " + name + " из Кто/Что ?");
                }
            }
            break;
        }

        case SettingsE::Objects: {
            QVariantList apartment = Operations::getApartment(id);
            if (!apartment.isEmpty()) {
                QVariantList apartmentData = apartment.first().toList();
                if (apartmentData.size() > 1) {
                    QString name = apartmentData[2].toString();
                    qDebug() << "apartment name:" << name;

                    // Проверяем, находится ли квартира в продаже
                    if (Operations::isApartmentInSales(id)) {
                        d = new CustomDialog("Вы уверены? Квартира " + name + " находится в продаже.");
                    } else {
                        d = new CustomDialog("Вы уверены, что хотите удалить квартиру " + name + "?");
                    }
                }
            }
            break;
        }
        default:
            qDebug() << "Error: Unknown table type.";
            break;
        }

        if (d) {
            qDebug() << "Showing confirmation dialog.";
            connect(d, &CustomDialog::yesClicked, this, &Settings2::onYes);
            connect(d, &CustomDialog::noClicked, this, &Settings2::onNo);
            d->show();
        } else {
            qDebug() << "Error: CustomDialog not created.";
        }
    } else {
        qDebug() << "No selection in tableView. Checking sideTable.";
        select = ui->sideTable->selectionModel();

        if (select->hasSelection()) {
            int row = select->selectedRows().at(0).row();
            int id = ui->sideTable->model()->index(row, 0).data().toLongLong();
            qDebug() << "Selected row in sideTable:" << row << ", ID:" << id;

            CustomDialog *d = nullptr;

            switch (this->table) {
            case SettingsE::Expenses: {
                QVariantList type = Operations::getType(id);
                if (!type.isEmpty()) {
                    QVariantList typeData = type.first().toList();
                    if (typeData.size() > 1) {
                        QString name = typeData[1].toString();
                        qDebug() << "Type name:" << name;
                        d = new CustomDialog("Вы уверены что хотите удалить тип " + name + "?");
                    }
                }
                break;
            }

            case SettingsE::Objects: {
                QVariantList object = Operations::getObject(id);
                if (!object.isEmpty()) {
                    QVariantList objectData = object.first().toList();
                    if (objectData.size() > 1) {
                        QString name = objectData[1].toString();
                        qDebug() << "object name:" << name;

                        // Проверяем, есть ли квартиры в продаже
                        QVariantList apartmentsInSales = Operations::getApartmentsInSales(id);
                        if (!apartmentsInSales.isEmpty()) {
                            // Если квартиры в продаже есть, показываем предупреждение
                            QString warningMessage = "Вы уверены? Для объекта " + name + " есть квартиры в продаже. ";

                            // Добавляем номера квартир в продаже в предупреждающее сообщение
                            warningMessage += "\nКвартиры в продаже: ";
                            foreach (const QVariant &apartment, apartmentsInSales) {
                                QVariantList apartmentData = apartment.toList();
                                QString apartmentNumber = apartmentData[1].toString();
                                warningMessage += apartmentNumber + " ";
                            }

                            // Отображаем предупреждение перед удалением
                            d = new CustomDialog(warningMessage);
                        } else {
                            // Если квартир в продаже нет, то просто подтверждаем удаление объекта
                            d = new CustomDialog("Вы уверены, что хотите удалить объект " + name + "?");
                        }
                    }
                }
                break;
            }
            default:
                qDebug() << "Error: Unknown table object.";
                break;
            }

            if (d) {
                qDebug() << "Showing confirmation dialog.";
                connect(d, &CustomDialog::yesClicked, this, &Settings2::onYes);
                connect(d, &CustomDialog::noClicked, this, &Settings2::onNo);
                d->show();
            } else {
                qDebug() << "Error: CustomDialog not created.";
            }
        } else {
            qDebug() << "Error: No selection in either table.";
        }
    }
}

void Settings2::onYes()
{
    qDebug() << "Confirmation dialog 'Yes' clicked.";

    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (select->hasSelection()) {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();
        qDebug() << "Selected row in tableView:" << row << ", ID:" << id;

        switch (this->table) {
        case SettingsE::Expenses: {
            QVariantList whoWhat = Operations::getWhoWhat(id);
            QVariantList whoWhats = whoWhat.first().toList();

            if (!whoWhat.isEmpty()) {
                QString type = ui->sideTable->model()->index(row, 1).data().toString();
                QString whoName = whoWhats[1].toString();
            }
            Operations::deleteWhoWhat(id);
            break;
        }
        case SettingsE::Objects: {
            QVariantList apartment = Operations::getApartment(id);
            QVariantList data = apartment.first().toList();

            if (!apartment.isEmpty()) {
                QString type = ui->sideTable->model()->index(row, 1).data().toString();
                QString name = data[1].toString();
            }
            Operations::deleteApartment(id);
            break;
        }
        default:
            qDebug() << "Error: Unknown table type.";
            break;
        }
        setSideTable();
        setTable();
    } else {
        select = ui->sideTable->selectionModel();
        if (select->hasSelection()) {
            int row = select->selectedRows().at(0).row();
            int id = ui->sideTable->model()->index(row, 0).data().toLongLong();
            qDebug() << "Selected row in sideTable:" << row << ", ID:" << id;

            switch (this->table) {

            case SettingsE::Expenses: {
                QString typeName = ui->sideTable->model()->index(row, 1).data().toString();
                Operations::deleteType(id);
                break;
            }
            case SettingsE::Objects: {
                QString apartmenName = ui->sideTable->model()->index(row, 1).data().toString();
                Operations::deleteObject(id);
                break;
            }
            default:
                qDebug() << "Error: Unknown table type.";
                break;
            }

            setSideTable();
            setTable();
        } else {
            qDebug() << "Error: No selection made in either table.";
        }
    }
}

void Settings2::onNo() {

}


void Settings2::on_backButton_clicked()
{
    nav->changeWindow(0);
}




void Settings2::onItemAdded(int itemId) {
    setTable(); // Обновляем таблицу с объектами
    // Находим строку с добавленным объектом
    QAbstractItemModel *model = ui->sideTable->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        qDebug() <<model->index(row, 0).data().toInt() << row << itemId;
        if (model->index(row, 0).data().toInt() == itemId) {
            qDebug() << row;
            ui->sideTable->selectRow(row); // Выделяем строку
            break;
        }
    }
    setTable();
}



void Settings2::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    int row = selected.indexes()[0].row();

    this->id = ui->sideTable->model()->index(row, 0).data().toLongLong();

    setTable();

    // setBottomTable();
}
void Settings2::onTableViewSelectionChanged()
{
    QItemSelectionModel *tableSelection = ui->tableView->selectionModel();
    if (tableSelection->hasSelection()) {
        ui->sideTable->selectionModel()->clearSelection();
        qDebug() << "Selection made in tableView; cleared selection in sideTable.";
    }
}

void Settings2::onSideTableSelectionChanged()
{
    QItemSelectionModel *sideSelection = ui->sideTable->selectionModel();
    if (sideSelection->hasSelection()) {
        ui->tableView->selectionModel()->clearSelection();
        qDebug() << "Selection made in sideTable; cleared selection in tableView.";
    }
}

void Settings2::onSectionResized(int logicalIndex, int oldSize, int newSize) {
    if (logicalIndex != ui->tableView->model()->columnCount() - 1) {
        adjustColumnWidths();
    }
}


void Settings2::adjustColumnWidths() {
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
