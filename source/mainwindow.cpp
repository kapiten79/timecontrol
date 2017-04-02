#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi         (this)  ;

}

MainWindow::~MainWindow()
{
    cmw->saveDay();
    cmw->dps->close();
    cmw->dct->close();
    cmw->di->close();

    delete ui;
}

/* Инициализируем параметры окна */
void MainWindow::init_windowParams()
{
    qDebug() << "Функция init_windowParams запустилась (модель)";

    this->showMaximized();
    cmw->model = this;

    /** Параметры главного окна */
    this->setWindowTitle("Контроль времени.  v 1.01.001");

    /** Получаем ширину поля таблицы и высчитываем ширину поля
     * заголовка в процентном соотношении */

    int fullWidth, nameWidth;
    fullWidth = ui->tableWidget->width();
    nameWidth = fullWidth - 144 - 160 - 18;

    /** Ширина столбцов */
    ui->tableWidget->setColumnWidth(0,nameWidth);
    ui->tableWidget->setColumnWidth(1,144);
    ui->tableWidget->setColumnWidth(2,160);


    /** Добавление иконки создания проекта */
    const QIcon newProjIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/new_proj.png"))   ;
    QAction     *newProjAct = new QAction(newProjIcon, tr("&Создание нового проекта..."), this)                     ;
    newProjAct->setShortcuts(QKeySequence::Open)                                                                    ;
    newProjAct->setStatusTip(tr("Создание нового проекта"))                                                         ;

    connect(newProjAct, &QAction::triggered, cmw, &controller_mainWindow::create_project);
    //fileMenu->addAction(newProjAct);
    ui->mainToolBar->addAction(newProjAct);

    /** Добавление иконки немедленного сохранения информации */
    const QIcon saveIcon    = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/save_all.png"))   ;
    QAction *saveAct        = new QAction(saveIcon, tr("&Сохранить все данные..."), this)                           ;
    saveAct->setShortcuts(QKeySequence::Open); //гоячая клавиша
    saveAct->setStatusTip(tr("Сохранить всё"));
    saveAct->setEnabled(false);
    connect(saveAct, &QAction::triggered, cmw, &controller_mainWindow::save_project);
    //fileMenu->addAction(newProjAct);
    ui->mainToolBar->addAction(saveAct);

    /** Добавление иконки открытия настроек проекта */

    const QIcon settingsProjIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/settings_proj.png"));
    QAction *settingsProjAct = new QAction(settingsProjIcon, tr("&Редактирование параметров проекта..."), this);
    settingsProjAct->setShortcuts(QKeySequence::Open);
    settingsProjAct->setStatusTip(tr("Редактирование параметров проекта"));
    settingsProjAct->setEnabled(false);
    connect(settingsProjAct, &QAction::triggered, cmw, &controller_mainWindow::open_projSettings);
    //fileMenu->addAction(newProjAct);
    ui->mainToolBar->addAction(settingsProjAct);

    ui->mainToolBar->addSeparator();

    /** Добавление иконки нового звонка */
    const QIcon newCallIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/new_call.png"));
    QAction *newCallAct = new QAction(newCallIcon, tr("&Регистрация нового звонка..."), this);
    newCallAct->setShortcuts(QKeySequence::Open);
    newCallAct->setStatusTip(tr("Регистрация нового звонка"));
    newCallAct->setEnabled(false);
    connect(newCallAct, &QAction::triggered, cmw, &controller_mainWindow::open_createCallDialog);
    //fileMenu->addAction(newProjAct);
    ui->mainToolBar->addAction(newCallAct);

    /** Добавление иконки истории звонков */
    const QIcon historyCallIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/history_call.PNG"));
    QAction *historyCallAct = new QAction(historyCallIcon, tr("&История звонков..."), this);
    historyCallAct->setShortcuts(QKeySequence::Open);
    historyCallAct->setStatusTip(tr("История звонков"));
    historyCallAct->setEnabled(false);
    connect(historyCallAct, &QAction::triggered, cmw, &controller_mainWindow::open_viewCallDialog);
    //fileMenu->addAction(newProjAct);
    ui->mainToolBar->addAction(historyCallAct);

    ui->mainToolBar->addSeparator();

    /** Добавление иконки поиска */
    const QIcon findIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/find.png"));
    QAction *findAct = new QAction(findIcon, tr("&Глобавльный поиск..."), this);
    findAct->setShortcuts(QKeySequence::Open);
    findAct->setStatusTip(tr("Глобавльный поиск"));
    findAct->setEnabled(false);
    connect(findAct, &QAction::triggered, cmw, &controller_mainWindow::open_dialogFind);
    //fileMenu->addAction(newProjAct);
    ui->mainToolBar->addAction(findAct);

    ui->mainToolBar->addSeparator();

    /** Добавление иконки переноса задачи на другую дату */
    const QIcon perenosIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/perenos.png"));
    QAction *perenosAct = new QAction(perenosIcon, tr("&Перенести задачу на другую дату..."), this);
    perenosAct->setShortcuts(QKeySequence::Open);
    perenosAct->setStatusTip(tr("Перенос задачи"));
    perenosAct->setEnabled(false);
    connect(perenosAct, &QAction::triggered, this, &MainWindow::set_nextDay);
    //fileMenu->addAction(newProjAct);
    ui->mainToolBar->addAction(perenosAct);

    /** Описание иконок кнопок задач */
    const QIcon newTaskIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/new_task.png"));
    ui->pushButton->setIcon(newTaskIcon);

    const QIcon playTaskIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/play.png"));
    ui->pushButton_2->setIcon(playTaskIcon);

    const QIcon stopTaskIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/png_ico/stop.png"));
    ui->pushButton_3->setIcon(stopTaskIcon);

    const QIcon deleteTaskIcon = QIcon::fromTheme("project-new", QIcon(":/ico/png_ico/png_ico/delete.png"));
    ui->pushButton_4->setIcon(deleteTaskIcon);

    /** Стиль календаря */
    ui->calendarWidget->setGridVisible(true);
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    /** Создание списка шрифтов */
    QFontDatabase *fd = new QFontDatabase;
    ui->comboBoxFontList->insertItems(0,fd->families());
    ui->comboBoxFontList->setCurrentText("Arial");

    QFont currFont;
    currFont.setPixelSize(14);
    currFont.setFamily("Arial");
    ui->plainTextEdit->setFont(currFont);

    textCursor = ui->plainTextEdit->textCursor();

    ui->toolButton          ->setCheckable(true);
    ui->toolButtonNaklon    ->setCheckable(true);
    ui->toolButtonPodcherk  ->setCheckable(true);
    ui->toolButtonZacherk   ->setCheckable(true);

    setClearCalendar();
}

