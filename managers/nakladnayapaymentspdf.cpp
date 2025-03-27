#include "nakladnayapaymentspdf.h"
#include "qlabel.h"
#include "qpainter.h"

NakladnayaPaymentsPDF::NakladnayaPaymentsPDF()
{
}

QString NakladnayaPaymentsPDF::createPaymentPDF(
    const QString &id,
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
    )
{
    // Generate a unique file name with the current date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString fileName = "Накладная_Расходы" + currentDateTime.toString("dd.MM.yyyy_HH-mm-ss") + ".pdf";

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
                          client,
                          object,
                          apartment,
                          paymentType,
                          currency,
                          paidAmount,
                          description,
                          dateStr,
                          roomsQuantity,
                          number,
                          floor,
                          ploshad));
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


















// QString NakladnayaPaymentsPDF::buildHtml(
//     const QString &id,
//     const QString &client,
//     const QString &object,
//     const QString &apartment,
//     const QString &paymentType,
//     const QString &currency,
//     const QString &paidAmount,
//     const QString &description,
//     const QString &dateStr
//     )
// {
//     // -- Левая часть (ваш исходный код), обёрнут для удобства в переменную leftSide
//     QString leftSide = R"(
//     <div style="position: relative;">
//         <!-- Блок в правом верхнем углу -->
//         <div style="position: absolute; top: 0; right: 0; text-align: right;">
//             <span style="font-size: 8px; display: inline-block;">
//                 Унифицированная форма КО-1<br>
//                 Утверждена постановлением<br>
//                 Нацстаткомитета Кыргызской<br>
//                 Республики от 07.04.03 № 4
//             </span>
//         </div>

//         <table style="width: 100%; border-collapse: collapse;">
//             <!-- Заголовок документа -->
//             <tr>
//                 <!-- Левая часть -->
//                 <td style="width: 60%; padding: 3px; vertical-align: top;">
//                     <div style="font-weight: bold;">Общество с ограниченной ответственностью </div>
//                     <div style="font-weight: bold; text-align: center;">"ЭРА Констракшн"</div>
//                     <div style="font-weight: bold; margin-top: -5px;">_____________________________________________</div>
//                     <div style="border-bottom: 1px solid black; font-size: 10px; text-align: center;">организация</div>
//                     <div style="height: 10px;"></div> <!-- Уменьшенное пустое место -->
//                     <div style="font-weight: bold;">_____________________________________________</div>
//                     <div style="border-bottom: 1px solid black; font-size: 10px; text-align: center;">подразделение</div>
//                 </td>

//                 <!-- Правая часть -->
//                 <td style="width: 40%; padding: 3px; vertical-align: top;">
//                     <table style="width: 100%; border-collapse: collapse; border: 1px solid black;">
//                         <tr>
//                             <td style="padding: 3px; font-size: 10px;">Форма по ГКУД</td>
//                             <td style="border-left: 1px solid black; padding: 3px; text-align: center;">0310001</td>
//                         </tr>
//                         <tr>
//                             <td style="padding: 3px; font-size: 10px;">по ОКПО</td>
//                             <td style="border-left: 1px solid black; padding: 3px; text-align: center;">29333797</td>
//                         </tr>
//                     </table>

//                     <!-- Квадрат с разделениями -->
//                     <div style="margin-top: 5px; border: 1px solid black; width: 100%; height: 40px; display: flex;">
//                         <div style="border-right: 1px solid black; width: 33%;"></div>
//                         <div style="border-right: 1px solid black; width: 33%;"></div>
//                         <div style="width: 34%;"></div>
//                     </div>
//                 </td>
//             </tr>

//             <!-- Блок с приходным кассовым ордером -->
//             <tr style="vertical-align: bottom;">
//                 <!-- Левая часть с надписью -->
//                 <td style="width: 50%; padding: 3px; text-align: right; position: relative;">
//                     <div style="position: absolute; bottom: 0; right: 5px; font-weight: bold;">
//                         ПРИХОДНЫЙ КАССОВЫЙ ОРДЕР
//                     </div>
//                 </td>

