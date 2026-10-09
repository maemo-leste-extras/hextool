#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>

namespace Ui
{
class copyDialog;
}

class copyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit copyDialog(QWidget *parent = 0);
    copyDialog(QString shex, QString soct, QString sdec, QString sip, QString stext, QWidget *parent = 0);
    ~copyDialog();
    QString getaction();

private:
    Ui::copyDialog *ui;

};

#endif // COPYDIALOG_H