/* Изменение размеров полей таблицы при изменении размеров окна */
void MainWindow::resizeEvent(QResizeEvent* event)
{
    qDebug() << "Функция resizeEvent запустилась (модель)";

    QMainWindow::resizeEvent(event);

    /** Получаем ширину поля таблицы и высчитываем ширину поля
     * заголовка в процентном соотношении */

    int fullWidth, nameWidth;
    fullWidth = ui->tableWidget->width();
    nameWidth = fullWidth - 144 - 160-18;

    /** Ширина столбцов */
    ui->tableWidget->setColumnWidth(0,nameWidth);
    ui->tableWidget->setColumnWidth(1,144);
    ui->tableWidget->setColumnWidth(2,160);
}

/* Нажатие кнопки создания новой задачи */
void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Функция MainWindow::on_pushButton_clicked() запустилась (модель)";

    if (ui->tabWidget->count() > 1)
    {
        if (ui->tableWidget->rowCount() > 0)cmw->saveDay();

        cmw->open_createTaskDialog();
    }

}

/* Добавление новой записи о задаче */
void MainWindow::reloadTaskList()
{
    qDebug() << "Функция reloadTaskList запустилась (модель)";

    /** Перебираем задачи и заполняем строки таблицы.
        Так как список задач обновляется на уровне глобальных переменных,
        каждый раз чистим таблицу перед заполнением */

    int rowCount = ui->tableWidget->rowCount();
    for (int i=0; i<rowCount; i++)
    {
        ui->tableWidget->removeRow(0);
    }

    for (int i=0; i<cmw->tList.taskName.count(); i++)
    {
        //qDebug() << cmw->tList.taskDate[i].toString();

        if (cmw->tList.taskDate[i].toString() == cmw->calendarDate.toString("dd_MM_yyyy"))
        {
            /** Сохранение "Начальных" данных о задаче в файл */
            /** Добавление новой строки для отображения задачи */
            int newRowNum = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(newRowNum);

            /** Запрещаем редактирование времени и цены задачи */

            QTableWidgetItem * taskNameCol              = new QTableWidgetItem(cmw->tList.taskName[i].toString() );
            QTableWidgetItem * timerCol                 = new QTableWidgetItem(cmw->tList.taskTime[i].toString() );
            QTableWidgetItem * priceCol                 = new QTableWidgetItem(cmw->tList.taskPrice[i].toString());

            if (cmw->tList.taskTime[i].toString() == "00:00:00")
            {
                /** Визуальное отображение только что созданной задачи */
                QColor indColor(255, 117, 24);
                //taskNameCol->setBackgroundColor(indColor);
                timerCol->setBackgroundColor(indColor);
                priceCol->setBackgroundColor(indColor);
            }
            else
            {
                /** Визуальное отображение начала работы над задачей */
                QColor indColor("lightyellow");
                //taskNameCol->setBackgroundColor(indColor);
                timerCol->setBackgroundColor(indColor);
                priceCol->setBackgroundColor(indColor);
            }

            if (cmw->tList.completeFlag[i].toString() == "1")
            {
                QColor indColor("lightgreen");
                //taskNameCol->setBackgroundColor(indColor);
                timerCol->setBackgroundColor(indColor);
                priceCol->setBackgroundColor(indColor);

                ui->checkBoxDone->setChecked(true);
            }
            else
            {
                ui->checkBoxDone->setChecked(false);
            }

            /** Запрещаем любые действия с полями времени и стоимости работ  */
            timerCol->setFlags(Qt::NoItemFlags);
            priceCol->setFlags(Qt::NoItemFlags);

            ui->tableWidget->setItem(newRowNum,0,taskNameCol        );
            ui->tableWidget->setItem(newRowNum,1,timerCol           );
            ui->tableWidget->setItem(newRowNum,2,priceCol           );

            for (int i=0; i<ui->tableWidget->rowCount(); i++)
            {
                ui->tableWidget->item(i,0)->setSelected(false);
                ui->tableWidget->item(i,1)->setSelected(false);
                ui->tableWidget->item(i,2)->setSelected(false);
            }

            ui->tableWidget->item(newRowNum,0)->setSelected(true);
            ui->tableWidget->item(newRowNum,1)->setSelected(true);
            ui->tableWidget->item(newRowNum,2)->setSelected(true);

            //ui->plainTextEdit->setPlainText(cmw->tList.taskFullText[cmw->currIndexNum].toString());
            ui->plainTextEdit->document()->setHtml(cmw->tList.taskFullText[cmw->currIndexNum].toString());
            ui->plainTextEdit->setFocus();

            if (cmw->tList.completeFlag[cmw->currIndexNum] == "1")
            {
                ui->checkBoxDone->setChecked(true);
            }
        }
        if(ui->tableWidget->rowCount() > 0)
        {
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_4->setEnabled(true);
        }
        else
        {
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
        }
    }
}


