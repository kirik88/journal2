#include "journaldialog.h"
#include "ui_journaldialog.h"

JournalDialog::JournalDialog(Journals *journals, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JournalDialog)
{
    ui->setupUi(this);

    if (!journals || !journals->loader->user) return;

    User *user = journals->loader->user;

    // скрываем виджеты ввода наименования
    ui->labelName->hide();
    ui->lineEditName->hide();

    // заполняем список классов
    ui->comboBoxClass->clear();
    ui->comboBoxClass->addItem("<Без класса>", -1);
    for (int i = 0; journals->classes->count() > i; i++)
    {
        Class *cl = journals->classes->at(i);

        // для админа доступны все классы, поэтому к наименованиям приписываем учебный год
        QString classTitle = (user->userType == utAdmin) ? QString("%1 (%2-%3)").arg(cl->getName()).arg(cl->startYear).arg(cl->startYear + 1) : cl->getName();

        ui->comboBoxClass->addItem(classTitle, cl->getId());
    }

    // заполняем список предментов
    ui->comboBoxCourse->clear();
    ui->comboBoxCourse->addItem("<Без предмета>", -1);
    for (int i = 0; journals->courses->count() > i; i++)
    {
        Course *course = journals->courses->at(i);

        ui->comboBoxCourse->addItem(course->getName(), course->getId());
    }

    // учитель не может выставить другого учителя
    ui->comboBoxTeacher->clear();
    if (user->userType == utTeacher)
    {
        ui->comboBoxTeacher->addItem(user->getName(), user->getId());
        ui->comboBoxTeacher->setEnabled(false);
    }
    else
    {
        // заполняем список учителей
        ui->comboBoxTeacher->addItem("<Без учителя>", -1);
        for (int i = 0; journals->teachers->count() > i; i++)
        {
            User *user = journals->teachers->at(i);

            ui->comboBoxTeacher->addItem(user->getName(), user->getId());
        }
    }
}

JournalDialog::~JournalDialog()
{
    delete ui;
}

// функция вводит окно в режим редактирования журнала
void JournalDialog::editJournal(Journal *journal)
{
    // заголовок
    this->setWindowTitle(tr("Редактирование журнала «%1»").arg(journal->getName().replace("\n", " ")));

    // наименование
    if (journal->name != "")
    {
        ui->labelNameAuto->hide();
        ui->labelName->show();
        ui->lineEditName->show();
        ui->lineEditName->setText(journal->name);
    }

    // класс
    if (journal->classId > 0)
    {
        int i = ui->comboBoxClass->findData(journal->classId);
        if (i >= 0) ui->comboBoxClass->setCurrentIndex(i);
    }

    // предмет
    if (journal->courseId > 0)
    {
        int i = ui->comboBoxCourse->findData(journal->courseId);
        if (i >= 0) ui->comboBoxCourse->setCurrentIndex(i);
    }

    // учитель
    if (journal->teacherId > 0)
    {
        int i = ui->comboBoxTeacher->findData(journal->teacherId);
        if (i >= 0) ui->comboBoxTeacher->setCurrentIndex(i);
    }

    // описание
    ui->plainTextEditDescription->setPlainText(journal->description);
}

// реакция на нажатие кнопки "ввести наименование вручную"
void JournalDialog::on_labelNameAuto_linkActivated()
{
    // скрываем надпись
    ui->labelNameAuto->hide();

    // показываем виджеты ввода наименования
    ui->labelName->show();
    ui->lineEditName->show();

    // ставим фокус
    ui->lineEditName->setFocus();
}

// реакция на нажатие кнопки "OK"
void JournalDialog::on_buttonBox_accepted()
{
    name = ui->lineEditName->text();
    classId = ui->comboBoxClass->itemData(ui->comboBoxClass->currentIndex()).toInt();
    courseId = ui->comboBoxCourse->itemData(ui->comboBoxCourse->currentIndex()).toInt();
    teacherId = ui->comboBoxTeacher->itemData(ui->comboBoxTeacher->currentIndex()).toInt();
    description = ui->plainTextEditDescription->toPlainText();

    this->accept();
}

// реакция на нажатие кнопки "Отмена"
void JournalDialog::on_buttonBox_rejected()
{
    this->reject();
}
