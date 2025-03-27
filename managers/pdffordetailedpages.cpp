#include "pdfForDetailedPages.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QMessageBox>
#include <QPrinter>
#include <QTextDocument>
#include <QClipboard>
#include <QMimeData>
#include <QStandardPaths>
#include <QDebug>
#include <QUrl>

PDFForDetailedPages::PDFForDetailedPages() {}

QString PDFForDetailedPages::getStyleSheet()
{
    return
        R"S(
/*
reset css
*/
* {
  -webkit-tap-highlight-color: transparent;
}
*:focus {
  outline: none;
}
html,
body,
div,
span,
applet,
object,
iframe,
h1,
h2,
h3,
h4,
h5,
h6,
p,
blockquote,
pre,
a,
abbr,
acronym,
address,
big,
cite,
code,
del,
dfn,
em,
img,
ins,
kbd,
q,
s,
samp,
small,
strike,
strong,
sub,
sup,
tt,
var,
b,
u,
i,
dl,
dt,
dd,
ol,
ul,
li,
fieldset,
form,
label,
legend,
table,
caption,
tbody,
tfoot,
thead,
tr,
th,
td,
article,
aside,
canvas,
details,
embed,
figure,
figcaption,
footer,
header,
hgroup,
menu,
nav,
output,
ruby,
section,
summary,
time,
mark,
audio,
video {
  margin: 0;
  padding: 0;
  border: 0;
  vertical-align: baseline;
}
article,
aside,
details,
figcaption,
figure,
footer,
header,
hgroup,
menu,
nav,
section {
  display: block;
}
body,
#root {
  line-height: 1;
  overflow-wrap: anywhere;
  overflow-x: hidden;
}
ol,
ul {
  list-style: none;
}
blockquote,
q {
  quotes: none;
}
blockquote:before,
blockquote:after,
q:before,
q:after {
  content: "";
  content: none;
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

QString PDFForDetailedPages::getAppDir()
{
    return QCoreApplication::applicationDirPath();
}

QString PDFForDetailedPages::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

QString PDFForDetailedPages::getHeader()
{
    return "<h1 width='100%' style='color:#007700;'>ERA CONSTRUCTION</h1>";
}

QString PDFForDetailedPages::getFooter()
{
    return "<br><p>ERA CONSTRUCTION</p><p></p>";
}

void PDFForDetailedPages::createPDF(const QString &html, const QString &title)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDateTime time = QDateTime::currentDateTime();
    QString appDir = getDesktopDir();
    QDir folder(appDir + "/eraCostruction_отчеты");
    if (!folder.exists())
    {
        folder.mkdir(appDir + "/eraCostruction_отчеты");
    }

    QString fileName = title + ".pdf";
    fileName.replace(" ", "_");
    fileName.replace("\"", "'");

    QString filePath = appDir + "/eraCostruction_отчеты/" + fileName;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName(filePath);

    qDebug() << "Output file:" << printer.outputFileName();

    QTextDocument doc;
    doc.setDefaultStyleSheet(getStyleSheet());
    doc.setHtml(getHeader() + html + getFooter());
    doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());

    doc.print(&printer);

    QApplication::restoreOverrideCursor();

    if (QFile::exists(filePath)) {
        qDebug() << "PDF успешно создан по пути:" << filePath;
        QMimeData *mimeData = new QMimeData();
        mimeData->setUrls({QUrl::fromLocalFile(filePath)});
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setMimeData(mimeData);
        QMessageBox::information(nullptr, "PDF Сохранен", "Отчет сохранен в папке 'eraCostruction_отчеты' на рабочем столе.");
    } else {
        qDebug() << "Не удалось создать PDF по пути:" << filePath;
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось сохранить PDF файл.");
    }
}

QString PDFForDetailedPages::modelToHTML(QAbstractItemModel *model, int start)
{
    QString html;
    html += "<table style='margin: 0;' width='100%'><tr>";

    // Добавляем нумерацию, если нужно
    if (start == 1)
    {
        html += "<th>#</th>";
    }

    // Заголовки столбцов
    for (int i = start; i < model->columnCount(); i++)
    {
        html += "<th>" + model->headerData(i, Qt::Horizontal).toString() + "</th>";
    }
    html += "</tr>";

    // Строки данных
    for (int i = 0; i < model->rowCount(); i++)
    {
        html += "<tr>";
        if (start == 1)
        {
            html += "<td>" + QString::number(i + 1) + "</td>";
        }

        for (int j = start; j < model->columnCount(); j++)
        {
            QString cellData = model->index(i, j).data(Qt::DisplayRole).toString();
            QString header = model->headerData(j, Qt::Horizontal).toString();

            // Пример дополнительной стилизации по названию заголовка
            if (header == "Инвестору" && start != 1)
            {
                html += "<td style='border: 1px solid black; color:#007700;'>" + cellData + "</td>";
            }
            else
            {
                html += "<td>" + cellData + "</td>";
            }
        }
        html += "</tr>";
    }
    html += "</table>";
    return html;
}

QString PDFForDetailedPages::additionalDataToHTML(const QMap<QString, QString> &additionalData)
{
    QString html;
    if (!additionalData.isEmpty())
    {
        html += "<h3>Дополнительная информация:</h3>";
        html += "<table style='margin: 0; width: 100%;'>";
        QMapIterator<QString, QString> it(additionalData);
        while (it.hasNext())
        {
            it.next();
            html += "<tr>";
            html += "<td><strong>" + it.key() + "</strong></td>";
            html += "<td>" + it.value() + "</td>";
            html += "</tr>";
        }
        html += "</table><br>";
    }
    return html;
}

void PDFForDetailedPages::ToPDF(const QString &title, const QString &dates, const QMap<QString, QString> &additionalData,
                                const QList<QAbstractItemModel *> &models, int start)
{
    QString html = "<h2>" + title + "</h2>";
    html += "<p>" + dates + "</p><br>";
    html += additionalDataToHTML(additionalData);
    for (int i = 0; i < models.size(); i++)
    {
        html += modelToHTML(models[i], (start != 0 && i == 0) ? 1 : 0);
    }
    createPDF(html, title + " " + dates);
}

void PDFForDetailedPages::exportToPDF(const QString &title, const QString &dates, const QMap<QString, QString> &additionalData,
                                      const QList<QAbstractItemModel *> &models, int start)
{
    ToPDF(title, dates, additionalData, models, start);
}