/* Запускаем таймер */
void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "Функция on_pushButton_2_clicked запустилась (модель)";
    cmw->stopTaskTimer();

    /** Визуальное отображение начала работы над задачей */
    int currRow = cmw->currTaskNum;
    qDebug() << currRow;
    QColor indColor("lightyellow");
    ui->tableWidget->item(currRow,0)->setBackgroundColor(indColor);
    ui->tableWidget->item(currRow,1)->setBackgroundColor(indColor);
    ui->tableWidget->item(currRow,2)->setBackgroundColor(indColor);

    if (ui->tableWidget->rowCount() > 0)
    {
        /** Определяем значения таймера в выделенной строке */
        qDebug() << "Количесво записей времени задачи " << cmw->tList.taskTime.count() << " Очередной номер " << cmw->currIndexNum;
        cmw->tList.taskTime[cmw->currIndexNum] = QVariant(ui->tableWidget->item(cmw->currTaskNum,1)->text());
        cmw->startTaskTimer();

        /** Делаем кнопку запустить не доступной, а кнопку остановить доступной */
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
    }

}

/* Выводим значения времени */
void MainWindow::setTaskTime()
{
    qDebug() << "Функция setTaskTime запустилась (модель)";
    ui->tableWidget->item(cmw->currTaskNum,1)->setText(cmw->tList.taskTime[cmw->currIndexNum].toString());
}
/* Отображение стоимости задачи */
void MainWindow::setTaskPrice()
{
    qDebug() << "Функция setTaskPrice запустилась (модель)";
    ui->tableWidget->item(cmw->currTaskNum,2)->setText(cmw->tList.taskPrice[cmw->currIndexNum].toString());
}

