#ifndef EXCELFORDETAILEDPAGES_H
#define EXCELFORDETAILEDPAGES_H

#include <QList>
#include <QAbstractItemModel>
#include <QString>
#include <QMap>

class ExcelForDetailedPages
{
public:
    ExcelForDetailedPages();

    QString getDesktopDir();
    // Функция экспортирует данные: title – заголовок, dates – даты, additionalData – дополнительные данные,
    // models – список моделей таблиц, start – индекс начала столбцов для экспорта (аналогично ExcelManager)
    void exportToExcel(QString title, QString dates, QMap<QString, QString> additionalData, QList<QAbstractItemModel*> models, int start);
};

#endif // EXCELFORDETAILEDPAGES_H
