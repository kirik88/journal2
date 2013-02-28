#include "commentsdialog.h"
#include "ui_commentsdialog.h"

CommentsDialog::CommentsDialog(bool isReadOnly, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommentsDialog)
{
    ui->setupUi(this);

    // скрываем отметку, так как о ней ещё ничего не известно
    ui->frameMark->hide();

    // если данные доступны только для чтения
    if (isReadOnly)
    {
        ui->buttonBox->clear();
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);

        ui->textEditComments->setReadOnly(true);
    }
}

CommentsDialog::~CommentsDialog()
{
    delete ui;
}

// функция вводит окно в режим редактировая отметки
void CommentsDialog::editValue(Value *value)
{
    // если отметка известна, отображаем её
    if (value->value != "")
    {
        ui->labelMark->setText(value->value);
        ui->frameMark->show();
    }

    // комментарий
    ui->textEditComments->setText(value->description);
}

// реакция на нажатие кнопки "OK"
void CommentsDialog::on_buttonBox_accepted()
{
    this->comments = ui->textEditComments->toPlainText();

    this->accept();
}

// реакция на нажатие кнопки "Отмена"
void CommentsDialog::on_buttonBox_rejected()
{
    this->reject();
}