/* Подсчёт суммарного времени для всех задач */
void MainWindow::setDayTime()
{
    qDebug() << "Функция setDayTime запустилась (модель)";
    ui->label_4->setText(cmw->allTime);
    ui->label_8->setText(cmw->dayTime);
}

/* Подсчёт суммарной стоимости всех задач */
void MainWindow::setDayPrice()
{
    qDebug() << "Функция setDayPrice запустилась (модель)";
    ui->label_5->setText(cmw->allPrice);
    ui->label_9->setText(cmw->dayPrice);
}

/* Остановка текущего таймера */
void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "Функция on_pushButton_3_clicked запустилась (модель)";
    if (ui->tableWidget->rowCount() > 0)
    {
        cmw->stopTaskTimer();
        /** После остановки таймера задачи делаем кнопку "Запустить" доступной, а "Остановить" не доступной */
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
    }
}

/* Изменение текста подробного описания задачи */
void MainWindow::on_plainTextEdit_textChanged()
{
    qDebug() << "Функция on_plainTextEdit_textChanged запустилась (модель) " << cmw->currTaskNum;
    if (ui->tableWidget->rowCount() > 0 && ui->tableWidget->rowCount() > cmw->currTaskNum)
    {
        cmw->tList.taskFullText[cmw->currIndexNum] = ui->plainTextEdit->document()->toHtml();
        if (cmw->tList.completeFlag[cmw->currIndexNum] == "1")
        {
            ui->checkBoxDone->setChecked(true);
        }
        else
        {
            ui->checkBoxDone->setChecked(false);
        }
    }
}


/* Действия при выборе строки в таблице */
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    qDebug() << "Функция on_tableWidget_cellClicked запустилась (модель)";
    cmw->saveDay();
    cmw->currTaskNum = row;
    cmw->currIndexNum = cmw->indexNumList[row].toString().toInt();
    //ui->plainTextEdit->setPlainText(cmw->tList.taskFullText[cmw->currIndexNum].toString());
    ui->plainTextEdit->document()->setHtml(cmw->tList.taskFullText[cmw->currIndexNum].toString());
    ui->plainTextEdit->setFocus();

}

/* Нажатие кнопки "Удалить" */
void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << "Функция on_pushButton_4_clicked запустилась (модель)";
    if (ui->tableWidget->rowCount() > 0)
    {
        cmw->open_removeTaskDialog();
    }
}
/* Удаление задачи из интерфейса*/
void MainWindow::removeCurrTask()
{
    qDebug() << "Функция removeCurrTask запустилась (модель)";
    ui->tableWidget->removeRow(cmw->currTaskNum);
    ui->plainTextEdit->clear    () ;
    if(ui->tableWidget->rowCount() > 0)
    {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }
}

/* Выбор даты в календаре */
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << "Функция on_calendarWidget_clicked запустилась (модель) " << date.toString("dd.MM.yyyy");

    cmw ->stopTaskTimer();
    ui  ->tableWidget->clearContents();
    cmw ->calendarDate = date;
    ui  ->plainTextEdit->clear();
    cmw ->setDay(1);
}

void MainWindow::on_projectSettings_triggered()
{
    qDebug() << "Функция on_projectSettings_triggered запустилась (модель)";
    cmw->open_projSettings(true);
}

/* Выбор закладки проекта */
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    qDebug() << "Функция on_tabWidget_tabBarClicked запустилась (модель) " << index;
    /** Если это не закладка откртыия проекта */
    if (ui->tabWidget->tabText(index) != "Выберите папку проекта двойным щелчком ...")
    {
        /** Останавливаем счётчик времени текущего проекта */
        cmw->stopTaskTimer();
        currentTab  = index;
        QString dir = cmw->projList[index].toString();
        if (dir != "")
        {
            qDebug() << "Название проекта " << dir;
            mainDir.setCurrent(*cmw->workDir);
            mainDir.cdUp();
            mainDir.cd(dir);
            cmw->workDir->clear();
            cmw->workDir = new QString(mainDir.path());
            //cmw->setWorkDir(dir);
            //cmw->init_window();

            ui->tableWidget->clearContents();

            ui->plainTextEdit->clear();

            cmw->setTodayDateTime();

            ui->calendarWidget->setSelectedDate(cmw->todayDate);

            cmw->setDay(1);
        }
    }
}

