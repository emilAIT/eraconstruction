#include "excelbyclientmanager.h"

ExcelByClientManager::ExcelByClientManager() {}

QString ExcelByClientManager::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void ExcelByClientManager::exportToExcel(QString title, QList<QAbstractItemModel *> models, int start)
{
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/eraCostruction_отчеты");
    if (!folder.exists())
    {
        folder.mkdir(appDir + "/eraCostruction_отчеты");
    }

    QString fileName = title + " " + ".xlsx";
    fileName.replace(" ", "_");

    QString filePath = appDir + "/eraCostruction_отчеты/" + fileName;

    // Создаем Excel документ
    QAxObject *excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", false);

    QAxObject *workbooks = excel->querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Add");
    QAxObject *sheet = workbook->querySubObject("Sheets(int)", 1);

    int currentRow = 1;

    // Заголовок
    sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", title);
    sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Font.Size", 14);
    currentRow++;

    // Дата
    // sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", dates);
    // currentRow++;

    // Добавляем таблицы из моделей
    for (int k = 0; k < models.size(); k++)
    {
        QAbstractItemModel *model = models[k];
        int columnOffset = start == 1 ? 2 : 1;

        // Устанавливаем ширину колонок
        QAxObject *columns = sheet->querySubObject("Columns");
        columns->setProperty("ColumnWidth", 30);

        // Добавляем нумерацию
        if (start == 1)
        {
            QAxObject *headerCell = sheet->querySubObject("Cells(int,int)", currentRow, 1);
            headerCell->setProperty("Value", "#");

            // Стилизация хедера
            headerCell->querySubObject("Font")->setProperty("Color", QColor("#FFFFFF").rgb());
            headerCell->querySubObject("Interior")->setProperty("Color", QColor("#007700").rgb());
            headerCell->querySubObject("Borders")->setProperty("LineStyle", 1);
        }

        // Добавляем заголовки с цветом и границами
        for (int j = start; j < model->columnCount(); j++)
        {
            QString header = model->headerData(j, Qt::Horizontal).toString();
            QAxObject *headerCell = sheet->querySubObject("Cells(int,int)", currentRow, j + columnOffset - start);
            headerCell->setProperty("Value", header);

            // Устанавливаем цвет текста заголовка
            headerCell->querySubObject("Font")->setProperty("Color", QColor("#ffffff").rgb());
            // Цвет фона заголовка
            headerCell->querySubObject("Interior")->setProperty("Color", QColor("#007700").rgb());
            // Добавляем границы
            headerCell->querySubObject("Borders")->setProperty("LineStyle", 1);
        }
        currentRow++;

        // Заполняем данные с границами
        for (int i = 0; i < model->rowCount(); i++)
        {
            if (start == 1)
            {
                QAxObject *cell = sheet->querySubObject("Cells(int,int)", currentRow, 1);
                cell->setProperty("Value", i + 1);

                QAxObject *border = cell->querySubObject("Borders");
                border->setProperty("LineStyle", 1);
            }

            for (int j = start; j < model->columnCount(); j++)
            {
                QString cellData = model->index(i, j).data(Qt::DisplayRole).toString();
                QAxObject *cell = sheet->querySubObject("Cells(int,int)", currentRow, j + columnOffset - start);
                cell->setProperty("Value", cellData);

                // Добавляем границы
                QAxObject *border = cell->querySubObject("Borders");
                border->setProperty("LineStyle", 1);
            }
            currentRow++;
        }
        currentRow++;
    }

    // Автоизменение ширины колонок
    QAxObject *usedRange = sheet->querySubObject("UsedRange");
    usedRange->querySubObject("Columns")->dynamicCall("AutoFit()");

    // Сохранение файла
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filePath));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete excel;

    QMessageBox::information(nullptr, "Экспорт Excel", "Отчет сохранен в папке 'eraCostruction_отчеты' на рабочем столе.");
}

