#include "nakladnayaexpensespdf.h"
#include "qlabel.h"
#include "qpainter.h"

NakladnayaExpensesPDF::NakladnayaExpensesPDF() {}

// Изменённая версия функции создания PDF с дополнительным параметром "описание"
QString NakladnayaPDF::createNakladnaya(
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
    const QString &opisanie    // новый параметр для описания
    )
{
    // Формируем имя PDF-файла с датой/временем
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString fileName = "Nakladnaya_" + currentDateTime.toString("dd.MM.yyyy_HH-mm-ss") + ".pdf";

    // Папка "отчеты" на рабочем столе
    QString desktopDir = getDesktopDir();
    QDir folder(desktopDir + "/отчеты");
    if(!folder.exists()) {
        folder.mkdir(desktopDir + "/отчеты");
    }

    QString filePath = folder.path() + "/" + fileName;

    // Настраиваем принтер и вывод в PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName(filePath);

    // Текстовый документ для формирования HTML
    QTextDocument doc;
    doc.setDefaultStyleSheet(getStyleSheet());
    // Загрузка логотипа
    QPixmap logoPixmap(":/logo.png");
    if (!logoPixmap.isNull()) {
        // Масштабируем логотип до 50x50, но в HTML отображаем как 20x20
        logoPixmap = logoPixmap.scaled(130, 130, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        doc.addResource(QTextDocument::ImageResource, QUrl("logo"), logoPixmap);
    } else {
        qDebug() << "Ошибка: Логотип не загружен!";
    }
    // Передаём новый параметр "описание" в buildHtml
    doc.setHtml(buildHtml(id,
                          otpravitel,
                          poluchatel,
                          objectName,
                          autoGosNumber,
                          voditelFIO,
                          timeStr,
                          markBetona,
                          edIzm,
                          kolvo,
                          opisanie));
    doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());

    // Печатаем в PDF
    doc.print(&printer);

    // Проверка результата
    if (QFile::exists(filePath)) {
        qDebug() << "PDF успешно создан: " << filePath;
        QMimeData *mimeData = new QMimeData();
        mimeData->setUrls({QUrl::fromLocalFile(filePath)});
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setMimeData(mimeData);
    }
    return filePath;
}

// Функция формирования HTML
// Базовый размер шрифта увеличен до 12px, логотип отображается 20x20,
// а заголовок "НАКЛАДНАЯ № _________" имеет размер 8px.
// Границы таблицы с бетоном заданы явно в inline-стилях.
QString NakladnayaPDF::buildHtml(
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
    const QString &opisanie    // новый параметр для описания
    )
{
    QString html;

    for (int i = 0; i < 3; i++) {  // Создаём 3 копии
        html += R"(
    <div style="font-family: Arial, sans-serif; font-size: 12px; width: 100%; border: 1px solid black; padding: 10px; margin-bottom: 5px;">
        <!-- Шапка -->
        <table width="100%" style="border-collapse: collapse; margin-bottom: 5px;">
            <tr>
                <td style="text-align: left; vertical-align: middle; width: 70%;">
                    <img src="logo" alt="Логотип" style="width: 20px; height: 20px; vertical-align: middle;">
                </td>
                <td style="text-align: right; font-weight: bold; vertical-align: middle;">
                    ДАТА: )" + QDate::currentDate().toString("dd/MM/yyyy") + R"(
                </td>
            </tr>
        </table>

        <!-- Накладная (уменьшенный заголовок) -->
        <h4 style="text-align: center; font-size: 8px; margin: 0;">НАКЛАДНАЯ № _________</h4>

        <!-- Основные данные -->
        <p style="margin: 0;"><strong>ОТПРАВИТЕЛЬ:</strong> )" + otpravitel + R"(</p>

        <!-- Получатель и Объект на одной строке -->
        <table width="100%" style="border-collapse: collapse; margin: 0;">
            <tr>
                <td style="text-align: left; vertical-align: middle; width: 50%;">
                    <strong>ПОЛУЧАТЕЛЬ:</strong> )" + poluchatel + R"(</td>
                <td style="text-align: right; vertical-align: middle; width: 50%;">
                    Объект: )" + objectName + R"(</td>
            </tr>
        </table>

        <p style="margin: 0;"><strong>АВТО/ГОС №:</strong> )" + autoGosNumber + R"(</p>

        <!-- Водитель и Время на одной строке -->
        <table width="100%" style="border-collapse: collapse; margin: 0;">
            <tr>
                <td style="text-align: left; vertical-align: middle; width: 50%;">
                    <strong>ВОДИТЕЛЬ:</strong> )" + voditelFIO + R"(</td>
                <td style="text-align: right; vertical-align: middle; width: 50%;">
                    Время: )" + timeStr + R"(</td>
            </tr>
        </table>

        <!-- Таблица с бетоном с границами -->
        <table width="100%" cellspacing="0" cellpadding="3" style="border-collapse: collapse; margin: 0; text-align: center; border: 1px solid black;">
            <tr>
                <th style="border: 1px solid black;">№</th>
                <th style="border: 1px solid black;">Марка Бетона</th>
                <th style="border: 1px solid black;">Ед. изм.</th>
                <th style="border: 1px solid black;">Кол-во</th>
            </tr>
            <tr>
                <td style="border: 1px solid black;">)" + id + R"(</td>
                <td style="border: 1px solid black;">)" + markBetona + R"(</td>
                <td style="border: 1px solid black;">)" + edIzm + R"(</td>
                <td style="border: 1px solid black;">)" + kolvo + R"(</td>
            </tr>
        </table>

        <!-- Подписи -->
        <table width="100%" style="border-collapse: collapse; margin: 0;">
            <tr>
                <td style="text-align: left; vertical-align: middle; width: 50%;">
                    <strong>Отпустил:</strong> _________</td>
                <td style="text-align: right; vertical-align: middle; width: 50%;">
                    <strong>Водитель:</strong> _________</td>
            </tr>
        </table>

        <table width="100%" style="border-collapse: collapse; margin: 0;">
            <tr>
                <td style="text-align: left; vertical-align: middle; width: 50%;">
                    <strong>Получил:</strong> _________</td>
                <td style="text-align: right; vertical-align: middle; width: 50%;">
                    <strong><p style="margin: 0;">)" + opisanie + R"(</p></td>
            </tr>
        </table>
    </div>
    )";

        // Примечание (без отступов)
        html += R"(<p style="font-size: 12px; font-style: italic; margin: 0;">Примечание: ожидание автомиксера на объекте в первый час - бесплатно. А более оплачивается 1 час - 1000 сом</p>)";

        if (i < 2) {  // Разделитель между копиями
            html += R"(<p style="text-align: center; font-weight: bold; font-size: 12px; margin: 0;">- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -</p>)";
        }
    }

    return html;
}



