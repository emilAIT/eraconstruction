#ifndef VERTICALPDFMANAGER_H
#define VERTICALPDFMANAGER_H

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QList>
#include <QMimeData>
#include <QPrinter>
#include <QStandardPaths>
#include <QTextDocument>
#include <QClipboard>
#include <QMessageBox>
#include <QAbstractItemModel>

class VerticalPDFmanager
{
public:
    VerticalPDFmanager();

    // Method to initiate PDF export
    void ToVerticalPDF(QString title, QString dates, QAbstractItemModel *model);

private:
    // Helper methods
    QString getStyleSheet();
    QString getAppDir();
    QString getDesktopDir();
    void createPDF(QString html, QString title);
    QString getHeader(QDateTime time);
    QString getFooter(QDateTime time);
    QString modelToVerticalHTML(QAbstractItemModel *model);
};

#endif // VERTICALPDFMANAGER_H
