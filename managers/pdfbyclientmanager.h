#ifndef PDFBYCLIENTMANAGER_H
#define PDFBYCLIENTMANAGER_H

#include <QCoreApplication>
#include <QTextDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>

#include <QPdfWriter>
#include <QPrinter>
#include <QStandardPaths>

#include <QDate>

#include "qstandarditemmodel.h"

class PdfByClientManager
{
public:
    PdfByClientManager();

    // general
    void static exportToPDF(QString title, QList<QAbstractItemModel *> models, int start = 1);

    void static ToPDF(QString title,  QList<QAbstractItemModel *> models, int start = 1);

    QString static modelToHTML(QAbstractItemModel *model, int start);

    QString static getStyleSheet();

    QString static getHeader(QDateTime time);
    QString static getFooter(QDateTime time);

private:
    QString static getAppDir();
    QString static getDesktopDir();

    // QString static getStyleSheet();

    // QString static getHeader(QDateTime time);
    // QString static getFooter(QDateTime time);

    void static createPDF(QString html, QString title);
};

#endif // PDFBYCLIENTMANAGER_H