/* Открытие проекта по двойному щелчку на закладке */
void MainWindow::on_tabWidget_tabBarDoubleClicked(int index)
{
    qDebug() << "Функция on_tabWidget_tabBarDoubleClicked запустилась (модель) " << *cmw->workDir;

    /** Перед котрытием нового проекта останавливаем таймер, если таковой был запущен */
    cmw->stopTaskTimer();

    tempWorkDir = *cmw->workDir;

    /** Если текущий рабочий каталог не является домашним, делаем текущим родительский каталог */
    if (*cmw->workDir != QDir::homePath())
    {
        QDir currDir;
        currDir.setPath(*cmw->workDir);
        currDir.cdUp();
        *cmw->workDir = currDir.path();
    }



    /** Открываем диалоговое окно выбора каталога */
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите каталог проекта или создайте новый ..."),
                                                    *cmw->workDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    /** Обрабатываем полученный адрес каталога, выделяя из него имя проекта  */
    //QString projName = dir.mid(dir.lastIndexOf("/")+1, dir.length()-1);

    /** Присваиваем имя проекта переменной workDir */
    delete(cmw->workDir);
    cmw->workDir = new QString(dir);

    /** Запускаем функцию открытия проекта */
    open_project();
}

/* Открытие новой закладки с проектом */
void MainWindow::open_project()
{
    qDebug() << "Функция open_project запустилась (модель) ";

    /** Исключаем ситуацию, при которой в окне выбора каталога была нажата кнопка "Отмена" */

    if (*cmw->workDir != "")
    {
        /** Активируем иконки панели задач. Они заблокированы до тех пор, пока не будет открыт хотябы 1 проект */
        ui->mainToolBar->actions()[0]->setEnabled(true);
        ui->mainToolBar->actions()[1]->setEnabled(true);
        ui->mainToolBar->actions()[2]->setEnabled(true);
        ui->mainToolBar->actions()[4]->setEnabled(true);
        ui->mainToolBar->actions()[5]->setEnabled(true);
        ui->mainToolBar->actions()[7]->setEnabled(true);

        /** Активируем основные элементы окна. Они заблокированы до тех пор, пока не будет открыт хотябы 1 проект */
        ui->tableWidget     ->setEnabled(true);
        ui->calendarWidget  ->setEnabled(true);
        ui->plainTextEdit   ->setEnabled(true);
        ui->checkBoxDone    ->setEnabled(true);

        /** Открываем файл проекта для получения его наименования */
        qDebug() << "ПЕРЕД ОТКРЫТИЕМ ПРОЕКТА";
        cmw->open_project();
        qDebug() << "ПОСЛЕ ОТКРЫТИЕМ ПРОЕКТА";
        if (cmw->errorFlag){*cmw->workDir = tempWorkDir;return;}

        /** Подготавливаем информацию для открытия проекта */
        int index = ui->tabWidget->count()-1    ;
        ui->tabWidget->setTabText(index,cmw->pList->projNameSt)    ;
        ui->tabWidget->setCurrentIndex(index);
        ui->tabWidget->setTabsClosable(true);
        currentTab = index                      ;

        /** Добавляем новую закладку, через которую можно будет открыть ещё один проект */
        QWidget *w = new QWidget();
        ui->tabWidget->insertTab(index+1, w, "Выберите папку проекта двойным щелчком ...");

        /** Очищаем таблицу задач и поле описания задач */
        ui->tableWidget->clearContents();
        ui->plainTextEdit->clear();

        /** Загружаем данные по заданному в календаре дню проекта */
        cmw->setDay();

        /** Активируем кнопки управления задачами */

        ui->pushButton->setEnabled(true);
        if(ui->tableWidget->rowCount() > 0)
        {
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_4->setEnabled(true);
        }
        else
        {
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
        }

        /** Активируем пункты меню */
        ui->action->setEnabled(true);
        ui->action_2->setEnabled(true);
        ui->action_3->setEnabled(true);
        ui->action_5->setEnabled(true);
        ui->action_6->setEnabled(true);
        ui->projectSettings->setEnabled(true);
        cmw->init_window();
    }
    else
    {
        /** Если каталог пустой, назначаем значение по умолчанию (после отмены открытия проекта в частности)*/
        cmw->workDir->clear();
        cmw->workDir = new QString(QDir::home().path());
    }
}

