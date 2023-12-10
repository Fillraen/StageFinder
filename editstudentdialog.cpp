#include "editstudentdialog.h"
#include "ui_editstudentdialog.h"

EditStudentDialog::EditStudentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditStudentDialog)
{
    ui->setupUi(this);
}

EditStudentDialog::~EditStudentDialog()
{
    delete ui;
}