//                 <!-- Правая часть с таблицей -->
//                 <td style="width: 50%; padding: 3px;">
//                     <table style="width: 100%; border-collapse: collapse; border: 1px solid black;">
//                         <tr>
//                             <td style="border: 1px solid black; padding: 3px; text-align: center;">Номер документа</td>
//                             <td style="border: 1px solid black; padding: 3px; text-align: center;">Дата составления</td>
//                         </tr>
//                         <tr>
//                             <td style="border: 1px solid black; padding: 5px; text-align: center;">)" + id + R"(</td>
//                             <td style="border: 1px solid black; padding: 5px; text-align: center;">)" + dateStr + R"(</td>
//                         </tr>
//                     </table>
//                 </td>
//             </tr>
//         </table>

//         <!-- Таблица с дебетом, кредитом и прочими полями -->
//         <table style="width: 100%; border-collapse: collapse; border: 1px solid black; margin-top: 10px;">
//             <!-- Заголовки -->
//             <tr>
//                 <td rowspan="2" style="border: 1px solid black; padding: 5px; text-align: center; width: 15%;">Дебет</td>
//                 <td colspan="4" style="border: 1px solid black; padding: 5px; text-align: center; width: 35%;">Кредит</td>
//                 <td rowspan="2" style="border: 1px solid black; padding: 5px; text-align: center; width: 15%;">Сумма кырг. сомы</td>
//                 <td rowspan="2" style="border: 1px solid black; padding: 5px; text-align: center; width: 15%;">Код целевого назначения</td>
//                 <td rowspan="2" style="border: 1px solid black; padding: 5px; text-align: center; width: 10%;"></td>
//             </tr>

//             <!-- Подзаголовки под "Кредит" -->
//             <tr>
//                 <td style="border: 1px solid black; padding: 5px; text-align: center;">Код структурного подразделения</td>
//                 <td style="border: 1px solid black; padding: 5px; text-align: center;">Корреспондирующий счет, субсчет</td>
//                 <td style="border: 1px solid black; padding: 5px; text-align: center;">Код аналитического учета</td>
//                 <td style="border: 1px solid black; padding: 5px;"></td> <!-- Пустая ячейка -->
//             </tr>

//             <!-- Данные -->
//             <tr>
//                 <td style="border: 1px solid black; padding: 10px; text-align: center;">1120</td>
//                 <td style="border: 1px solid black; padding: 10px;"></td>
//                 <td style="border: 1px solid black; padding: 10px;"></td>
//                 <td style="border: 1px solid black; padding: 10px; text-align: center;">3210</td>
//                 <td style="border: 1px solid black; padding: 10px;"></td>
//                 <td style="border: 1px solid black; padding: 10px;"></td>
//                 <td style="border: 1px solid black; padding: 10px;"></td>
//                 <td style="border: 1px solid black; padding: 10px;"></td>
//             </tr>
//         </table>

//         <!-- Принято от -->
//         <div style="margin-bottom: 10px; margin-top: 10px;">
//             <span>Принято от</span>
//             <span style="display: inline-block; width: 80%; text-align: center; position: relative;">
//                 <span style="position: absolute; top: -15px;">)" + client + R"(</span>
//                 <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
//             </span>
//         </div>

//         <!-- Основание -->
//         <div style="margin-bottom: 10px;">
//             <span>Основание</span>
//             <span style="display: inline-block; width: 80%; text-align: center; position: relative;">
//                 <span style="position: absolute; top: -15px;">)" + description + R"(</span>
//                 <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
//             </span>
//         </div>

//         <!-- Сумма -->
//         <div>
//             <span>Сумма</span>
//             <span style="display: inline-block; width: 80%; text-align: center; position: relative;">
//                 <span style="position: absolute; top: -15px;">)" + paidAmount + " " + currency + R"(</span>
//                 <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
//             </span>
//         </div>
//     </div>
//     )";

//     // -- Правая часть (квитанция). Здесь можете добавить любые поля по аналогии с левой:
//     QString rightSide = R"(
//     <div>
//         <div style="font-weight: bold;">Общество с ограниченной ответственностью</div>
//         <div style="font-weight: bold; text-align: center;">"ЭРА Констракшн"</div>
//         <div style="font-weight: bold; margin-top: -5px;">_____________________________________________</div>
//         <div style="border-bottom: 1px solid black; font-size: 10px; text-align: center;">организация</div>

//         <!-- Заголовок "КВИТАНЦИЯ" -->
//         <div style="text-align: center; font-size: 12px; font-weight: bold; margin-top: 10px;">
//             КВИТАНЦИЯ
//         </div>

