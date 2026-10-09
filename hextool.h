/****************************************************************************

    FILE:    hextool.h

    COMMENTS:

 ****************************************************************************

    LICENSE:

    HexTool -  a simple program to convert numbers on the fly
    Copyright (C) 2003 Ryan Harkin, 2014 Christian Weniger; www.harkin.org/HexTool

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

****************************************************************************/
#ifndef HEXTOOL_H
#define HEXTOOL_H

#include <common/debug.h>
#include <common/handlebinarydigit.h>
#include <common/numbits.h>
#include <common/register.h>
#include <common/registeroperation.h>
#include <common/signed.h>
#include <common/value.h>

#include <QMainWindow>
//#include <QModelIndex>

namespace Ui
{
class HexTool;
}

class HexTool : public QMainWindow
{
    Q_OBJECT
public:

    explicit HexTool(QWidget *parent = 0);
    virtual ~HexTool();

    void showExpanded();
    bool optsl;
    bool optsignext;

public slots:
    void setclipboard(QString clip);

private slots:

    void groupButton_clicked(int);
    void about();
    void on_radioButton_32bit_clicked();
    void on_radioButton_16bit_clicked();
    void on_radioButton_8bit_clicked();
    void on_lineEdit_dec_textEdited();
    void on_lineEdit_hex_textEdited();

    void on_pushButton_not_pressed();
    void on_checkBox_signed_toggled(bool isSigned);

    void on_pushButton_bitNumbering_pressed();

    void on_pushButton_shiftLeft_pressed();
    void on_pushButton_shiftRight_pressed();
    void on_pushButton_endianSwap_pressed();

    void on_pushButton_AND_pressed();
    void on_pushButton_OR_pressed();
    void on_pushButton_XOR_pressed();
    void on_pushButton_plus_pressed();
    void on_pushButton_minus_pressed();
    void on_pushButton_multiply_pressed();
    void on_pushButton_divide_pressed();

    void on_lineEdit_ipaddress0_textEdited();
    void on_lineEdit_ipaddress1_textEdited();
    void on_lineEdit_ipaddress2_textEdited();
    void on_lineEdit_ipaddress3_textEdited();

    void on_lineEdit_ascii0_textEdited();
    void on_lineEdit_ascii1_textEdited();
    void on_lineEdit_ascii2_textEdited();
    void on_lineEdit_ascii3_textEdited();

    void on_pushButton_increase_clicked();
    void on_pushButton_decrease_clicked();

    void on_radioButton_registerA_clicked();
    void on_radioButton_registerB_clicked();
    void on_radioButton_registerC_clicked();

    void on_lineEdit_oct_textEdited();

    void on_pushButton_clr_clicked();
    void clearAll();
    void save_ini();
    void load_ini();
    void help();
    void optDialog();
    void showcopyDialog();

    void on_pushButton_neg_clicked();


private:

    void UpdateDisplay();
    void ToggleBit(int b);
    int  HandleOperation(eOpType op);
    void HandleIpAddrEditBox(int field, unsigned int val);
    void HandleAsciiEditBox(int field, unsigned int val);
    QAction *a_clear;
    QAction *a_about;
    QAction *a_save;
    QAction *a_load;
    QAction *a_help;
    QAction *a_opt;
    QAction *a_copy;

    Ui::HexTool *ui;

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // HEXTOOL_H
