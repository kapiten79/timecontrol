#include "controller_exportexcel.h"

controller_exportExcel::controller_exportExcel(QObject *parent) : QObject(parent)
{

}

void controller_exportExcel::process_export()
{
    qDebug() << "Функция controller_exportExcel::process_export() запустилась (контроллер)" ;
#ifdef Q_OS_WIN32
    /** Инициализация Excel */
    QAxWidget excel     ("Excel.Application"    );
    excel.setProperty   ("Visible", false)      ;

    QAxObject* workbooks= excel.querySubObject("WorkBooks");
    workbooks->dynamicCall("Add"); // Добавление нового workbook

    QAxObject* workbook = excel.querySubObject("ActiveWorkBook");
    QAxObject* sheets   = workbook->querySubObject("WorkSheets");

    QAxObject * cell;
    QAxObject * font;
    QString     text;
    int count= sheets->dynamicCall("Count()").toInt();
    bool isEmpty = true;

    QAxObject *rangec;
    QAxObject *razmer;

    for(int k=1;k<=count;k++)
    {
        QAxObject* sheet = sheets->querySubObject( "Item( int )", k );

        if (qv.count() > 0)
        {
            int row     = qv[0].count()+5;
            int column  = topics.count()+1;
            /** Перебираем строки отчёта */
            for(int i=1; i<row ; i++)
            {
                /** Перебираем столбцы отчёта */
                for(int j=1; j<column ;j++)
                {
                    /** Выбираем объект ячейки, в которую в данный момент будет заноситься информация */
                    cell = sheet->querySubObject( "Cells( int, int )", i,j)  ;   // Координаты ячейки, в которую надо записать информацию
                    /** Создаём объект, икапсулирующий шрифт данной конкретной ячейки  */
                    font = cell->querySubObject("Font");

                    /** Если строка первая, то это реклама */
                    if (i == 1)
                    {
                        /** Устанавливаем высоту первой строки*/
                        rangec = sheet->querySubObject( "Range(const QVariant&)",QVariant(QString("A1")));//опять же,указатель на D2
                        razmer = rangec->querySubObject("Rows"); //получаю указатель на строку
                        razmer->setProperty("RowHeight",100);// устанавливаю её размер.

                        /** Объединение ячеек в шапке */
                        QAxObject *rangec = sheet->querySubObject( "Range(const QVariant&)",QVariant(QString("A1:E1")));
                        rangec->dynamicCall("Select()");
                        rangec->dynamicCall("MergeCells",true);


                        /** Шрифт шапки отчёта */
                        font->setProperty("Bold", false)    ;
                        font->setProperty("Italic", true)   ;

                        /** Внесение шапки */
                        if (j == 1)
                        {
                            text = "Отчёт сгенерирован в программе 'Контроль времени'. "
                                           "\nНаименование отчёта: "+reportName+" \nДата отчёта: "+reportDate+" \nСформирован в: "+reportTime;
                        }
                        else{text = "";}
                    }
                    /** Если строка вторая, это заголовок. Форматичнуем его */
                    else if (i == 3)
                    {
                        /** Устанавливаем параметры обрамления ячеек
                         xlEdgeTop(верхняя граница) (xlEdgeLeft) левая,
                         (xlEdgeRight) правая,(xlEdgeBottom) нижняя
                         и 2 диагонали (xlDiagonalDown) (xlDiagonalUp)*/

                        /** Верхняя граница */
                        QAxObject *borderTop = cell->querySubObject("Borders(xlEdgeTop)");
                        // тип линии (там пунктиры,сплошная и так далее)
                        borderTop->setProperty("LineStyle",1);
                        // толщина
                        borderTop->setProperty("Weight",4);

                        /** Нижняя граница */
                        QAxObject *borderBottom = cell->querySubObject("Borders(xlEdgeBottom)");
                        borderBottom->setProperty("LineStyle",1);
                        borderBottom->setProperty("Weight",4);

                        /** Справа */
                        QAxObject *borderRight = cell->querySubObject("Borders(xlEdgeRight)");
                        borderRight->setProperty("LineStyle",1);
                        borderRight->setProperty("Weight",4);

                        /** Слева */
                        QAxObject *borderLeft = cell->querySubObject("Borders(xlEdgeLeft)");
                        borderLeft->setProperty("LineStyle",1);
                        borderLeft->setProperty("Weight",4);

                        /** Устанавливаем параметры шрифта */
                        font->setProperty("Bold", true)    ;
                        font->setProperty("Italic", false)   ;
                        text     = topics[j-1].toString()  ;

                        /** Очередной столбец */
                        razmer = cell->querySubObject("Columns"); //получаю указатель на строку
                        razmer->setProperty("ColumnWidth",widthList[j-1].toString().toInt());// устанавливаю её размер.

                        cell->dynamicCall("Select()");
                        cell->dynamicCall("HorizontalAlignment",-4108);
                    }
                    /** Тело отчёта */
                    else if (i > 3 && i != qv[0].count()+4)
                    {
                        if (i != 4)
                        {
                            /** Верхняя граница */
                            QAxObject *borderTop = cell->querySubObject("Borders(xlEdgeTop)");
                            borderTop->setProperty("LineStyle",1);
                            borderTop->setProperty("Weight",2);
                        }
                        else
                        {
                            /** Верхняя граница */
                            QAxObject *borderTop = cell->querySubObject("Borders(xlEdgeTop)");
                            borderTop->setProperty("LineStyle",1);
                            borderTop->setProperty("Weight",4);
                        }

                        if (j > 1)
                        {
                            // выбираю
                            cell->dynamicCall("Select()");
                            // по горизонтальной оси устанавливаю по центру.
                            // Excel интерпретирует числовые значения положений в свои строковые.например xlCenter.
                            cell->dynamicCall("HorizontalAlignment",-4152);
                        }
                        else
                        {
                            // выбираю
                            cell->dynamicCall("Select()");
                            // по горизонтальной оси устанавливаю по центру.
                            // Excel интерпретирует числовые значения положений в свои строковые.например xlCenter.
                            cell->dynamicCall("HorizontalAlignment",-4131);
                        }

                        /** Форматируем вертикальное выравнивание */
                        // Выбираю
                        cell->dynamicCall("Select()");
                        // По вертикальной оси
                        cell->dynamicCall("VerticalAlignment",-4160);

                        /** Разрешаем перенос на новую строку */
                        cell->dynamicCall("Select()");
                        // устанавливаю свойство разрешения переноса
                        cell->dynamicCall("WrapText",true);
                        // Всё пашет )))Сорри за мой английский)))
                        cell->dynamicCall("setValue(const QVariant&)",QVariant("This text is so long:)))WordWrap is working"));

                        /** Нижняя граница */
                        QAxObject *borderBottom = cell->querySubObject("Borders(xlEdgeBottom)");
                        borderBottom->setProperty("LineStyle",1);
                        borderBottom->setProperty("Weight",2);

                        /** Справа */
                        QAxObject *borderRight = cell->querySubObject("Borders(xlEdgeRight)");
                        borderRight->setProperty("LineStyle",1);
                        borderRight->setProperty("Weight",2);

                        /** Слева */
                        QAxObject *borderLeft = cell->querySubObject("Borders(xlEdgeLeft)");
                        borderLeft->setProperty("LineStyle",1);
                        borderLeft->setProperty("Weight",2);

                        /** Если строка вторая и более, это обычная таблица. Форматируем её. */
                        font->setProperty("Bold", false);
                        text     = qv[j-1][i-4]                              ;
                    }
                    else if (i == qv[0].count()+4)
                    {
                        /** Верхняя граница */
                        QAxObject *borderTop = cell->querySubObject("Borders(xlEdgeTop)");
                        borderTop->setProperty("LineStyle",1);
                        borderTop->setProperty("Weight",4);
                        if (j > 1)
                        {
                            // выбираю
                            cell->dynamicCall("Select()");
                            // по горизонтальной оси устанавливаю по центру.
                            // Excel интерпретирует числовые значения положений в свои строковые.например xlCenter.
                            cell->dynamicCall("HorizontalAlignment",-4152);
                        }
                        else
                        {
                            // выбираю
                            cell->dynamicCall("Select()");
                            // по горизонтальной оси устанавливаю по центру.
                            // Excel интерпретирует числовые значения положений в свои строковые.например xlCenter.
                            cell->dynamicCall("HorizontalAlignment",-4131);
                        }

                        /** Нижняя граница */
                        QAxObject *borderBottom = cell->querySubObject("Borders(xlEdgeBottom)");
                        borderBottom->setProperty("LineStyle",1);
                        borderBottom->setProperty("Weight",4);

                        /** Справа */
                        QAxObject *borderRight = cell->querySubObject("Borders(xlEdgeRight)");
                        borderRight->setProperty("LineStyle",1);
                        borderRight->setProperty("Weight",4);

                        /** Слева */
                        QAxObject *borderLeft = cell->querySubObject("Borders(xlEdgeLeft)");
                        borderLeft->setProperty("LineStyle",1);
                        borderLeft->setProperty("Weight",4);

                        /** Если строка вторая и более, это обычная таблица. Форматируем её. */
                        font->setProperty("Bold", false);
                        if (j == 1)
                        {
                            text = "ИТОГО:";
                        }
                        else
                        {
                            text = total[j].toString();
                        }
                    }


                    // Значение, которое вы хотите экспортировать
                    cell->dynamicCall("SetValue(QString)",text)                             ;   // Запись информации в ячейку
                }
            }
        }
        QString fileName=QFileDialog::getSaveFileName(0,"save file",reportName,
            "XML Spreadhseet(*.xlsx);;eXceL Spreadsheet(*.xls);;Comma Seperated Value(*.csv)");
        fileName.replace("/","\\");
        workbook->dynamicCall("SaveAs(QString&)",fileName);
        workbook->dynamicCall("Close (Boolean)",false);
    }
#endif


}