//         <!-- Номер документа, дата -->
//         <table style="width: 100%; border-collapse: collapse; margin-top: 5px;">
//             <tr>
//                 <td style="width: 50%; font-size: 10px;">№ документа:</td>
//                 <td style="width: 50%; text-align: right;">)" + id + R"(</td>
//             </tr>
//             <tr>
//                 <td style="font-size: 10px;">Дата:</td>
//                 <td style="text-align: right;">)" + dateStr + R"(</td>
//             </tr>
//         </table>

//         <!-- Принято от -->
//         <div style="margin-top: 10px;">
//             <span>Принято от: </span>
//             <span style="border-bottom: 1px solid black;">)" + client + R"(</span>
//         </div>

//         <!-- Основание -->
//         <div style="margin-top: 10px;">
//             <span>Основание: </span>
//             <span style="border-bottom: 1px solid black;">)" + description + R"(</span>
//         </div>

//         <!-- Сумма -->
//         <div style="margin-top: 10px;">
//             <span>Сумма: </span>
//             <span style="border-bottom: 1px solid black;">)" + paidAmount + " " + currency + R"(</span>
//         </div>

//         <!-- Подписи (пример) -->
//         <div style="margin-top: 15px; text-align: right;">
//             <div style="margin-bottom: 5px;">Главный бухгалтер _________</div>
//             <div>Кассир _________</div>
//         </div>
//     </div>
//     )";

//     // -- Формируем общий HTML: таблица на 2 колонки, между которыми вертикальная пунктирная линия
//     QString html = R"(
//     <div style="font-family: Arial, sans-serif; font-size: 12px; width: 100%; position: relative;">
//         <table style="width: 100%; border-collapse: collapse;">
//             <tr>
//                 <!-- Левая часть -->
//                 <td style="width: 50%; vertical-align: top; border-right: 2px dashed black; padding-right: 10px;">
//                     )" + leftSide + R"(
//                 </td>

//                 <!-- Правая часть -->
//                 <td style="width: 50%; vertical-align: top; padding-left: 10px;">
//                     )" + rightSide + R"(
//                 </td>
//             </tr>
//         </table>
//     </div>
//     )";

//     return html;
// }