QString NakladnayaPDF::getStyleSheet()
{
    // Стили для PDF: отступы и поля обнулены
    return R"(
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}
h1, h2, p, table, th, td {
    margin: 0;
    padding: 0;
}
)";
}

QString NakladnayaPDF::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void NakladnayaPDF::showPdfViewer(const QString &pdfPath) {
    QDialog *viewerDialog = new QDialog();
    viewerDialog->setWindowTitle("Просмотр Накладной");
    viewerDialog->setStyleSheet("background-color: white;");

    QVBoxLayout *layout = new QVBoxLayout(viewerDialog);

    QPdfDocument *pdfDocument = new QPdfDocument();
    pdfDocument->load(pdfPath);

    const int pageCount = pdfDocument->pageCount();

    QWidget *container = new QWidget();
    container->setStyleSheet("background-color: white;");
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(5,5,5,5);

    const int targetWidth = 800;

    for (int i = 0; i < pageCount; ++i) {
        QSizeF originalSize = pdfDocument->pagePointSize(i);
        qreal scale = targetWidth / originalSize.width();
        QSize renderSize = (originalSize * scale).toSize();

        QImage pageImage = pdfDocument->render(i, renderSize);

        QLabel *pageLabel = new QLabel();
        pageLabel->setPixmap(QPixmap::fromImage(pageImage));
        pageLabel->setAlignment(Qt::AlignCenter);
        pageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        pageLabel->setFixedSize(renderSize);

        containerLayout->addWidget(pageLabel);
    }
    container->setLayout(containerLayout);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(container);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setStyleSheet("background-color: white;");

    QPushButton *printButton = new QPushButton("Печать");
    printButton->setStyleSheet("QPushButton { background-color: lightgray; color: black; border: 1px solid gray; padding: 5px; }");

    QObject::connect(printButton, &QPushButton::clicked, [pdfDocument]() {
        QPrinter printer;
        QPrintDialog dialog(&printer);
        if (dialog.exec() == QDialog::Accepted) {
            QPainter painter(&printer);
            QSizeF pageSizeF = printer.pageRect(QPrinter::DevicePixel).size();
            QSize pageSize = pageSizeF.toSize();
            QImage image1 = pdfDocument->render(0, pageSize);
            painter.drawImage(0, 0, image1);
            if (pdfDocument->pageCount() > 1) {
                printer.newPage();
                QImage image2 = pdfDocument->render(1, pageSize);
                painter.drawImage(0, 0, image2);
            }
            painter.end();
        }
    });

    layout->addWidget(scrollArea, 1);
    layout->addWidget(printButton, 0);

    viewerDialog->setLayout(layout);
    viewerDialog->resize(850, 600);
    viewerDialog->exec();

    delete pdfDocument;
}
