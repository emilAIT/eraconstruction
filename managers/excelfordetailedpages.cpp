#include "excelForDetailedPages.h"
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QColor>
#include <ActiveQt/QAxObject>

ExcelForDetailedPages::ExcelForDetailedPages() {}

QString ExcelForDetailedPages::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void ExcelForDetailedPages::exportToExcel(QString title, QString dates, QMap<QString, QString> additionalData, QList<QAbstractItemModel *> models, int start)
{
    QDateTime time = QDateTime::currentDateTime();
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/eraCostruction_отчеты");
    if (!folder.exists())
    {
        folder.mkdir(appDir + "/eraCostruction_отчеты");
    }

    QString fileName = title + " " + time.toString("dd.MM.yyyy HH-mm-ss") + ".xlsx";
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

    // Даты
    // sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", dates);
    currentRow++;

    // Экспорт дополнительных данных (информация, отображаемая на правой части страницы)
    if (!additionalData.isEmpty())
    {
        // Заголовок для дополнительной информации
        sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", "Дополнительная информация:");
        sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Font.Bold", true);
        currentRow++;

        QMapIterator<QString, QString> it(additionalData);
        while(it.hasNext())
        {
            it.next();
            sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", it.key());
            sheet->querySubObject("Cells(int,int)", currentRow, 2)->setProperty("Value", it.value());
            currentRow++;
        }
        // Пустая строка после дополнительной информации
        currentRow++;
    }

    // Добавляем таблицы из моделей (как в стандартном ExcelManager)
    for (int k = 0; k < models.size(); k++)
    {
        QAbstractItemModel *model = models[k];
        int columnOffset = (start == 1) ? 2 : 1;

        // Устанавливаем ширину колонок
        QAxObject *columns = sheet->querySubObject("Columns");
        columns->setProperty("ColumnWidth", 30);

        // Добавляем нумерацию, если нужно
        if (start == 1)
        {
            QAxObject *headerCell = sheet->querySubObject("Cells(int,int)", currentRow, 1);
            headerCell->setProperty("Value", "#");
            headerCell->querySubObject("Font")->setProperty("Color", QColor("#FFFFFF").rgb());
            headerCell->querySubObject("Interior")->setProperty("Color", QColor("#007700").rgb());
            headerCell->querySubObject("Borders")->setProperty("LineStyle", 1);
        }

        // Добавляем заголовки столбцов таблицы с цветом и границами
        for (int j = start; j < model->columnCount(); j++)
        {
            QString header = model->headerData(j, Qt::Horizontal).toString();
            QAxObject *headerCell = sheet->querySubObject("Cells(int,int)", currentRow, j + columnOffset - start);
            headerCell->setProperty("Value", header);
            headerCell->querySubObject("Font")->setProperty("Color", QColor("#ffffff").rgb());
            headerCell->querySubObject("Interior")->setProperty("Color", QColor("#007700").rgb());
            headerCell->querySubObject("Borders")->setProperty("LineStyle", 1);
        }
        currentRow++;

        // Заполняем данными таблицы с границами
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