void MainWindow::setDayColor(QDate editedDate)
{
    //qDebug() << "Функция setDayColor() запустилась (модель) " << editedDate.toString("dd_MM_yyyy");
    QFont           charFont    ;
    charFont.setBold(true);
    charFont.setPixelSize(12);

    QTextCharFormat charFormat  ;
    charFormat.setFont(charFont);

    ui->calendarWidget->setDateTextFormat(editedDate, charFormat);
}

void MainWindow::setClearCalendar()
{
    qDebug() << "Функция setClearCalendar запустилась (модель)";
    QDate editedDate;
    QTextCharFormat charFormat;
    ui->calendarWidget->setDateTextFormat(editedDate, charFormat);
}

/* Открытие окна создания нового звонка */
void MainWindow::on_action_triggered()
{
    qDebug() << "Функция on_action_triggered запустилась (модель)";

    cmw->open_createCallDialog(true);
}

/* Открытие окна просмотра списка звонков для данного проекта */
void MainWindow::on_action_2_triggered()
{
    qDebug() << "Функция on_action_triggered запустилась (модель)";

    cmw->open_viewCallDialog(true);
}

/* Открытие окна поиска по всем данным (общий поиск) */
void MainWindow::on_action_3_triggered()
{
    qDebug() << "Функция on_action_3_triggered запустилась (модель)";

    cmw->open_dialogFind(true);
}

/* Обновление содержимого окна при переключении между месяцами и годами */
void MainWindow::on_calendarWidget_currentPageChanged(int year, int month)
{
    qDebug() << "Функция on_calendarWidget_currentPageChanged запустилась (модель)";

    QDate currDate;
    currDate.setDate(year, month, ui->calendarWidget->selectedDate().day());
    ui->calendarWidget->setSelectedDate(currDate);
    cmw->calendarDate = currDate;

    if (ui->menuBar->isEnabled() == true)
    {
        cmw->init_window(0);
    }

}

/* Перенос задачи на новую дату */
void MainWindow::set_nextDay(bool tr)
{
    qDebug() << "Функция set_nextDay запустилась (модель)";
    ui->menuBar         ->setEnabled(false);
    ui->mainToolBar     ->setEnabled(false);
    ui->plainTextEdit   ->setEnabled(false);
    ui->checkBoxDone    ->setEnabled(false);
    ui->pushButton      ->setEnabled(false);
    ui->pushButton_2    ->setEnabled(false);
    ui->pushButton_3    ->setEnabled(false);
    ui->pushButton_4    ->setEnabled(false);
    ui->tabWidget       ->setEnabled(false);
}

/* Выбор пункта меню "Отчёты -> По времени" */
void MainWindow::on_action_5_triggered()
{
    qDebug() << "Функция on_action_5_triggered запустилась (модель)";
    Dialog_timeReport *tr = new Dialog_timeReport;
    tr->ctr->workDir = cmw->workDir;
    tr->setWindowTitle("Отчёт по времени");
    tr->init_window();
}


/* Выбор пункта "По выполненным задачам" меню "Отчёты" */
void MainWindow::on_action_6_triggered()
{
    qDebug() << "Функция MainWindow::on_action_6_triggered() запустилась (модель)";
    Dialog_reportTask *drt = new Dialog_reportTask;
    drt->crt.workDir = cmw->workDir;
    drt->setWindowTitle("Отчёт по выполненным заданиям за период");
    drt->init_window();
}

