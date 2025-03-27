#ifndef NAKLADNAYAEXPENSEPDF_H
#define NAKLADNAYAEXPENSEPDF_H

#include <QObject>
#include <QPrinter>
#include <QTextDocument>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>
#include <QMessageBox>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>
#include <QFile>

#include <QPdfView>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPrintDialog>
#include <QPdfDocument>
#include <QScrollArea>
#include <QPdfPageNavigator>

class NakladnayaExpensePDF
{
public:
    NakladnayaExpensePDF();
    QString createExpensePDF(const QString &id,
                             const QString &type,
                             const QString &whoWhat,
                             const QString &paymentType,
                             const QString &currency,
                             const QString &amount,
                             const QString &description,
                             const QString &dateStr);

    void showPdfViewer(const QString &pdfPath);

private:
    QString buildHtml(const QString &id,
                      const QString &type,
                      const QString &whoWhat,
                      const QString &paymentType,
                      const QString &currency,
                      const QString &amount,
                      const QString &description,
                      const QString &dateStr);

    QString getStyleSheet();
    QString getDesktopDir();
};

#endif // NAKLADNAYAEXPENSEPDF_H
