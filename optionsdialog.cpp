#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "hextool.h"

optionsDialog::optionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionsDialog)
{
    ui->setupUi(this);
}
optionsDialog::optionsDialog(bool boptsl, bool boptsignext, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionsDialog)
{
    ui->setupUi(this);
    ui->checkBox_signext->setChecked(boptsignext);
    ui->checkBox_loadsave->setChecked(boptsl);

}

optionsDialog::~optionsDialog()
{
    delete ui;
}
bool optionsDialog::saveload()
{
    return (ui->checkBox_loadsave->isChecked());
}
bool optionsDialog::signext()
{
    return ui->checkBox_signext->isChecked();
}


