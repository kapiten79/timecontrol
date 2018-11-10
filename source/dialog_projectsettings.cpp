#include "dialog_projectsettings.h"
#include "ui_dialog_projectsettings.h"

Dialog_projectSettings::Dialog_projectSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_projectSettings)
{
    ui->setupUi(this);
    cps.model = this;

}

Dialog_projectSettings::~Dialog_projectSettings()
{

}

void Dialog_projectSettings::init_window(QString p_caller)
{
    qDebug() << "Функция Dialog_projectSettings::init_window() запустилась " << caller;
    this->open();

    cps.model = this;
    caller = p_caller;
    QDir upDir;

    if (caller == "createNewProj")
    {
        this->setWindowTitle("Параметры проекта нового проекта");
        ui->lineEdit            ->setEnabled(true);
        ui->pushButton          ->setEnabled(true);
        ui->lineEditProjectName ->setEnabled(true);
        qDebug() << *cps.workDir << " != " << QDir::homePath();
        if (*cps.workDir != QDir::homePath())
        {
            qDebug() << "Это не каталог по умолчанию";
            upDir.setPath(*cps.workDir);
            upDir.cdUp();
            ui->lineEdit->setText(upDir.path());
        }
        else
        {
            ui->lineEdit->setText(*cps.workDir);
        }
        ui->lineEditProjectName             ->clear();
        ui->lineEditProjectPrice            ->clear();
        ui->plainTextEditProjectDescription ->clear();
        ui->lineEditContactName             ->clear();
        ui->lineEdiContactPthone            ->clear();
        ui->lineEditContactEmail            ->clear();
    }
    else
    {
        this->setWindowTitle("Параметры проекта '" + cps.pList->projNameSt+"'");
        ui->lineEdit            ->setEnabled(false);
        ui->pushButton          ->setEnabled(false);
        ui->lineEditProjectName ->setEnabled(false);
        ui->lineEditProjectName             ->setText       (cps.pList->projNameSt)          ;
        ui->lineEditProjectPrice            ->setText       (cps.pList->projPriceSt)         ;
        ui->plainTextEditProjectDescription ->setPlainText  (cps.pList->projDescriptionSt)   ;
        ui->lineEditContactName             ->setText       (cps.pList->projContactNameSt)   ;
        ui->lineEdiContactPthone            ->setText       (cps.pList->projPhoneSt)         ;
        ui->lineEditContactEmail            ->setText       (cps.pList->projEmailSt)         ;

        /** Если редактируются данные проекта, мне нужно указать в данной строке родительскую папку рабочего каталога */

        upDir.setPath(*cps.workDir);
        upDir.cdUp();
        ui->lineEdit->setText(upDir.path());
    }
}



void Dialog_projectSettings::on_buttonBoxAcceptDecline_accepted()
{
    qDebug() << "Функция Dialog_projectSettings::on_buttonBoxAcceptDecline_accepted() запустилась " << caller;
    if (ui->lineEditProjectName->text() != "")
    {
        cps.pList->projNameSt        = ui->lineEditProjectName               ->text          ();
        cps.pList->projPriceSt       = ui->lineEditProjectPrice              ->text          ();
        cps.pList->projDescriptionSt = ui->plainTextEditProjectDescription   ->toPlainText   ();
        cps.pList->projContactNameSt = ui->lineEditContactName               ->text          ();
        cps.pList->projPhoneSt       = ui->lineEdiContactPthone              ->text          ();
        cps.pList->projEmailSt       = ui->lineEditContactEmail              ->text          ();

        qDebug() << "Коллер " << caller;
        if (caller == "createNewProj")
        {

            cps.create_newProject();
            qDebug() << "Эммитируется сигнела s_setProjParams ";
            emit s_setProjParams();

        }
        else
        {

            cps.writeProjToJSON();
            connect (this, SIGNAL(s_setProjParams()), parentObj, SLOT(setProjParams()));
            emit s_setProjParams();
            disconnect (this, SIGNAL(s_setProjParams()), parentObj, SLOT(setProjParams()));
        }
    }
}



/* Нажатие кнопки выбора родительского каталога папки проекта */
void Dialog_projectSettings::on_pushButton_clicked()
{
    qDebug() << "Функция Dialog_projectSettings::on_pushButton_clicked() запустилась " << *cps.workDir;
    /** Открываем диалоговое окно выбора каталога */
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите каталог проекта или создайте новый ..."),
                                                    *cps.workDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    /** Указываем в окне новый каталог */
    ui->lineEdit->setText(dir);

    /** Присваиваем имя проекта переменной workDir */
    delete(cps.workDir);
    cps.workDir = new QString(dir);

}

/* Обработка нажатия кнопки отменя создания проекта */
void Dialog_projectSettings::on_buttonBoxAcceptDecline_rejected()
{
    qDebug() << "Dialog_projectSettings::on_buttonBoxAcceptDecline_rejected() " << *cps.workDir;
    //cps.workDir = "";

}