/* Кнопка закрытия закладки с проектом */
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    qDebug() << "Функция MainWindow::on_tabWidget_tabCloseRequested(int index) запустилась (модель)" ;
    if (ui->tabWidget->tabText(index) != "Выберите папку проекта двойным щелчком ...")
    {
        qDebug() << "В условии";

        /** Открываем проект на случай, если выбрана не активная закладка */
        on_tabWidget_tabBarClicked(index);


        ui->tableWidget->setRowCount(0);
        ui->plainTextEdit->clear();
        setClearCalendar();

        /** Если все закладки с проектами закрыты, блокируем кнопки управления проектами */
        qDebug() << ui->tabWidget->count();
        if (ui->tabWidget->count() == 1)
        {
            ui->pushButton  ->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_4->setEnabled(false);

            /** Блокируем календарь, таблицу и поле описания задачи */
            ui->tableWidget     ->setEnabled(false);
            ui->plainTextEdit   ->setEnabled(false);
            ui->checkBoxDone    ->setEnabled(false);

            /** Блокируем иконки панели инструментов */
            ui->mainToolBar->actions()[1]->setEnabled(false);
            ui->mainToolBar->actions()[2]->setEnabled(false);
            ui->mainToolBar->actions()[3]->setEnabled(false);
            ui->mainToolBar->actions()[4]->setEnabled(false);
            ui->mainToolBar->actions()[5]->setEnabled(false);
            ui->mainToolBar->actions()[6]->setEnabled(false);
            cmw->close_project(index);
        }
        else
        {
            /** Если ещё остались открытые проекты */
            if (ui->tabWidget->count() > 2)
            {
                /** Если есть проекты справа */
                qDebug() << ui->tabWidget->count() << " > " << index+1;
                if (ui->tabWidget->count() > index+2)
                {
                    on_tabWidget_tabBarClicked(index+1);
                }
                else
                {
                    /** Если нет проектов справа, открываем проект слева */
                    on_tabWidget_tabBarClicked(index-1);
                    ui->tabWidget->setCurrentIndex(index-1);
                }
            }
            cmw->close_project(index);
        }
        ui->tabWidget->removeTab(index);
    }
}

/* Установка флага выполненной задачи */
void MainWindow::on_checkBoxDone_clicked()
{
    qDebug() << "Функция MainWindow::on_checkBoxDone_clicked() запустилась";

    if (ui->checkBoxDone->isChecked())
    {
        cmw->tList.completeFlag[cmw->currIndexNum] = "1";
        QColor indColor("lightgreen");
        //ui->tableWidget->item(cmw->currTaskNum,0)->setBackgroundColor(indColor);
        ui->tableWidget->item(cmw->currTaskNum,1)->setBackgroundColor(indColor);
        ui->tableWidget->item(cmw->currTaskNum,2)->setBackgroundColor(indColor);
    }
    else
    {
        cmw->tList.completeFlag[cmw->currIndexNum] = "0";
        QColor indColor("lightyellow");
        //ui->tableWidget->item(cmw->currTaskNum,0)->setBackgroundColor(indColor);
        ui->tableWidget->item(cmw->currTaskNum,1)->setBackgroundColor(indColor);
        ui->tableWidget->item(cmw->currTaskNum,2)->setBackgroundColor(indColor);
    }
}

/* Открытие окна создания проекта */
void MainWindow::on_action_8_triggered()
{
    qDebug() << "Функция MainWindow::on_action_8_triggered() запустилась";
    cmw->create_project();
}

/* Открытие окна рассчёта текущего проекта */
void MainWindow::on_action_7_triggered()
{
    qDebug() << "Функция MainWindow::on_action_7_triggered() запустилась";
    dialog_claculateProj *dcp = new dialog_claculateProj;\
    dcp->initWindow();
}

/* Обработка изменения шрифта в описании задачи */
void MainWindow::on_comboBoxFontList_currentIndexChanged(const QString &arg1)
{
    qDebug() << "Функция MainWindow::on_comboBoxFontList_currentIndexChanged(const QString &arg1) запустилась";
    checkCurrentFontParam();
}

/* Обработка изменения размера шрифта */
void MainWindow::on_lineEditFontSize_editingFinished()
{
    qDebug() << "Функция MainWindow::on_lineEditFontSize_editingFinished() запустилась";
    checkCurrentFontParam();
}

/* Обработка кнопки жирного текста */
void MainWindow::on_toolButton_clicked()
{
    qDebug() << "Функция MainWindow::on_toolButton_clicked() запустилась";
    if (ui->toolButton->isChecked() == true)
    {
        ui->toolButton->setChecked(true);
    }
    else
    {
        ui->toolButton->setChecked(false);
    }
    checkCurrentFontParam();
}

