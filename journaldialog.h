#ifndef JOURNALDIALOG_H
#define JOURNALDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QAbstractButton>

#include "journals.h"

namespace Ui {
    class JournalDialog;
}

class JournalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JournalDialog(Journals *journals, QWidget *parent = 0);
    ~JournalDialog();

    /* выхоные данные */
    QString name;
    int classId;
    int courseId;
    int teacherId;
    QString description;

private:
    Ui::JournalDialog *ui;

private slots:
    void on_labelNameAuto_linkActivated();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // JOURNALDIALOG_H
