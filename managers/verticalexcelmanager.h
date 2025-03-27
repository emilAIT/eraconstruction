#ifndef VERTICALEXCELMANAGER_H
#define VERTICALEXCELMANAGER_H

#include <QObject>
#include <QAbstractItemModel>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QAxObject>

class VerticalExcelManager : public QObject
{
    Q_OBJECT
public:
    explicit VerticalExcelManager(QObject *parent = nullptr);

    // Экспорт "вертикального" отчёта в Excel
    void exportToExcelVertical(const QString &title,
                               const QString &dates,
                               QAbstractItemModel *model);

private:
    QString getDesktopDir();
};

#endif // VERTICALEXCELMANAGER_H
