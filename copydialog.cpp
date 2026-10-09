#include "copydialog.h"
#include "ui_copydialog.h"
#include "hextool.h"

copyDialog::copyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copyDialog)
{
    ui->setupUi(this);
}
copyDialog::copyDialog(QString shex, QString soct, QString sdec, QString sip, QString stext, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copyDialog)
{
    ui->setupUi(this);
    ui->radioButton_hex->setText("Hex: " + shex);
    ui->radioButton_oct->setText("Oct: " + soct);
    ui->radioButton_dec->setText("Dec: " + sdec);
    ui->radioButton_ip->setText("Ip: " + sip);
    ui->radioButton_unicode->setText("Unicode: " + stext);
}

copyDialog::~copyDialog()
{
    delete ui;
}

QString copyDialog::getaction()
{
    return ui->buttonGroup->checkedButton()->text();
}
