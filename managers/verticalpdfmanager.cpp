#include "verticalpdfmanager.h"
#include "qapplication.h"

VerticalPDFmanager::VerticalPDFmanager() {}

QString VerticalPDFmanager::getStyleSheet()
{
    return R"S(
    * {
      -webkit-tap-highlight-color: transparent;
    }
    html, body, table, th, td {
      border: 1px solid;
      margin: 0;
      padding: 0;
      text-align: left;
    }
    h1, h2, p {
      text-align: center;
    }
    h1 {
      color: #007700;
    }
    table {
      margin: 0;
      border-collapse: collapse;
      border-spacing: 0;
    }

    table, th, td {
      border: 1px solid;
    }

    h1, h2, p, td {
      text-align: center;
    }

    h1 {
      color: #007700;
    }
    )S";
}

QString VerticalPDFmanager::getAppDir()
{
    return QCoreApplication::applicationDirPath();
}

QString VerticalPDFmanager::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void VerticalPDFmanager::createPDF(QString html, QString title)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDateTime time = QDateTime::currentDateTime();
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/eraCostruction_отчеты");
    if (!folder.exists())
    {
        folder.mkdir(appDir + "/eraCostruction_отчеты");
    }

    QString fileName = title + " " + time.toString("dd.MM.yyyy HH-mm-ss") + ".pdf";
    fileName.replace(" ", "_");

    QString filePath = appDir + "/eraCostruction_отчеты/" + fileName;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName(filePath);

    QTextDocument doc;

    doc.setDefaultStyleSheet(getStyleSheet());
    doc.setHtml(getHeader(time) + html + getFooter(time));
    doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());

    doc.print(&printer);

    QMimeData *mimeData = new QMimeData();
    mimeData->setUrls({QUrl::fromLocalFile(filePath)});

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData);

    QApplication::restoreOverrideCursor();

    QMessageBox popup;
    popup.setTextFormat(Qt::MarkdownText);
    popup.setText("Отчет сохранен в папке 'eraCostruction отчеты' на рабочем столе");
    popup.exec();
}

QString VerticalPDFmanager::getHeader(QDateTime time)
{
    return "<p>" + time.toString("dd.MM.yyyy HH:mm:ss") + "</p><h1 width=100% color='#007700'>ERA CONSTRUCTION</h1>";
}

QString VerticalPDFmanager::getFooter(QDateTime time)
{
    return "<br><p>ERA CONSTRUCTION</p><p>" + time.toString("dd.MM.yyyy HH:mm:ss") + "</p>";
}

void VerticalPDFmanager::ToVerticalPDF(QString title, QString dates, QAbstractItemModel *model)
{
    QString html = "<h2>" + title + "</h2><p>" + dates + "</p><br>";
    html += modelToVerticalHTML(model);
    createPDF(html, title + " " + dates);
}

QString VerticalPDFmanager::modelToVerticalHTML(QAbstractItemModel *model)
{
    QString html = "<table style='width: 100%;'>";

    for (int row = 0; row < model->rowCount(); ++row)
    {
        QString rowLabel = model->headerData(row, Qt::Vertical).toString();
        QString cellData = model->index(row, 0).data(Qt::DisplayRole).toString();

        html += "<tr><th>" + rowLabel + "</th><td>" + cellData + "</td></tr>";
    }

    html += "</table>";
    return html;
}
