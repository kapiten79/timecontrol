#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi         (this)  ;
    this->showMaximized();
    cmw->model = this;

    /** Параметры главного окна */
    this->setWindowTitle("Контроль времени.  v 2.00.001");

    /** Получаем ширину поля таблицы и высчитываем ширину поля
     * заголовка в процентном соотношении */

    int fullWidth, nameWidth;
    fullWidth = ui->tableForTasks->width();
    nameWidth = fullWidth - timeColumnWidth - priceColumnWidth;

    /** Ширина столбцов */
    ui->tableForTasks->setColumnWidth(0,nameWidth);
    ui->tableForTasks->setColumnWidth(1,timeColumnWidth);
    ui->tableForTasks->setColumnWidth(2,priceColumnWidth);
    ui->tableForTasks->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableForTasks->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableForTasks->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

    /** Добавление иконки создания проекта */
    QAction *newProjAct = addToolbarAction(":/ico/png_ico/new_proj.png","Создать проект",true) ;
    connect(newProjAct, &QAction::triggered, cmw, &controller_mainWindow::create_project) ;

    /** Добавление иконки немедленного сохранения информации */
    QAction *saveAct = addToolbarAction(":/ico/png_ico/save_all.png","Сохранить все данные...") ;
    connect(saveAct, &QAction::triggered, cmw, &controller_mainWindow::save_project);

    /** Добавление иконки открытия настроек проекта */
    QAction *settingsProjAct = addToolbarAction(":/ico/png_ico/settings_proj.png","Редактирование параметров проекта...") ;
    connect(settingsProjAct, &QAction::triggered, cmw, &controller_mainWindow::open_projSettings);

    ui->mainToolBar->addSeparator();

    /** Добавление иконки нового звонка */
    QAction *newCallAct = addToolbarAction(":/ico/png_ico/new_call.png","Регистрация нового звонка...") ;
    connect(newCallAct, &QAction::triggered, cmw, &controller_mainWindow::open_createCallDialog);

    /** Добавление иконки истории звонков */
    QAction *historyCallAct = addToolbarAction(":/ico/png_ico/history_call.png","История звонков...") ;
    connect(historyCallAct, &QAction::triggered, cmw, &controller_mainWindow::open_viewCallDialog);

    ui->mainToolBar->addSeparator();

    /** Добавление иконки поиска */
    QAction *findAct = addToolbarAction(":/ico/png_ico/find.png","Глобавльный поиск...") ;
    connect(findAct, &QAction::triggered, cmw, &controller_mainWindow::open_dialogFind);

    ui->mainToolBar->addSeparator();

    /** Добавление иконки переноса задачи на другую дату */
    QAction *perenosAct = addToolbarAction(":/ico/png_ico/perenos.png","Перенести задачу на другую дату...") ;
    connect(perenosAct, &QAction::triggered, this, &MainWindow::set_nextDay);

    /** Описание иконок кнопок задач */
    ui->buttonNewTask->setIcon(QIcon(":/ico/png_ico/new_task.png"));
    ui->buttonStartTaskTimer->setIcon(QIcon(":/ico/png_ico/play.png"));
    ui->buttonStopTaskTimer->setIcon(QIcon(":/ico/png_ico/stop.png"));
    ui->buttonDeleteTask->setIcon(QIcon(":/ico/png_ico/delete.png"));

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

MainWindow::~MainWindow()
{
    cmw->saveDay();
    cmw->dps->close();
    cmw->dct->close();
    cmw->di->close();

    delete ui;
}

/*!
 * \brief MainWindow::addToolbarAction - добавление иконок toolbar
 * \param iconUrl - URL иконки
 * \param descriptionText - текст описания действия (подсказка)
 * \param enabled - значение setEnabled по умолчанию для данной иконки
 * \return
 */
QAction* MainWindow::addToolbarAction(QString iconUrl, QString descriptionText, bool enabled)
{
    const QIcon newIcon = QIcon(iconUrl)                               ;
    QAction     *newAct = new QAction(newIcon, descriptionText, this)  ;
    ui->mainToolBar->addAction(newAct)                                 ;
    newAct->setEnabled(enabled);
    return newAct;
}

/*!
 * \brief MainWindow::on_pushButton_clicked - обработка нажатия кнопки новой задачи
 */
void MainWindow::on_buttonNewTask_clicked()
{
    if (ui->tabWidget->count() > 1) {
        if (ui->tableForTasks->rowCount() > 0)
            cmw->saveDay();
        cmw->open_createTaskDialog();
    }
}

/*!
 * \brief MainWindow::reloadTaskList - добавление новой записи о задаче
 */
