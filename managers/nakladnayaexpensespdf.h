#ifndef NAKLADNAYAEXPENSESPDF_H
#define NAKLADNAYAEXPENSESPDF_H

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
#include <QPdfView>
#include <QPdfDocument>
#include <QPdfPageNavigator>

class NakladnayaExpensesPDF
{
public:
    NakladnayaExpensesPDF();

    QString createNakladnaya(
        const QString &id,
        const QString &otpravitel,
        const QString &poluchatel,
        const QString &objectName,
        const QString &autoGosNumber,
        const QString &voditelFIO,
        const QString &timeStr,
        const QString &markBetona,
        const QString &edIzm,
        const QString &kolvo,
        const QString &opisanie
        );

    void showPdfViewer(const QString &pdfPath);  // Новый метод

private:
    QString buildHtml(
        const QString &id,
        const QString &otpravitel,
        const QString &poluchatel,
        const QString &objectName,
        const QString &autoGosNumber,
        const QString &voditelFIO,
        const QString &timeStr,
        const QString &markBetona,
        const QString &edIzm,
        const QString &kolvo,
        const QString &opisanie
        );

    QString getStyleSheet();
    QString getDesktopDir();
};

#endif // NAKLADNAYAEXPENSESPDF_H