/* Обработка кнопки наклонного текста */
void MainWindow::on_toolButtonNaklon_clicked()
{
    qDebug() << "Функция MainWindow::on_toolButtonNaklon_clicked() запустилась";

    if (ui->toolButtonNaklon->isChecked() == true)
    {
        ui->toolButtonNaklon->setChecked(true);
    }
    else
    {
        ui->toolButtonNaklon->setChecked(false);
    }
    checkCurrentFontParam();
}

/* Обработка кнопки подчеркнутого текста */
void MainWindow::on_toolButtonPodcherk_clicked()
{
    qDebug() << "Функция MainWindow::on_toolButtonNaklon_clicked() запустилась";
    if (ui->toolButtonPodcherk->isChecked() == true)
    {
        ui->toolButtonPodcherk->setChecked(true);
    }
    else
    {
        ui->toolButtonPodcherk->setChecked(false);
    }
    checkCurrentFontParam();
}

/* Обработка кнопки зачеркнутого текста */
void MainWindow::on_toolButtonZacherk_clicked()
{
    qDebug() << "Функция MainWindow::on_toolButtonNaklon_clicked() запустилась";
    if (ui->toolButtonZacherk->isChecked() == true)
    {
        ui->toolButtonZacherk->setChecked(true);
    }
    else
    {
        ui->toolButtonZacherk->setChecked(false);
    }
    checkCurrentFontParam();
}


/* Обновление параметров шрифта в зависимости от панели */
void MainWindow::checkCurrentFontParam()
{
    qDebug() << "Функция MainWindow::checkCurrentFontParam() запустилась";

    QFont newFont;
    QTextCharFormat textFormat;

    /** Задаём семейство шрифтов */
    newFont.setFamily(ui->comboBoxFontList->currentText());
    /** Задаём размер шрифта */
    newFont.setPixelSize(ui->lineEditFontSize->text().toInt());
    /** Задаём выделение жирным */
    if (ui->toolButton->isChecked() == true)
    {
        qDebug() << "Кнопка жирности нажата";
        newFont.setBold(true);
    }
    else
    {
        newFont.setBold(false);
    }
    /** Задаём выделение наклоном */
    if (ui->toolButtonNaklon->isChecked() == true)
    {
        newFont.setItalic(true);
    }
    else
    {
        newFont.setItalic(false);
    }
    /** Задаём выделение подчёркиванием */
    if (ui->toolButtonPodcherk->isChecked() == true)
    {
        newFont.setUnderline(true);
    }
    else
    {
        newFont.setUnderline(false);
    }
    /** Задаём выделение зачёркиванием */
    if (ui->toolButtonZacherk->isChecked() == true)
    {
        newFont.setStrikeOut(true);
    }
    else
    {
        newFont.setStrikeOut(false);
    }
    textFormat.setFont(newFont);
    ui->plainTextEdit->mergeCurrentCharFormat(textFormat);
}



/* Обработка события изменения позиции курсора в поле описания задачи */
void MainWindow::on_plainTextEdit_cursorPositionChanged()
{
    qDebug() << "Функция MainWindow::on_plainTextEdit_cursorPositionChanged() запустилась ";

    /** Определяем, выделен ли текст под курсором жирным */
    if (ui->plainTextEdit->textCursor().charFormat().font().bold())
    {
        ui->toolButton->setChecked(true);
    }
    else
    {
        ui->toolButton->setChecked(false);
    }

    /** Определяем, выделен ли текст под курсором наклоном */
    if (ui->plainTextEdit->textCursor().charFormat().font().italic())
    {
        ui->toolButtonNaklon->setChecked(true);
    }
    else
    {
        ui->toolButtonNaklon->setChecked(false);
    }

    /** Определяем, выделен ли текст под курсором подчёркиванием */
    if (ui->plainTextEdit->textCursor().charFormat().font().underline())
    {
        ui->toolButtonPodcherk->setChecked(true);
    }
    else
    {
        ui->toolButtonPodcherk->setChecked(false);
    }

    /** Определяем, выделен ли текст под курсором зачеркиванием */
    if (ui->plainTextEdit->textCursor().charFormat().font().strikeOut())
    {
        ui->toolButtonZacherk->setChecked(true);
    }
    else
    {
        ui->toolButtonZacherk->setChecked(false);
    }
}