void MainWindow::reloadTaskList()
{
    /** Перебираем задачи и заполняем строки таблицы.
        Так как список задач обновляется на уровне глобальных переменных,
        каждый раз чистим таблицу перед заполнением */

    ui->tableForTasks->setRowCount(0);

    for (int i=0; i<cmw->tList.taskName.count(); i++)
    {
        if (cmw->tList.taskDate[i].toString() == cmw->calendarDate.toString("dd_MM_yyyy"))
        {
            /** Сохранение "Начальных" данных о задаче в файл */
            /** Добавление новой строки для отображения задачи */
            int newRowNum = ui->tableForTasks->rowCount();
            ui->tableForTasks->insertRow(newRowNum);

            /** Запрещаем редактирование времени и цены задачи */
            QTableWidgetItem * taskNameCol              = new QTableWidgetItem(cmw->tList.taskName[i].toString() );
            QTableWidgetItem * timerCol                 = new QTableWidgetItem(cmw->tList.taskTime[i].toString() );
            QTableWidgetItem * priceCol                 = new QTableWidgetItem(cmw->tList.taskPrice[i].toString());

            if (cmw->tList.taskTime[i].toString() == "00:00:00")
            {
                /** Визуальное отображение только что созданной задачи */
                QColor indColor(255, 117, 24);
                timerCol->setBackgroundColor(indColor);
                priceCol->setBackgroundColor(indColor);
            }
            else
            {
                /** Визуальное отображение начала работы над задачей */
                QColor indColor("lightyellow");
                timerCol->setBackgroundColor(indColor);
                priceCol->setBackgroundColor(indColor);
            }

            if (cmw->tList.completeFlag[i].toString() == "1")
            {
                QColor indColor("lightgreen");
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

            ui->toolButton          ->setChecked(false);
            ui->toolButtonNaklon    ->setChecked(false);
            ui->toolButtonPodcherk  ->setChecked(false);
            ui->toolButtonZacherk   ->setChecked(false);
            ui->comboBoxFontList    ->setCurrentText("Arial");
            ui->lineEditFontSize    ->setText("14");

            ui->tableForTasks->setItem(newRowNum,taskColumn   ,taskNameCol        );
            ui->tableForTasks->setItem(newRowNum,timeColumn   ,timerCol           );
            ui->tableForTasks->setItem(newRowNum,priceColumn  ,priceCol           );

            QList<QTableWidgetItem *> selectedItems = ui->tableForTasks->selectedItems();
            for (int i=0; i<selectedItems.count(); i++)
            {
                selectedItems[i]->setSelected(false);
                selectedItems[i]->setSelected(false);
                selectedItems[i]->setSelected(false);
            }

            ui->tableForTasks->item(newRowNum,taskColumn)->setSelected(true);
            ui->tableForTasks->item(newRowNum,timeColumn)->setSelected(true);
            ui->tableForTasks->item(newRowNum,priceColumn)->setSelected(true);

            ui->plainTextEdit->document()->setHtml(cmw->tList.taskFullText[cmw->currIndexNum].toString());
            ui->plainTextEdit->setFocus();

            if (cmw->tList.completeFlag[cmw->currIndexNum] == "1")
                ui->checkBoxDone->setChecked(true);
        }

        ui->buttonStartTaskTimer->setEnabled(ui->tableForTasks->rowCount());
        ui->buttonDeleteTask->setEnabled(ui->tableForTasks->rowCount());

        if(!ui->tableForTasks->rowCount())
            ui->buttonStopTaskTimer->setEnabled(false);
    }
}

/*!
 * \brief MainWindow::on_buttonStartTaskTimer_clicked - запуск таймера задачи
 */
void MainWindow::on_buttonStartTaskTimer_clicked()
{
    cmw->stopTaskTimer();

    /** Визуальное отображение начала работы над задачей */
    int currRow = cmw->currTaskNum;
    QColor indColor("lightyellow");
    ui->tableForTasks->item(currRow,taskColumn)->setBackgroundColor(indColor);
    ui->tableForTasks->item(currRow,timeColumn)->setBackgroundColor(indColor);
    ui->tableForTasks->item(currRow,priceColumn)->setBackgroundColor(indColor);

    if (ui->tableForTasks->rowCount())
    {
        /** Определяем значения таймера в выделенной строке */
        cmw->tList.taskTime[cmw->currIndexNum] = QVariant(ui->tableForTasks->item(cmw->currTaskNum,1)->text());
        cmw->startTaskTimer();

        /** Делаем кнопку запустить не доступной, а кнопку остановить доступной */
        ui->buttonStartTaskTimer->setEnabled(false);
        ui->buttonStopTaskTimer->setEnabled(true);
    }
}

/* Выводим значения времени */
void MainWindow::setTaskTime()
{
    ui->tableForTasks->item(cmw->currTaskNum,timeColumn)->setText(cmw->tList.taskTime[cmw->currIndexNum].toString());
}
/* Отображение стоимости задачи */
void MainWindow::setTaskPrice()
{
    ui->tableForTasks->item(cmw->currTaskNum,priceColumn)->setText(cmw->tList.taskPrice[cmw->currIndexNum].toString());
}

/* Подсчёт суммарного времени для всех задач */
void MainWindow::setDayTime()
{
    ui->labelTotalTime->setText(cmw->allTime);
    ui->labelDayTime->setText(cmw->dayTime);
}

/* Подсчёт суммарной стоимости всех задач */
void MainWindow::setDayPrice()
{
    ui->labelTotalPrice->setText(cmw->allPrice);
    ui->labelDayPrice->setText(cmw->dayPrice);
}

/* Остановка текущего таймера */
void MainWindow::on_buttonStopTaskTimer_clicked()
{
    if (ui->tableForTasks->rowCount())
    {
        cmw->stopTaskTimer();
        /** После остановки таймера задачи делаем кнопку "Запустить" доступной, а "Остановить" не доступной */
        ui->buttonStartTaskTimer->setEnabled(true);
        ui->buttonStopTaskTimer->setEnabled(false);
    }
}

/* Изменение текста подробного описания задачи */
void MainWindow::on_plainTextEdit_textChanged()
{
    if (ui->tableForTasks->rowCount() > 0 && ui->tableForTasks->rowCount() > cmw->currTaskNum)
    {
        cmw->tList.taskFullText[cmw->currIndexNum] = ui->plainTextEdit->document()->toHtml();
        if (cmw->tList.completeFlag[cmw->currIndexNum] == "1")
            ui->checkBoxDone->setChecked(true);
        else
            ui->checkBoxDone->setChecked(false);
    }
}


/* Действия при выборе строки в таблице */
void MainWindow::on_tableForTasks_cellClicked(int row, int)
{
    cmw->saveDay();
    cmw->currTaskNum = row;
    cmw->currIndexNum = cmw->indexNumList[row].toString().toInt();
    ui->plainTextEdit->document()->setHtml(cmw->tList.taskFullText[cmw->currIndexNum].toString());
    ui->plainTextEdit->setFocus();
}

/* Нажатие кнопки "Удалить" */
void MainWindow::on_buttonDeleteTask_clicked()
{
    if (ui->tableForTasks->rowCount())
        cmw->open_removeTaskDialog();
}
/* Удаление задачи из интерфейса*/
void MainWindow::removeCurrTask()
{
    ui->tableForTasks->removeRow(cmw->currTaskNum);
    ui->plainTextEdit->clear    () ;

    ui->buttonStartTaskTimer->setEnabled(ui->tableForTasks->rowCount());
    ui->buttonDeleteTask->setEnabled(ui->tableForTasks->rowCount());
    if(!ui->tableForTasks->rowCount())
    {
        ui->buttonStopTaskTimer->setEnabled(false);
    }
}

/* Выбор даты в календаре */
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    cmw ->stopTaskTimer();
    ui  ->tableForTasks->clearContents();
    cmw ->calendarDate = date;
    ui  ->plainTextEdit->clear();
    cmw ->setDay(1);
}

