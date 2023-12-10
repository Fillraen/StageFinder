#ifndef EDITSTUDENTDIALOG_H
#define EDITSTUDENTDIALOG_H

#include <QDialog>
#include "Model/Etudiant.h"

namespace Ui {
class EditStudentDialog;
}

class EditStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditStudentDialog(QWidget *parent = nullptr);
    ~EditStudentDialog();

private slots:
    void on_modifyButton_clicked();
    void on_cancelButton_clicked();
    void on_deleteButton_clicked();

private:
    Ui::EditStudentDialog *ui;
    Etudiant etudiant;
};

#endif // EDITSTUDENTDIALOG_H
