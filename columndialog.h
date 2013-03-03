#ifndef COLUMNDIALOG_H
#define COLUMNDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QAbstractButton>
#include <QDateTime>

#include "journals.h"
#include "column.h"

namespace Ui {
    class ColumnDialog;
}

class ColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnDialog(Journals *journals, QWidget *parent = 0);
    ~ColumnDialog();

    /* выходные данные */
    QString name;
    QDateTime date;
    int columnTypeId;
    QString description;

    /* функции */
    void editColumn(Column *column);

private:
    Ui::ColumnDialog *ui;

    /* автоматическое имя */
    QString autoName;

private slots:
    void on_labelNameAuto_linkActivated();
    void on_toolButtonAuto_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // COLUMNDIALOG_H