QString NakladnayaPaymentsPDF::buildHtml(
    const QString &id,
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
    )
{

    QString leftSide = R"(
    <div style="font-family: Arial, sans-serif; font-size: 8px; width: 70%; position: relative;">
        <!-- Блок в правом верхнем углу -->
                <div style="position: absolute; top: 0; right: 0; text-align: right;">
                    <span style="font-size: 7px; display: inline-block;">
                        Унифицированная форма КО-1<br>
                        Утверждена постановлением<br>
                        Нацстаткомитета Кыргызской<br>
                        Республики от 07.04.03 № 4
                    </span>
                </div>

<table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 70%; padding: 3px; vertical-align: top;">
                    <div style="font-weight: bold;text-align:center;font-size: 8px;">Общество с ограниченной ответственностью </div>
                    <div style="font-weight: bold; text-align: center;font-size: 8px;">"ЭРА Констракшн"</div>
                    <div style="font-weight: bold; margin-top: -5px;">————————————</div>
                    <div style="border-bottom: 1px solid black; font-size: 7px; text-align: center;">организация</div>
                    <div style="height: 10px;"></div> <!-- Уменьшенное пустое место -->
                    <div style="font-weight: bold;">_______________________</div>
                    <div style="border-bottom: 1px solid black; font-size: 7px; text-align: center;">подразделение</div>
                </td>

                <!-- Правая часть -->
                <td style="width: 30%; padding: 3px; vertical-align: top;text-align:right;">
                    <table style="width: 80%; border-collapse: collapse; border: 1px solid black;">
                        <tr>
                            <td style="padding: 3px; font-size: 7px;">Форма по ПКУД</td>
                            <td style="border-left: 1px solid black; padding: 3px; text-align: center;font-size: 7px;">0310001</td>
                        </tr>
                        <tr>
                            <td style="padding: 3px; font-size: 7px;">по ОКПО</td>
                            <td style="border-left: 1px solid black; padding: 3px; text-align: center;font-size: 7px;">29333797</td>
                        </tr>
                    </table>

                    <!-- Квадрат с разделениями -->
                    <div style="margin-top: 5px; border: 1px solid black; width: 100%; height: 30px; display: flex;">
                        <div style="border-right: 1px solid black; width: 33%;"></div>
                        <div style="border-right: 1px solid black; width: 33%;"></div>
                        <div style="width: 34%;"></div>
                    </div>
                </td>
            </tr>

            <!-- Блок с приходным кассовым ордером -->
            <tr style="vertical-align: bottom;">
                <!-- Левая часть с надписью -->
                <td style="width: 70%; padding: 3px; text-align: right; position: relative;">
                    <div style="position: absolute; bottom: 0; right: 5px; font-weight: bold;font-size:8px;">
                        ПРИХОДНЫЙ КАССОВЫЙ ОРДЕР
                    </div>
                </td>

                <!-- Правая часть с таблицей -->
                <td style="width: 30%; padding: 3px;">
                    <table style="width: 80%; border-collapse: collapse; border: 1px solid black;">
                        <tr>
                            <td style="font-size: 7px;border: 1px solid black; padding: 3px; text-align: center;">Номер документа</td>
                            <td style="font-size: 7px;border: 1px solid black; padding: 3px; text-align: center;">Дата составления</td>
                        </tr>
                        <tr>
                            <td style="font-size: 7px;border: 1px solid black; padding: 5px; text-align: center;"></td>
                            <td style="font-size: 7px;border: 1px solid black; padding: 5px; text-align: center;">)" + dateStr + R"(</td>
                        </tr>
                    </table>
                </td>
            </tr>

        </table>
<!-- Таблица с дебетом, кредитом и прочими полями -->
        <table style="width: 100%; border-collapse: collapse; border: 1px solid black; margin-top: 10px;font-size: 8px;">
            <!-- Заголовки -->
            <tr style="font-size:5px;">
                <td rowspan="2" style="font-size: 8px;border: 1px solid black; padding: 5px; text-align: center; width: 15%;vertical-align: middle;">Дебет</td>
                <td colspan="4" style="font-size: 8px;border: 1px solid black; padding: 5px; text-align: center; width: 35%;vertical-align: middle;">Кредит</td>
                <td rowspan="2" style="font-size: 7px;border: 1px solid black; padding: 5px; text-align: center; width: 15%;vertical-align: middle;">Сумма )"+currency+R"(</td>
                <td rowspan="2" style="font-size: 7px;border: 1px solid black; padding: 5px; text-align: center; width: 15%;vertical-align: middle;">Код целевого назначения</td>
                <td rowspan="2" style="font-size: 8px;border: 1px solid black; padding: 5px; text-align: center; width: 10%;vertical-align: middle;"></td>
            </tr>

            <!-- Подзаголовки под "Кредит" -->
            <tr style="font-size:4px;">
                <td style="font-size: 5px;border: 1px solid black; padding: 5px;vertical-align: middle;"></td> <!-- Пустая ячейка -->
                <td style="font-size: 5px;border: 1px solid black; padding: 5px; text-align: center;vertical-align: middle;">Код структурного подразделения</td>
                <td style="font-size: 5px;border: 1px solid black; padding: 5px; text-align: center;vertical-align: middle;">Корреспондирующий счет, субсчет</td>
                <td style="font-size: 5px;border: 1px solid black; padding: 5px; text-align: center;vertical-align: middle;">Код аналитического учета</td>
            </tr>

            <!-- Данные -->
            <tr style="font-size:3px;">
                <td style="border: 1px solid black; padding: 10px; text-align: center;font-size:7px;">1120</td>
                <td style="border: 1px solid black; padding: 10px;"></td>
                <td style="border: 1px solid black; padding: 10px;"></td>
                <td style="border: 1px solid black; padding: 10px; text-align: center;font-size:7px;">3210</td>
                <td style="border: 1px solid black; padding: 10px;"></td>
                <td style="border: 1px solid black;font-size:6px;text-align: center;vertical-align: middle;">)"+ paidAmount+R"(</td>
                <td style="border: 1px solid black; padding: 10px;"></td>
                <td style="border: 1px solid black; padding: 10px;"></td>
            </tr>
        </table>
        <!-- Основание -->
        <div style="margin-bottom: 10px;">
            <span style="font-size:8px;">Принято от :</span>
            <span style="display: block; width: 80%; text-align: center; position: relative;">
                <span style="position: absolute; top: -15px;font-size:8px;">)" +client + R"(</span>
                <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
            </span>
        </div>


        <!-- Основание -->
        <div style="margin-bottom: 10px;">
            <span style="font-size:8px;">Основание :</span>
            <span style="display: block; width: 80%; text-align: center; position: relative;">
                <span style="position: absolute; top: -15px;font-size:8px;">)"+ "Оплата за "+roomsQuantity+" ком.,Обьект "+object+","+"кв. "+number+",этаж "+floor+","+ploshad+R"( кв.метр</span>
                <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
            </span>
        </div>

        <!-- Сумма -->
        <div>
            <span style="font-size:8px;">Сумма :  _________________________________</span>
            <span style="display: inline-block; width: 80%; text-align: center; position: relative;">
                <span style="position: absolute; top: -15px;">)" + R"(</span>
                <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
            </span>
        </div>
        <div style="height: 10px;"></div> <!-- Уменьшенное пустое место -->

        <!-- В том числе -->
        <div>
            <span style="font-size:8px;">В том числе _________________________________</span>
            <span style="display: inline-block; width: 80%; text-align: center; position: relative;">
                <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
            </span>
        </div>

        <!-- Приложение -->
        <div style="margin-bottom: 10px;">
            <span style="font-size:8px;">Приложение :</span>
            <span style="display: block; width: 80%; text-align: center; position: relative;">
                <span style="position: absolute; top: -15px;font-size:7px;">)" +description + R"(</span>
                <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
            </span>
        </div>


<table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px 5px 5px 0; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div style="font-weight: bold;font-size:8px;">Главный бухгалтер</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium;">__________&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_________________</div>
                    <div style="text-align: left; font-size: 8px">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;расшифровка подписи</div>
                </td>
            </tr>
        </table>

<table style="width: 100%; border-collapse: collapse;">
            <!-- Заголовок документа -->
            <tr>
                <!-- Левая часть -->
                <td style="width: 10%; padding: 5px 5px 5px 0; vertical-align: top;">
                    <div style="position: absolute; top: 0; right: 0; text-align: left;">
                        <div style="font-weight: bold;font-size:8px;">Получил кассир</div>
                    </div>
                </td>

                <!-- Правая часть -->
                <td style="width: 90%; padding: 5px; vertical-align: top;">
                    <div style="font-weight: medium;">&nbsp;&nbsp;&nbsp;&nbsp;__________&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_________________</div>
                    <div style="text-align: left; font-size: 8px">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;расшифровка подписи</div>
                </td>
            </tr>
        </table>

    </div>
)";

    // -- Правая часть (квитанция). Здесь можете добавить любые поля по аналогии с левой:
    QString rightSide = R"(
    <div style="width: 30%;">
        <div style="font-weight: bold;font-size:9px;">Общество с ограниченной ответственностью</div>
        <div style="font-weight: bold; text-align: center;font-size:9px;">"ЭРА Констракшн"</div>
        <div style="font-weight: bold; margin-top: -5px;text-align:center;">————————————————</div>
        <div style="border-bottom: 1px solid black; font-size: 10px; text-align: center;"><sup>организация</sup></div>

        <!-- Заголовок "КВИТАНЦИЯ" -->
        <div style="text-align: center; font-size: 10px; font-weight: bold; margin-top: 10px;">
            КВИТАНЦИЯ
        </div>

        <!-- Номер документа, дата -->
        <table style="width: 100%; border-collapse: collapse; margin-top: 5px;">
            <tr>
                <td style="width: 50%; font-size: 10px;">к ПКО №</td>
                <td style="width: 50%; text-align: right;">_______</td>
                <div style="height: 5px;"></div>
            </tr>
            <tr>
                <td style="font-size: 10px;">&nbsp;&nbsp;&nbsp;&nbsp;от:</td>
                <td style="text-align: right;font-size: 10px; margin-left: 50px;">)" + dateStr + R"(</td>
            </tr>
        </table>

        <!-- Принято от -->
        <div style="margin-top: 10px;">
            <span style="font-size: 10px;">Принято от: </span>
            <span style="border-bottom: 1px solid black;font-size: 10px;">)" + client + R"(</span>
        </div>

        <!-- Основание -->
        <div style="margin-top: 10px;">
            <span style="font-size: 10px;">Основание: </span>
            <span style="border-bottom: 1px solid black;font-size: 10px;">кв. )" + number+","+roomsQuantity+" комнат,"+ploshad + R"(кв.метр</span>
        </div>

        <!-- Сумма -->
        <div style="margin-top: 10px;">
            <span style="font-size: 10px;">Сумма: </span>
            <span style="border-bottom: 1px solid black;font-size: 10px;">)" + paidAmount + " " + currency + R"(</span>
        </div>

        <div style="margin-bottom: 10px;">
            <span style="display: block; width: 80%; text-align: center; position: relative;">
                <span style="position: absolute; top: -15px;font-size: 10px;">)" +description + R"(</span>
                <span style="border-bottom: 1px solid black; display: block; width: 100%;font-size: 10px;">&nbsp;</span>
            </span>
        </div>

        <div style="margin-bottom: 10px;">
            <span style="display: block; width: 80%; text-align: right; position: relative;">
                <span style="position: absolute; top: -15px;">)" +dateStr + R"(</span>
                <span style="border-bottom: 1px solid black; display: block; width: 100%;">&nbsp;</span>
            </span>
        </div>


            <!-- Главный бухгалтер -->
            <div style="display: flex; flex-direction: column; margin-bottom: 40px;margin-top: 10px;font-size: 3px;">
                <span style="font-weight: bold;>М.П.(штамп)</span>
            </div>

        <div style=" height: 30px;"></div>

        <table style="width: 100%; border-collapse: collapse;">
                    <!-- Заголовок документа -->
                    <tr>
                        <!-- Левая часть -->
                        <td style="width: 10%; padding: 5px 5px 5px 0; vertical-align: top;">
                            <div style="position: absolute; top: 0; right: 0; text-align: left;">
                                <div style="font-weight: bold;font-size:8px;">Главный бухгалтер</div>
                            </div>
                        </td>

                        <td style="width: 10%; padding: 5px 5px 5px 0; vertical-align: top;">
                            <div style="font-weight: medium;">_____&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;________</div>
                            <div style="text-align: left; font-size: 5px">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;расшифровка подписи</div>

                        </td>
                    </tr>
                </table>

        <table style="width: 100%; border-collapse: collapse;">
                    <!-- Заголовок документа -->
                    <tr>
                        <!-- Левая часть -->
                        <td style="width: 10%; padding: 5px 5px 5px 0; vertical-align: top;">
                            <div style="position: absolute; top: 0; right: 0; text-align: left;">
                                <div style="font-weight: bold;font-size:8px;">Кассир</div>
                            </div>
                        </td>
                        <td style="width: 10%; padding: 5px 5px 5px 0; vertical-align: top;">
                            <div style="font-weight: medium;">&nbsp;&nbsp;&nbsp;&nbsp;_____&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;_________</div>
                            <div style="text-align: left; font-size: 5px">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;подпись &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;расшифровка подписи</div>

                        </td>
                    </tr>
                </table>



    </div>
    )";

    // -- Формируем общий HTML: таблица на 2 колонки, между которыми вертикальная пунктирная линия
    QString html = R"(
    <div style="font-family: Arial, sans-serif; font-size: 12px; width: 100%; position: relative;">
        <table style="width: 100%; border-collapse: collapse;">
            <colgroup>
                <col style="width: 70%;">
                <col style="width: 30%;">
            </colgroup>
            <tr>
                <td style="vertical-align: top; border-right: 2px dashed black; padding: 0 10px;">
                    )" + leftSide + R"(
                </td>
                <td style="vertical-align: top; padding-left: 10px;">
                    )" + rightSide + R"(
                </td>
            </tr>
        </table>
    </div>
)";




    return html;
}














QString NakladnayaPaymentsPDF::getStyleSheet()
{
    // CSS styles for the PDF document
    return R"(
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: Arial, sans-serif;
    font-size: 12px;
}
h1, h2, p, table, th, td {
    margin: 0;
    padding: 0;
}
)";
}


QString NakladnayaPaymentsPDF::getDesktopDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

void NakladnayaPaymentsPDF::showPdfViewer(const QString &pdfPath) {
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

