#ifndef COMMENTSDIALOG_H
#define COMMENTSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTextEdit>

#include "value.h"

namespace Ui {
    class CommentsDialog;
}

class CommentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommentsDialog(bool isReadOnly, QWidget *parent = 0);
    ~CommentsDialog();

    /* выходные данные */
    QString comments;

    void editValue(Value *value);

private:
    Ui::CommentsDialog *ui;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

};

#endif // COMMENTSDIALOG_H
