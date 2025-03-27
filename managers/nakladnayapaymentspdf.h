#ifndef NAKLADNAYAPAYMENTSPDF_H
#define NAKLADNAYAPAYMENTSPDF_H

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


class NakladnayaPaymentsPDF
{
public:
    NakladnayaPaymentsPDF();
    QString createPaymentPDF(const QString &id,
                             const QString &client,
                             const QString &object,
                             const QString &apartment,
                             const QString &paymentType,
                             const QString &currency,
                             const QString &paidAmount,
                             const QString &description,
                             const QString &dateStr,
                             const QString roomsQuantity,
                             const QString number,
                             const QString floor,
                             const QString ploshad
                             );

    void showPdfViewer(const QString &pdfPath);  // Новый метод

private:
    QString buildHtml(const QString &id,
                      const QString &client,
                      const QString &object,
                      const QString &apartment,
                      const QString &paymentType,
                      const QString &currency,
                      const QString &paidAmount,
                      const QString &description,
                      const QString &dateStr,
                      const QString roomsQuantity,
                      const QString number,
                      const QString floor,
                      const QString ploshad
                      );


    QString getStyleSheet();
    QString getDesktopDir();
};

#endif // NAKLADNAYAPAYMENTSPDF_H
