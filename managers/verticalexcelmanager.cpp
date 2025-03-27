#include "verticalexcelmanager.h"

VerticalExcelManager::VerticalExcelManager(QObject *parent)
    : QObject(parent)
{
}

QString VerticalExcelManager::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void VerticalExcelManager::exportToExcelVertical(const QString &title,
                                                 const QString &dates,
                                                 QAbstractItemModel *model)
{
    if (!model) {
        QMessageBox::warning(nullptr, "Ошибка", "Данные для экспорта отсутствуют.");
        return;
    }

    // Создаём папку "eraCostruction_отчеты" на рабочем столе, если её нет
    QDateTime time = QDateTime::currentDateTime();
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/eraCostruction_отчеты");
    if (!folder.exists()) {
        folder.mkdir(appDir + "/eraCostruction_отчеты");
    }

    // Формируем имя файла
    // Например: Общий_отчет_21.02.2025_18-30-00.xlsx
    // (чтобы избежать пробелов, можно заменить их на "_")
    QString fileName = title + " " + dates + " " + time.toString("dd.MM.yyyy HH-mm-ss") + ".xlsx";
    fileName.replace(" ", "_");  // заменяем пробелы на подчёркивания
    QString filePath = appDir + "/eraCostruction_отчеты/" + fileName;

    // Создаем COM-объект Excel (работает только на Windows + установленный MS Office)
    QAxObject *excel = new QAxObject("Excel.Application");
    if (!excel || excel->isNull()) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось запустить Excel. Убедитесь, что MS Office установлен.");
        return;
    }
    excel->setProperty("Visible", false);

    // Создаем новую книгу
    QAxObject *workbooks = excel->querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Add");
    // Выбираем первый лист
    QAxObject *sheet = workbook->querySubObject("Sheets(int)", 1);

    // Текущая строка, с которой начнём заполнять
    int currentRow = 1;

    // 1) Заголовок (title)
    sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", title);
    sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Font.Size", 14);
    currentRow++;

    // 2) Даты (dates)
    sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", dates);
    currentRow += 2; // небольшой отступ вниз

    // 3) Заполняем данные «вертикально»
    // Подобно тому, как это делается в VerticalPDFmanager.
    // Для каждой строки:
    //   - label = заголовок строки из вертикального хедера
    //   - value = данные из ячейки (row, 0) (если у нас одна колонка)
    // Записываем в Excel: первая колонка - label, вторая колонка - value.
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QString rowLabel = model->headerData(row, Qt::Vertical).toString();
        QString value = model->index(row, 0).data(Qt::DisplayRole).toString();

        sheet->querySubObject("Cells(int,int)", currentRow, 1)->setProperty("Value", rowLabel);
        sheet->querySubObject("Cells(int,int)", currentRow, 2)->setProperty("Value", value);

        currentRow++;
    }

    // Автоизменение ширины колонок
    QAxObject *usedRange = sheet->querySubObject("UsedRange");
    usedRange->querySubObject("Columns")->dynamicCall("AutoFit()");

    // Сохраняем и закрываем
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filePath));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete excel;

    QMessageBox::information(nullptr,
                             "Экспорт в Excel",
                             QString("Отчет сохранен в папке 'eraCostruction_отчеты' на рабочем столе"));
}
