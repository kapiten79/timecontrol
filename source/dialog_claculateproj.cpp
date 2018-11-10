#include "dialog_claculateproj.h"
#include "ui_dialog_claculateproj.h"

dialog_claculateProj::dialog_claculateProj(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialog_claculateProj)
{
    ui->setupUi(this);
}

dialog_claculateProj::~dialog_claculateProj()
{
    delete ui;
}

/* Инициализация окна калькулятора */
void dialog_claculateProj::initWindow()
{
    qDebug() << "Функция dialog_claculateProj::initWindow() запустилась";

    ui->treeWidgetProject->setCurrentItem(ui->treeWidgetProject->topLevelItem(0), 0);
    ui->labelProjNameText->setText(ui->treeWidgetProject->currentItem()->text(0));
    ui->labelParentTaskName->setText("Нет");
    ui->lineEditCurrTask->setText(ui->treeWidgetProject->currentItem()->text(0));
    this->setWindowTitle("Калькулятор проекта");
    this->show();


}

/* Добавляем задачу в дерево */
void dialog_claculateProj::on_pushButtonAddTask_clicked()
{
    qDebug() << "Функция dialog_claculateProj::on_pushButtonAddTask_clicked() запустилась";

    /** Получаем текущий выделеный элемент дерева */
    if (ui->treeWidgetProject->topLevelItem(0)->text(0) == ui->treeWidgetProject->currentItem()->text(0))
    {
        qDebug() << "Это корневая группа, она может быть только одна";
    }
    else
    {
        QTreeWidgetItem *parentTaskItem = ui->treeWidgetProject->currentItem()->parent();

        qDebug() << "Текст выбранного элемента " << ui->treeWidgetProject->currentItem()->text(0);
        qDebug() << "Текст родительского элемента дерева" << ui->treeWidgetProject->currentItem()->parent();

        /** Добавляем элемент в этуже ветку дерева */
        QTreeWidgetItem *nItem = new QTreeWidgetItem(parentTaskItem);
        nItem->setText(0, "Новая задача");
        ui->treeWidgetProject->insertTopLevelItem(parentTaskItem->indexOfChild(nItem), nItem);
        ui->treeWidgetProject->clearSelection();
        ui->treeWidgetProject->setCurrentItem(nItem,0);
        nItem->setSelected(true);

        /** Выводим свойства элемента */
        ui->lineEditCurrTask->setText(nItem->text(0));

    }

}

/* Добавление подгруппы к выбранной группе */
void dialog_claculateProj::on_pushButtonAddSubTask_clicked()
{
    qDebug() << "Функция dialog_claculateProj::on_pushButtonAddSubTask_clicked() запустилась";
    QTreeWidgetItem *currentTaskItem = ui->treeWidgetProject->currentItem();

    qDebug() << "Текст выбранного элемента " << ui->treeWidgetProject->currentItem()->text(0);
    qDebug() << "Текст родительского элемента дерева" << ui->treeWidgetProject->currentItem()->parent();

    /** Добавляем элемент в этуже ветку дерева */
    QTreeWidgetItem *nItem = new QTreeWidgetItem();
    nItem->setText(0, "Новая задача");
    currentTaskItem->addChild(nItem);

    /** Открываем ветку дерева, котором создан элемент */
    currentTaskItem->setExpanded(true);

    /** Делаем элемент активным и выбранным */
    ui->treeWidgetProject->clearSelection();
    ui->treeWidgetProject->setCurrentItem(nItem,0);
    nItem->setSelected(true);

    /** Выводим свойства элемента */
    ui->labelParentTaskName->setText(currentTaskItem->text(0));
    ui->lineEditCurrTask->setText(nItem->text(0));
}

/* Обработка выбора элемента дерева */
void dialog_claculateProj::on_treeWidgetProject_clicked(const QModelIndex &)
{
    qDebug() << "Функция dialog_claculateProj::on_treeWidgetProject_clicked(const QModelIndex &index) запустилась";
    QString text = ui->treeWidgetProject->currentItem()->text(0);
    ui->lineEditCurrTask->setText(text);
    if (ui->treeWidgetProject->topLevelItem(0)->text(0) == ui->treeWidgetProject->currentItem()->text(0))
    {
        ui->labelParentTaskName->setText("Нет");
    }
    else
    {
        ui->labelParentTaskName->setText(ui->treeWidgetProject->currentItem()->parent()->text(0));
    }
}

/* Изменение наименования задачи в дереве при его изменении в свойствах задачи */
void dialog_claculateProj::on_lineEditCurrTask_textEdited(const QString &arg1)
{
    qDebug() << "Функция dialog_claculateProj::on_lineEditCurrTask_textEdited(const QString &arg1) запустилась";
    ui->treeWidgetProject->currentItem()->setText(0, arg1);
}