void MainWindow::on_projectSettings_triggered()
{
    cmw->open_projSettings(true);
}

/* Выбор закладки проекта */
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    /** Если это не закладка откртыия проекта */
    if (ui->tabWidget->tabText(index) != "Выберите папку проекта двойным щелчком ...")
    {
        /** Останавливаем счётчик времени текущего проекта */
        cmw->stopTaskTimer();
        currentTab  = index;
        QString dir = cmw->projList[index].toString();
        if (dir != "")
        {
            mainDir.setCurrent(*cmw->workDir);
            mainDir.cdUp();
            mainDir.cd(dir);
            cmw->workDir->clear();
            cmw->workDir = new QString(mainDir.path());
            ui->tableForTasks->clearContents();
            ui->plainTextEdit->clear();
            cmw->setTodayDateTime();
            ui->calendarWidget->setSelectedDate(cmw->todayDate);
            cmw->setDay(1);
        }
    }
}

/* Открытие проекта по двойному щелчку на закладке */
void MainWindow::on_tabWidget_tabBarDoubleClicked(int)
{
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

    /** Присваиваем имя проекта переменной workDir */
    delete(cmw->workDir);
    cmw->workDir = new QString(dir);

    /** Запускаем функцию открытия проекта */
    open_project();
}

/* Открытие новой закладки с проектом */
void MainWindow::open_project()
{
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
        ui->tableForTasks   ->setEnabled(true);
        ui->calendarWidget  ->setEnabled(true);
        ui->plainTextEdit   ->setEnabled(true);
        ui->checkBoxDone    ->setEnabled(true);

        /** Открываем файл проекта для получения его наименования */
        cmw->open_project();
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
        ui->tableForTasks->clearContents();
        ui->plainTextEdit->clear();

        /** Загружаем данные по заданному в календаре дню проекта */
        cmw->setDay();

        /** Активируем кнопки управления задачами */
        ui->buttonNewTask->setEnabled(true);

        ui->buttonStartTaskTimer->setEnabled(ui->tableForTasks->rowCount());
        ui->buttonDeleteTask->setEnabled(ui->tableForTasks->rowCount());

        if(ui->tableForTasks->rowCount() > 0)
        {
            ui->buttonStopTaskTimer->setEnabled(false);
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
    QFont           charFont    ;
    charFont.setBold(true);
    charFont.setPixelSize(12);

    QTextCharFormat charFormat  ;
    charFormat.setFont(charFont);

    ui->calendarWidget->setDateTextFormat(editedDate, charFormat);
}

void MainWindow::setClearCalendar()
{
    QDate editedDate;
    QTextCharFormat charFormat;
    ui->calendarWidget->setDateTextFormat(editedDate, charFormat);
}

/* Открытие окна создания нового звонка */
void MainWindow::on_action_triggered()
{
    cmw->open_createCallDialog(true);
}

/* Открытие окна просмотра списка звонков для данного проекта */
void MainWindow::on_action_2_triggered()
{
    cmw->open_viewCallDialog(true);
}

/* Открытие окна поиска по всем данным (общий поиск) */
void MainWindow::on_action_3_triggered()
{
    cmw->open_dialogFind(true);
}

/* Обновление содержимого окна при переключении между месяцами и годами */
void MainWindow::on_calendarWidget_currentPageChanged(int year, int month)
{
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
void MainWindow::set_nextDay(bool)
{
    ui->menuBar                 ->setEnabled(false);
    ui->mainToolBar             ->setEnabled(false);
    ui->plainTextEdit           ->setEnabled(false);
    ui->checkBoxDone            ->setEnabled(false);
    ui->buttonNewTask           ->setEnabled(false);
    ui->buttonStartTaskTimer    ->setEnabled(false);
    ui->buttonStopTaskTimer     ->setEnabled(false);
    ui->buttonDeleteTask        ->setEnabled(false);
    ui->tabWidget               ->setEnabled(false);
}

/* Выбор пункта меню "Отчёты -> По времени" */
void MainWindow::on_action_5_triggered()
{
    Dialog_timeReport *tr = new Dialog_timeReport;
    tr->ctr->workDir = cmw->workDir;
    tr->setWindowTitle("Отчёт по времени");
    tr->init_window();
}


/* Выбор пункта "По выполненным задачам" меню "Отчёты" */
void MainWindow::on_action_6_triggered()
{
    Dialog_reportTask *drt = new Dialog_reportTask;
    drt->crt.workDir = cmw->workDir;
    drt->setWindowTitle("Отчёт по выполненным заданиям за период");
    drt->init_window();
}

/* Кнопка закрытия закладки с проектом */
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (ui->tabWidget->tabText(index) != "Выберите папку проекта двойным щелчком ...")
    {
        /** Открываем проект на случай, если выбрана не активная закладка */
        on_tabWidget_tabBarClicked(index);
        ui->tableForTasks->setRowCount(0);
        ui->plainTextEdit->clear();
        setClearCalendar();

        /** Если все закладки с проектами закрыты, блокируем кнопки управления проектами */
        qDebug() << ui->tabWidget->count();
        if (ui->tabWidget->count() == 1)
        {
            ui->buttonNewTask       ->setEnabled(false);
            ui->buttonStartTaskTimer->setEnabled(false);
            ui->buttonStopTaskTimer ->setEnabled(false);
            ui->buttonDeleteTask    ->setEnabled(false);
            /** Блокируем календарь, таблицу и поле описания задачи */
            ui->tableForTasks   ->setEnabled(false);
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
    QColor indColor;
    if (ui->checkBoxDone->isChecked())
    {
        cmw->tList.completeFlag[cmw->currIndexNum] = "1";
        indColor.setNamedColor("lightgreen");
    }
    else
    {
        cmw->tList.completeFlag[cmw->currIndexNum] = "0";
        indColor.setNamedColor("lightyellow");
    }
    ui->tableForTasks->item(cmw->currTaskNum,1)->setBackgroundColor(indColor);
    ui->tableForTasks->item(cmw->currTaskNum,2)->setBackgroundColor(indColor);
}

/* Открытие окна создания проекта */
void MainWindow::on_action_8_triggered()
{
    cmw->create_project();
}

/* Открытие окна рассчёта текущего проекта */
void MainWindow::on_action_7_triggered()
{
    dialog_claculateProj *dcp = new dialog_claculateProj;
    dcp->initWindow();
}

/* Обработка изменения шрифта в описании задачи */
void MainWindow::on_comboBoxFontList_currentIndexChanged(const QString &)
{
    checkCurrentFontParam();
}

/* Обработка изменения размера шрифта */
void MainWindow::on_lineEditFontSize_editingFinished()
{
    checkCurrentFontParam();
}

/* Обработка кнопки жирного текста */
void MainWindow::on_toolButton_clicked()
{
    ui->toolButton->setChecked(ui->toolButton->isChecked());
    checkCurrentFontParam();
}

/* Обработка кнопки наклонного текста */
void MainWindow::on_toolButtonNaklon_clicked()
{
    ui->toolButtonNaklon->setChecked(ui->toolButtonNaklon->isChecked());
    checkCurrentFontParam();
}

/* Обработка кнопки подчеркнутого текста */
void MainWindow::on_toolButtonPodcherk_clicked()
{
    ui->toolButtonPodcherk->setChecked(ui->toolButtonPodcherk->isChecked());
    checkCurrentFontParam();
}

/* Обработка кнопки зачеркнутого текста */
void MainWindow::on_toolButtonZacherk_clicked()
{
    ui->toolButtonZacherk->setChecked(ui->toolButtonZacherk->isChecked());
    checkCurrentFontParam();
}


/* Обновление параметров шрифта в зависимости от панели */
void MainWindow::checkCurrentFontParam()
{
    QFont newFont;
    QTextCharFormat textFormat;

    /** Задаём семейство шрифтов */
    newFont.setFamily(ui->comboBoxFontList->currentText());
    /** Задаём размер шрифта */
    newFont.setPixelSize(ui->lineEditFontSize->text().toInt());
    /** Задаём выделение жирным */
    newFont.setBold(ui->toolButton->isChecked());
    /** Задаём выделение наклоном */
    newFont.setItalic(ui->toolButtonNaklon->isChecked());
    /** Задаём выделение подчёркиванием */
    newFont.setUnderline(ui->toolButtonPodcherk->isChecked());
    /** Задаём выделение зачёркиванием */
    newFont.setStrikeOut(ui->toolButtonZacherk->isChecked());
    textFormat.setFont(newFont);
    ui->plainTextEdit->mergeCurrentCharFormat(textFormat);
}

/* Обработка события изменения позиции курсора в поле описания задачи */
void MainWindow::on_plainTextEdit_cursorPositionChanged()
{
    /** Определяем, выделен ли текст под курсором жирным */
    ui->toolButton->setChecked(ui->plainTextEdit->textCursor().charFormat().font().bold());
    /** Определяем, выделен ли текст под курсором наклоном */
     ui->toolButtonNaklon->setChecked(ui->plainTextEdit->textCursor().charFormat().font().italic());
    /** Определяем, выделен ли текст под курсором подчёркиванием */
     ui->toolButtonPodcherk->setChecked(ui->plainTextEdit->textCursor().charFormat().font().underline());
    /** Определяем, выделен ли текст под курсором зачеркиванием */
     ui->toolButtonZacherk->setChecked(ui->plainTextEdit->textCursor().charFormat().font().strikeOut());
}

/* Обработка нажатия кнопки выравнивания по левому краю */
void MainWindow::on_toolButtonLeft_clicked()
{
    QTextOption op(Qt::AlignLeft);
    ui->plainTextEdit->document()->setDefaultTextOption(op);
}

/* Обработка нажатия кнопки выравнивания по центру */
void MainWindow::on_toolButtonCenter_clicked()
{
    QTextOption op(Qt::AlignCenter);
    ui->plainTextEdit->document()->setDefaultTextOption(op);
}

/* Обработка нажатия кнопки выравнивания по правому краю */
void MainWindow::on_toolButtonRight_clicked()
{
    QTextOption op(Qt::AlignRight);
    ui->plainTextEdit->document()->setDefaultTextOption(op);
}
