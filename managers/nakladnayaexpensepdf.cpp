#include "nakladnayaexpensepdf.h"
#include "qlabel.h"
#include "qpainter.h"

NakladnayaExpensePDF::NakladnayaExpensePDF() {}





// Изменённая версия функции создания PDF с дополнительным параметром "описание"
QString NakladnayaExpensePDF::createExpensePDF(
    const QString &id,
    const QString &type,
    const QString &whoWhat,
    const QString &paymentType,
    const QString &currency,
    const QString &amount,
    const QString &description,
    const QString &dateStr
    )
{
    // Формируем имя PDF-файла с датой/временем
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString fileName = "Накладная_Расхода" + currentDateTime.toString("dd.MM.yyyy_HH-mm-ss") + ".pdf";

    // Папка "отчеты" на рабочем столе
    QString desktopDir = getDesktopDir();
    QDir folder(desktopDir + "/eraCostruction_отчеты");
    if(!folder.exists()) {
        folder.mkdir(desktopDir + "/eraCostruction_отчеты");
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


    doc.setHtml(buildHtml(id,
                          type,
                          whoWhat,
                          paymentType,
                          currency,
                          amount,
                          description,
                          dateStr));
    doc.setPageSize(printer.pageRect(QPrinter::DevicePixel).size());


    doc.print(&printer);

    if (QFile::exists(filePath)) {
        qDebug() << "PDF успешно создан: " << filePath;
        QMimeData *mimeData = new QMimeData();
        mimeData->setUrls({QUrl::fromLocalFile(filePath)});
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setMimeData(mimeData);
    }
    return filePath;
}



QString NakladnayaExpensePDF::buildHtml(
    const QString &id,
    const QString &type,
    const QString &whoWhat,
    const QString &paymentType,
    const QString &currency,
    const QString &amount,
    const QString &description,
    const QString &dateStr
    )
{
    QString html;

    /*for (int i = 0; i < 3; i++) { */ // Создаём 3 копии
    html += R"(
    <div style="font-family: Arial, sans-serif; font-size: 12px; width: 100%; position: relative;">

        <!-- Блок в правом верхнем углу -->
        <div style="position: absolute; top: 0; right: 0; text-align: right; font-size: 10px;">
            <div style="margin-right: 48px;">Унифицированная форма КО-2</div>
            <div style="margin-right: 58px;">Утверждена постановлением</div>
            <div>Нацстаткомитета Кыргызской Республики</div>
            <div style="margin-right: 120px;">от 07.04.03 № 4</div>
        </div>

        <br>

        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 60%; padding-top: 5px; padding-left: 5px; padding-right: 5px; vertical-align: top;">
                    <div style="font-weight: bold; text-align: center; font-size: 15px;">ОсОО "ЭРА Констракшн"</div>
                    <div style="font-weight: bold; margin-bottom: 5px;">_____________________________________________________</div>
                    <div style="border-bottom: 1px solid black; padding-bottom: 10px; font-size: 10px; text-align: center;">организация</div>
                    <div style="height: 10px;"></div> <!-- Пустое место -->
                    <div style="font-weight: bold;">_____________________________________________________</div>
                    <div style="border-bottom: 1px solid black; padding-bottom: 5px; font-size: 10px; text-align: center;">подразделение</div>
                </td>

                <!-- Правая часть -->
                <td style="width: 40%; padding: 5px; vertical-align: top; text-align: right;">
                    <div style="font-size: 12px; margin-right: 22px;">Код</div>
                    <table style="width: 100%; border-collapse: collapse; border: 1px solid black;">
                        <tr>
                            <td style="padding: 5px; font-size: 10px; text-align: right;">Форма по ГКУД</td>
                            <td style="border-left: 1px solid black; padding: 5px; text-align: center; font-weight: bold;">310001</td>
                        </tr>
                        <tr>
                            <td style="padding: 5px; font-size: 10px; text-align: right;">по ОКПО</td>
                            <td style="border-left: 1px solid black; padding: 5px; text-align: center; font-weight: bold;">29333797</td>
                        </tr>
                    </table>

                    <!-- Квадрат с разделениями -->
                    <div style="margin-top: 10px; border: 1px solid black; width: 100%; height: 50px; display: flex; flex-direction: row;">
                        <div style="border-right: 1px solid black; width: 33%; height: 100%;"></div>
                        <div style="border-right: 1px solid black; width: 33%; height: 100%;"></div>
                        <div style="width: 34%; height: 100%;"></div>
                    </div>
                </td>
            </tr>


            <!-- Название документа -->
            <tr>
                <td style="padding: 5px; text-align: center; font-weight: bold; font-size: 15px;">
                    РАСХОДНЫЙ КАССОВЫЙ ОРДЕР
                </td>
                <td style="padding: 5px;">
                    <table style="width: 100%; border-collapse: collapse;">
                        <tr>
                            <th style="border: 1px solid black; padding: 2px; font-size: 10px;">Номер документа</th>
                            <th style="border: 1px solid black; padding: 2px; font-size: 10px;">Дата составления</th>
                        </tr>
                        <tr>
                            <td style="border: 1px solid black; padding: 2px; text-align: center;">17/01</td>
                            <td style="border: 1px solid black; padding: 2px; text-align: center;">)" + dateStr + R"(</td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>

        <table style="width: 90%; border-collapse: collapse; border: 1px solid black; margin-top: 10px;">
            <!-- Заголовки -->
            <tr>
                <td colspan="3" style="border: 1px solid black; padding: 5px; text-align: center; width: 40%;">Дебет</td>
                <td rowspan="2" style="border: 1px solid black; padding: 5px; vertical-align: middle; width: 20%;">Кредит</td>
                <td rowspan="2" style="border: 1px solid black; padding: 5px; vertical-align: middle; width: 25%;">Сумма, )" + currency + R"(</td>
                <td rowspan="2" style="border: 1px solid black; padding: 5px; vertical-align: middle; width: 15%;">Код целевого назначения</td>
            </tr>

            <!-- Подзаголовки под "Кредит" -->
            <tr>
                <td style="border: 1px solid black; padding: 5px; text-align: center; width: 50%;">Код структурного подразделения</td>
                <td style="border: 1px solid black; padding: 5px; text-align: center; width: 25%;">корр.счет, субсчет</td>
                <td style="border: 1px solid black; padding: 5px; text-align: center; width: 25%;">Код ан.учета</td>
            </tr>

            <!-- Данные -->
            <tr>
                <td style="border: 1px solid black; padding: 10px; text-align: center;"></td>
                <td style="border: 1px solid black; padding: 10px;">3110</td>
                <td style="border: 1px solid black; padding: 10px;"></td>
                <td style="border: 1px solid black; padding: 10px;">1110</td>
                <td style="border: 1px solid black; padding: 10px;">)" + amount + R"(</td>
                <td style="border: 1px solid black; padding: 10px;"></td>
            </tr>
        </table>

        <br>

        <div style="display: flex; align-items: flex-start; width: 100%; margin: 5px 0;">
            <!-- Левая часть -->
            <div style="width: 40%; padding: 5px; text-align: left; margin-left: 6px;">
                Выдать:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;______________________________________________________________________
            </div>

            <!-- Правая часть -->
            <div style="width: 60%; padding: 5px;">
                <div style="text-align: center; font-size: 10px; margin-top: -2px;">
                    фамилия, имя, отчество
                </div>
            </div>
        </div>

        <table style="width: 100%; border-collapse: collapse; margin-top: 5px;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 40%; padding: 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left; margin-top: 5px;">
                        <div>Основание:&nbsp;&nbsp;&nbsp;&nbsp;</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 60%; padding: 5px; vertical-align: top;">
                    <div>)" + type + R"(</div>
                    <div style="text-align: center; font-size: 10px; margin-top: -2px;"> </div>
                </td>
            </tr>
        </table>
        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div>Сумма:</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;______________________________________________________________________</div>
                    <div style="text-align: center; font-size: 10px; margin-top: -2px;">прописью</div>
                </td>
            </tr>
        </table>
        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px 2px 5px 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div>Приложение:</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium; margin-right: 2px;">______________________________________________________________________</div>
                    <div style="text-align: center; font-size: 10px; margin-top: -2px;"> </div>
                </td>
            </tr>
        </table>
        <table style="width: 100%; border-collapse: collapse; margin-top: 5px;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div style="font-weight: bold;">Руководитель</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium; margin-left: 2px;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;______________________________________________________________</div>
                    <div style="text-align: center; font-size: 10px">&nbsp;&nbsp;&nbsp;должность&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;расшифровка подписи</div>
                </td>
            </tr>
        </table>
        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div style="font-weight: bold;">Главный бухгалтер</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium;">&nbsp;&nbsp;_____________________&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_____________________________________</div>
                    <div style="text-align: left; font-size: 10px">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;расшифровка подписи</div>
                </td>
            </tr>
        </table>
        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px 2px 5px 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div>Получил:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium;">_____________________________________________________________________</div>
                    <div style="text-align: center; font-size: 10px; margin-top: -2px;"> </div>
                </td>
            </tr>
        </table>
        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div>____________________</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium;">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Подпись&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;____________________</div>
                    <div style="text-align: center; font-size: 10px; margin-top: -2px;"> </div>
                </td>
            </tr>
        </table>
        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px 2px 5px 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div>По:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium;">_____________________________________________________________________</div>
                    <div style="text-align: center; font-size: 10px">наименование, номер, дата и место выдачи документа</div>
                </td>
            </tr>
        </table>

        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div style="font-weight: medium;">__________________________________________________________________________________</div>
                        <div style="text-align: center; font-size: 10px; margin-top: -2px;">удостоверяющего личность получателя</div>
                        <div style="text-align: center; font-size: 10px; margin-top: -2px;"> </div>
                    </div>
                </td>
            </tr>
        </table>

        <table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 0 5px 5px 5px; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div style="font-weight: bold;">Выдал кассир&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 0 5px 5px 5px; vertical-align: top;">
                    <div style="font-weight: medium;">&nbsp;&nbsp;_____________________&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_____________________________________</div>
                    <div style="text-align: left; font-size: 10px">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;расшифровка подписи</div>
                </td>
            </tr>
        </table>
    </div>
    )";

    return html;
}



QString NakladnayaExpensePDF::getStyleSheet()
{
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

QString NakladnayaExpensePDF::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void NakladnayaExpensePDF::showPdfViewer(const QString &pdfPath) {
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
