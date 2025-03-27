#ifndef PDFFORDETAILEDPAGES_H
#define PDFFORDETAILEDPAGES_H

#include <QString>
#include <QList>
#include <QMap>
#include <QAbstractItemModel>

class PDFForDetailedPages
{
public:
    PDFForDetailedPages();

    QString getStyleSheet();
    QString getAppDir();
    QString getDesktopDir();
    QString getHeader();
    QString getFooter();
    void createPDF(const QString &html, const QString &title);
    QString modelToHTML(QAbstractItemModel *model, int start);
    QString additionalDataToHTML(const QMap<QString, QString> &additionalData);
    void ToPDF(const QString &title, const QString &dates, const QMap<QString, QString> &additionalData, const QList<QAbstractItemModel *> &models, int start);
    void exportToPDF(const QString &title, const QString &dates, const QMap<QString, QString> &additionalData, const QList<QAbstractItemModel *> &models, int start);
};

#endif // PDFFORDETAILEDPAGES_H
