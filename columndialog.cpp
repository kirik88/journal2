#include "columndialog.h"
#include "ui_columndialog.h"

ColumnDialog::ColumnDialog(Journals *journals, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnDialog)
{
    ui->setupUi(this);

    if (!journals) return;

    autoName = "";

    // скрываем виджеты ввода наименования
    ui->labelName->hide();
    ui->frameName->hide();

    // дата по умолчанию - сегодняшняя
    ui->calendarWidget->setSelectedDate(QDate::currentDate());

    // заполняем список типов колонок
    ui->comboBoxColumnType->clear();
    for (int i = 0; journals->columnTypes->count() > i; i++)
    {
        ColumnType *columnType = journals->columnTypes->at(i);

        ui->comboBoxColumnType->addItem(columnType->getName(), columnType->getId());
    }
    // по умолчанию выбран тип "урок"
    if (ui->comboBoxColumnType->count() > 0) ui->comboBoxColumnType->setCurrentIndex(0);
}

ColumnDialog::~ColumnDialog()
{
    delete ui;
}

// функция вводит окно в режим редактирования колонки
void ColumnDialog::editColumn(Column *column)
{
    // заголовок
    this->setWindowTitle(tr("Редактирование колонки «%1»").arg(column->getName().replace("\n", " ")));

    // наименование
    autoName = column->getName();
    if (column->name != "")
    {
        ui->labelNameAuto->hide();
        ui->labelName->show();
        ui->frameName->show();
        ui->lineEditName->setText(column->name);
    }

    // дата
    ui->calendarWidget->setSelectedDate(column->date.date());

    // тип колонки
    if (column->columnTypeId > 0)
    {
        int i = ui->comboBoxColumnType->findData(column->columnTypeId);
        if (i >= 0) ui->comboBoxColumnType->setCurrentIndex(i);
        else if (ui->comboBoxColumnType->count() > 0) ui->comboBoxColumnType->setCurrentIndex(0);
    }

    // описание
    ui->plainTextEditDescription->setPlainText(column->description);
}

// реакция на нажатие кнопки "ввести наименование вручную"
void ColumnDialog::on_labelNameAuto_linkActivated()
{
    // скрываем надпись
    ui->labelNameAuto->hide();

    // устанавливаем автоматически сформированое имя
    ui->lineEditName->setText(autoName);

    // показываем виджеты ввода наименования
    ui->labelName->show();
    ui->frameName->show();

    // ставим фокус
    ui->lineEditName->setFocus();
}

// реакция на нажатие кнопки "формировать название автоматически"
void ColumnDialog::on_toolButtonAuto_clicked()
{
    // показываем надпись
    ui->labelNameAuto->show();

    // удаляем имя
    ui->lineEditName->setText("");

    // скрываем виджеты ввода наименования
    ui->labelName->hide();
    ui->frameName->hide();
}

// реакция на нажатие кнопки "OK"
void ColumnDialog::on_buttonBox_accepted()
{
    name = ui->lineEditName->text();
    date.setDate(ui->calendarWidget->selectedDate());
    columnTypeId = ui->comboBoxColumnType->itemData(ui->comboBoxColumnType->currentIndex()).toInt();
    description = ui->plainTextEditDescription->toPlainText();

    this->accept();
}

// реакция на нажатие кнопки "Отмена"
void ColumnDialog::on_buttonBox_rejected()
{
    this->reject();
}
