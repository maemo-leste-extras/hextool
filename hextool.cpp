/****************************************************************************

    FILE:    hextool.cpp

 ****************************************************************************

    LICENSE:

    HexTool -  a simple program to convert numbers on the fly
    Copyright (C) 2003 Ryan Harkin and 2014 Christian Weniger; www.harkin.org/HexTool

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
#include <stdio.h>

#include <common/debug.h>
#include <common/handlebinarydigit.h>
#include <common/numbits.h>
#include <common/register.h>
#include <common/registeroperation.h>
#include <common/signed.h>
#include <common/value.h>
#include <optionsdialog.h>
#include <copydialog.h>
#include <QClipboard>
#include "hextool.h"
#if defined(Q_WS_MAEMO_5)
#include "ui_hextool.h"
#include <QtMaemo5/QMaemo5InformationBox>
#else
#include "ui_hextool_desk.h"
#endif

#include <QtCore/QCoreApplication>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QCloseEvent>

HexTool::HexTool(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::HexTool)
{
    optsl=false;
    optsignext=false;
    SetSigned(UNSIGNED);
    SetNumBits(NUMBITS_32);
    SetRegister(REG_A);

    ui->setupUi(this);

    /************setting IDs for buttons*************/
    ui->buttonGroup->setId(ui->pushButton_bit1, 1);
    ui->buttonGroup->setId(ui->pushButton_bit2, 2);
    ui->buttonGroup->setId(ui->pushButton_bit3, 3);
    ui->buttonGroup->setId(ui->pushButton_bit4, 4);
    ui->buttonGroup->setId(ui->pushButton_bit5, 5);
    ui->buttonGroup->setId(ui->pushButton_bit6, 6);
    ui->buttonGroup->setId(ui->pushButton_bit7, 7);
    ui->buttonGroup->setId(ui->pushButton_bit8, 8);
    ui->buttonGroup->setId(ui->pushButton_bit9, 9);
    ui->buttonGroup->setId(ui->pushButton_bit10, 10);
    ui->buttonGroup->setId(ui->pushButton_bit11, 11);
    ui->buttonGroup->setId(ui->pushButton_bit12, 12);
    ui->buttonGroup->setId(ui->pushButton_bit13, 13);
    ui->buttonGroup->setId(ui->pushButton_bit14, 14);
    ui->buttonGroup->setId(ui->pushButton_bit15, 15);
    ui->buttonGroup->setId(ui->pushButton_bit16, 16);
    ui->buttonGroup->setId(ui->pushButton_bit17, 17);
    ui->buttonGroup->setId(ui->pushButton_bit18, 18);
    ui->buttonGroup->setId(ui->pushButton_bit19, 19);
    ui->buttonGroup->setId(ui->pushButton_bit20, 20);
    ui->buttonGroup->setId(ui->pushButton_bit21, 21);
    ui->buttonGroup->setId(ui->pushButton_bit22, 22);
    ui->buttonGroup->setId(ui->pushButton_bit23, 23);
    ui->buttonGroup->setId(ui->pushButton_bit24, 24);
    ui->buttonGroup->setId(ui->pushButton_bit25, 25);
    ui->buttonGroup->setId(ui->pushButton_bit26, 26);
    ui->buttonGroup->setId(ui->pushButton_bit27, 27);
    ui->buttonGroup->setId(ui->pushButton_bit28, 28);
    ui->buttonGroup->setId(ui->pushButton_bit29, 29);
    ui->buttonGroup->setId(ui->pushButton_bit30, 30);
    ui->buttonGroup->setId(ui->pushButton_bit31, 31);
    ui->buttonGroup->setId(ui->pushButton_bit32, 32);
    ui->rbuttonGroup_bitsize->setId(ui->radioButton_8bit, 8);
    ui->rbuttonGroup_bitsize->setId(ui->radioButton_16bit, 16);
    ui->rbuttonGroup_bitsize->setId(ui->radioButton_32bit, 32);
    ui->rbuttonGroup_reg->setId(ui->radioButton_registerA, 1);
    ui->rbuttonGroup_reg->setId(ui->radioButton_registerB, 2);
    ui->rbuttonGroup_reg->setId(ui->radioButton_registerC, 3);
    connect(ui->buttonGroup, SIGNAL(idClicked(int)), this, SLOT(groupButton_clicked(int)));
    UpdateDisplay();
    /********setting the menu********************************/
    /*QAction *clear = new QAction("ClearAll",this);
    QAction *about = new QAction("About",this);
    QAction *save = new QAction("Save",this);
    QAction *load = new QAction("Load",this);
    QAction *help = new QAction("Help",this);
    QAction *opt = new QAction("Options",this);
    QAction *copy = new QAction("Copy",this);*/
    a_clear = menuBar()->addAction(tr("ClearAll"));
    a_about = menuBar()->addAction(tr("About"));
    a_save = menuBar()->addAction(tr("Save"));
    a_load = menuBar()->addAction(tr("Load"));
    a_copy = menuBar()->addAction(tr("Copy"));
    a_help = menuBar()->addAction(tr("Help"));
    a_opt = menuBar()->addAction(tr("Options"));
    /*this->menuBar()->addAction(clear);
    this->menuBar()->addAction(load);
    this->menuBar()->addAction(save);
    this->menuBar()->addAction(opt);
    this->menuBar()->addAction(copy);
    this->menuBar()->addAction(help);
    this->menuBar()->addAction(about);*/
    connect(a_clear, SIGNAL(triggered()), this, SLOT(clearAll()));
    connect(a_about, SIGNAL(triggered()), this, SLOT(about()));
    connect(a_save, SIGNAL(triggered()), this, SLOT(save_ini()));
    connect(a_load, SIGNAL(triggered()), this, SLOT(load_ini()));
    connect(a_help, SIGNAL(triggered()), this, SLOT(help()));
    connect(a_opt, SIGNAL(triggered()), this, SLOT(optDialog()));
    connect(a_copy, SIGNAL(triggered()), this, SLOT(showcopyDialog()));
#if defined(Q_WS_MAEMO_5)
    menuBar()->hide();
#endif
    /********automatic load settings**************************/
    QFileInfo fi(QDir::homePath() + "/.config/hextool/settings.ini");
    if (fi.exists())
    {
        QSettings settings(QDir::homePath() + "/.config/hextool/settings.ini", QSettings::IniFormat);
        settings.beginGroup("Options");
        optsl=settings.value("optsl").toBool();
        settings.endGroup();
        if (optsl)
        {
            load_ini();
        }
    }
    /**********************************************************/
}

HexTool::~HexTool()
{
    delete ui;
}

void HexTool::showExpanded()
{
#if defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

/****************************************************************************

    Qt Event Handler

    control:    hextool widget
    signal:     closed, exit

    It is used for automatic save during exit.

****************************************************************************/
void HexTool::closeEvent(QCloseEvent *event)
{
    if (optsl)
    {
        save_ini();
    }
    else
    {
        QFileInfo fi(QDir::homePath() + "/.config/hextool/settings.ini");
        if (fi.exists())
        {
            QSettings settings(QDir::homePath() + "/.config/hextool/settings.ini", QSettings::IniFormat);
            settings.beginGroup("Options");
            settings.setValue("optsl", optsl);
            settings.endGroup();
        }
    }
    event->accept();
}

/****************************************************************************

    Update Display function

    Refreshes the gui.
//QString::number(value,8) //octal
//int hex = str.toInt(&ok, 16);
//int dec = str.toInt(&ok, 10);
****************************************************************************/
void HexTool::UpdateDisplay()
{
    static bool updatingDisplay = false;

    if (!updatingDisplay)
    {
        updatingDisplay = true;

        int value = GetValue(REG_CURRENT);
#ifdef QT_DEBUG
        qDebug() << "value from current reg [raw]:" << QString::number(value);
#endif

        char str[128];
        if (GetSigned()==UNSIGNED)
        {
            sprintf(str, "%u", value);

#ifdef QT_DEBUG
            qDebug() << "dec char128 [unsigned]:" << str;
#endif
        }
        else
        {
            switch (GetNumBits())
            {
            case NUMBITS_8:
                sprintf(str, "%d", (qint8) value);
                break;
            case NUMBITS_16:
                sprintf(str, "%d", (qint16) value);
                break;
            case NUMBITS_32:
                sprintf(str, "%d", (qint32) value);
                break;
            }
#ifdef QT_DEBUG
            qDebug() << "dec char128 [signed]:" << str;
#endif
        }
        ui->lineEdit_dec->setText(str);
        sprintf(str, "%X", value);
        ui->lineEdit_hex->setText(str);
        sprintf(str, "%o", value);
        ui->lineEdit_oct->setText(str);

        sprintf(str, "%d", (quint8)((value & 0x000000FF) >> 0));
        ui->lineEdit_ipaddress0->setText(str);
        sprintf(str, "%d", (quint8)((value & 0x0000FF00) >> 8));
        ui->lineEdit_ipaddress1->setText(str);
        sprintf(str, "%d", (quint8)((value & 0x00FF0000) >> 16));
        ui->lineEdit_ipaddress2->setText(str);
        sprintf(str, "%d", (quint8)((value & 0xFF000000) >> 24));
        ui->lineEdit_ipaddress3->setText(str);

        sprintf(str, "%c", (quint8)((value & 0x000000FF) >> 0));
        ui->lineEdit_ascii0->setText(str);
        sprintf(str, "%c", (quint8)((value & 0x0000FF00) >> 8));
        ui->lineEdit_ascii1->setText(str);
        sprintf(str, "%c", (quint8)((value & 0x00FF0000) >> 16));
        ui->lineEdit_ascii2->setText(str);
        sprintf(str, "%c", (quint8)((value & 0xFF000000) >> 24));
        ui->lineEdit_ascii3->setText(str);

        ////////////////////////////////////////////////////////////////////////
        int bitVal;

        for (int bit=0; bit<8; bit++)
        {
            bitVal = (value & (1<<bit))?1:0;
            sprintf(str, "%d", bitVal);
            ui->buttonGroup->button(bit+1)->setText(str);
        }

        if (GetNumBits() != 8)
        {
            ui->frame_bit16->setDisabled(false);
            ui->lineEdit_ascii1 ->setDisabled(false);
            ui->lineEdit_ipaddress1 ->setDisabled(false);

            for (int bit=8; bit<16; bit++)
            {
                bitVal = (value & (1<<bit))?1:0;
                sprintf(str, "%d", bitVal);
                ui->buttonGroup->button(bit+1)->setText(str);
            }

            if (GetNumBits() != 16)
            {

                ui->frame_bit32->setDisabled(false);
                ui->lineEdit_ascii3->setDisabled(false);
                ui->lineEdit_ascii2->setDisabled(false);
                ui->lineEdit_ipaddress3->setDisabled(false);
                ui->lineEdit_ipaddress2->setDisabled(false);

                for (int bit=16; bit<32; bit++)
                {
                    bitVal = (value & (1<<bit))?1:0;
                    sprintf(str, "%d", bitVal);
                    ui->buttonGroup->button(bit+1)->setText(str);
                }
            }
            else
            {
                for (int i=1; i<=16; i++)
                {
                    ui->buttonGroup->button(i+16)->setText("");
                }
                ui->frame_bit32->setDisabled(true);
                ui->lineEdit_ascii3->setDisabled(true);
                ui->lineEdit_ascii2->setDisabled(true);
                ui->lineEdit_ipaddress3->setDisabled(true);
                ui->lineEdit_ipaddress2->setDisabled(true);
            }
        }
        else
        {
            for (int i=1; i<=24; i++)
            {
                ui->buttonGroup->button(i+8)->setText("");
            }
            ui->frame_bit16->setDisabled(true);
            ui->frame_bit32->setDisabled(true);
            ui->lineEdit_ascii1->setDisabled(true);
            ui->lineEdit_ipaddress1->setDisabled(true);
            ui->lineEdit_ascii3->setDisabled(true);
            ui->lineEdit_ascii2->setDisabled(true);
            ui->lineEdit_ipaddress3->setDisabled(true);
            ui->lineEdit_ipaddress2->setDisabled(true);
        }
        updatingDisplay = false;
    }
}

/****************************************************************************

    Qt Signal Handler and toggle bit function

    control: pushbuttongroup
    signal: clicked

    Toggles single bits

****************************************************************************/
void HexTool::groupButton_clicked(int bit)
{
    ToggleBit(bit-1);
}

void HexTool::ToggleBit(int bit)
{
    HandleBinaryDigit(bit);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    lineEdit_dec
    signal:     textEdited

    The decimal number edit box has changed

****************************************************************************/
void HexTool::on_lineEdit_dec_textEdited()
{
    int value;
    if (GetSigned()==UNSIGNED)
    {
        value = (int)ui->lineEdit_dec->text().toUInt();
#ifdef QT_DEBUG
        qDebug() << "input unsigned dec:" << QString::number(value);
#endif
    }
    else
    {
        value = (int)ui->lineEdit_dec->text().toInt();
#ifdef QT_DEBUG
        qDebug() << "input signed dec:" << QString::number(value);
#endif
    }
    SetValue(value, REG_CURRENT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    lineEdit_oct
    signal:     textEdited

    The octal number edit box has changed

****************************************************************************/
void HexTool::on_lineEdit_oct_textEdited()
{
    int value;
    value = (int)ui->lineEdit_oct->text().toUInt(0,8);
    SetValue(value, REG_CURRENT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    lineEdit_hex
    signal:     textEdited

    The hexadecimal number edit box has changed

****************************************************************************/
void HexTool::on_lineEdit_hex_textEdited()
{
    int value;
    value = (int)ui->lineEdit_hex->text().toUInt(0,16);
    SetValue(value, REG_CURRENT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_clr
    signal:     clicked

    The clr button was clicked clears current register

****************************************************************************/
void HexTool::on_pushButton_clr_clicked()
{
    int value;
    value=0;
    SetValue(value, REG_CURRENT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    radioButton_8bit
    signal:     pressed

    The radio button for 8 bit mode has been selected (aka pressed)

****************************************************************************/
void HexTool::on_radioButton_8bit_clicked()
{
    SetNumBits(NUMBITS_8);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    radioButton_16bit
    signal:     pressed

    The radio button for 16 bit mode has been selected (aka pressed)

****************************************************************************/
void HexTool::on_radioButton_16bit_clicked()
{
    int bitsize;
    int value = GetValue(REG_CURRENT);
#ifdef QT_DEBUG
    qDebug() << "not changed value(r16):" << QString::number(value);
#endif
    bitsize=GetNumBits();
    if (bitsize==8 && ui->checkBox_signed->checkState() && optsignext)
    {
        qint8 signedvalue=value;
        SetNumBits(NUMBITS_16);
        SetValue(signedvalue, REG_CURRENT);
#ifdef QT_DEBUG
        qDebug() << "changed value(r16):" << QString::number(signedvalue);
#endif
    }
    else
    {
        SetNumBits(NUMBITS_16);
    }
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    radioButton_32bit
    signal:     pressed

    The radio button for 32 bit mode has been selected (aka pressed)

****************************************************************************/
void HexTool::on_radioButton_32bit_clicked()
{
    int bitsize;
    int value=GetValue(REG_CURRENT);;
#ifdef QT_DEBUG
    qDebug() << "not changed value(r32):" << QString::number(value);
#endif
    bitsize=GetNumBits();
    if (bitsize!=32 && ui->checkBox_signed->checkState() && optsignext)
    {
        SetNumBits(NUMBITS_32);
        if (bitsize==8)
        {
            qint8 signedvalue=value;
            SetValue(signedvalue, REG_CURRENT);
#ifdef QT_DEBUG
            qDebug() << "changed value(r32):" << QString::number(signedvalue);
#endif
        }
        else if (bitsize==16)
        {
            qint16 signedvalue=value;
            SetValue(signedvalue, REG_CURRENT);
#ifdef QT_DEBUG
            qDebug() << "changed value:(r32)" << QString::number(signedvalue);
#endif
        }
    }
    else
    {
        SetNumBits(NUMBITS_32);
    }
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_not
    signal:     pressed

    The NOT button has been pressed

****************************************************************************/
void HexTool::on_pushButton_not_pressed()
{
    RegisterOperation(REG_CURRENT, REG_CURRENT, REG_CURRENT, OP_INVERSE);
    UpdateDisplay();
}

/****************************************************************************

   Qt Signal Handler

    control:    checkBox_signed
    signal:     toggled

    The checkbox for signed has been toggled for signed/unsigned operation

****************************************************************************/
void HexTool::on_checkBox_signed_toggled(bool isChecked)
{
    eSigned sig;

    if (isChecked)
        sig=SIGNED;
    else
        sig=UNSIGNED;

    SetSigned(sig);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_shiftRight
    signal:     pressed

    The right shift button has been pressed to shift the number 1 place

****************************************************************************/
void HexTool::on_pushButton_shiftLeft_pressed()
{
    RegisterOperation(REG_CURRENT, REG_CURRENT, REG_CURRENT, OP_LEFT_SHIFT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_shiftRight
    signal:     pressed

    The right shift button has been pressed to shift the number 1 place

****************************************************************************/
void HexTool::on_pushButton_shiftRight_pressed()
{
    RegisterOperation(REG_CURRENT, REG_CURRENT, REG_CURRENT, OP_RIGHT_SHIFT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_bitNumbering
    signal:     pressed

    Toggle the labelling of the bits from zero based to 1 based

****************************************************************************/
void HexTool::on_pushButton_bitNumbering_pressed()
{
    static bool zeroBased = false;

    if (zeroBased)
    {
        zeroBased = false;
        ui->pushButton_bitNumbering->setText("1");
        ui->label_bit4->setText("4");
        ui->label_bit8->setText("8");
        ui->label_bit12->setText("12");
        ui->label_bit16->setText("16");
        ui->label_bit20->setText("20");
        ui->label_bit24->setText("24");
        ui->label_bit28->setText("28");
        ui->label_bit32->setText("32");
    }
    else
    {
        zeroBased = true;
        ui->pushButton_bitNumbering->setText("0");
        ui->label_bit4->setText("3");
        ui->label_bit8->setText("7");
        ui->label_bit12->setText("11");
        ui->label_bit16->setText("15");
        ui->label_bit20->setText("21");
        ui->label_bit24->setText("23");
        ui->label_bit28->setText("27");
        ui->label_bit32->setText("31");
    }
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_endianSwap
    signal:     pressed

    Toggles the endian between Big and Little endian

****************************************************************************/
void HexTool::on_pushButton_endianSwap_pressed()
{
    unsigned int value = GetValue(REG_CURRENT);
    unsigned int newValue = 0;

    switch (GetNumBits())
    {
    case 32:
    {
        newValue |= (value & 0x000000ff) << 24;
        newValue |= (value & 0x0000ff00) <<  8;
        newValue |= (value & 0x00ff0000) >>  8;
        newValue |= (value & 0xff000000) >> 24;
    }
    break;

    case 16:
    {
        newValue |= (value & 0x000000ff) << 8;
        newValue |= (value & 0x0000ff00) >> 8;
    }
    break;

    default:
    case 8:
        // don't do anything!
        newValue = value;
        break;
    }
    SetValue(newValue, REG_CURRENT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton increase and decrease
    signal:     clicked

    increases decreases value by one

****************************************************************************/
void HexTool::on_pushButton_increase_clicked()
{
    int value = GetValue(REG_CURRENT);
    value++;
    SetValue(value, REG_CURRENT);
    UpdateDisplay();
}

void HexTool::on_pushButton_decrease_clicked()
{
    int value = GetValue(REG_CURRENT);
    value--;
    SetValue(value, REG_CURRENT);
    UpdateDisplay();
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_neg
    signal:     clicked

    Makes the decimal value negativ or positive in signed mode

****************************************************************************/
void HexTool::on_pushButton_neg_clicked()
{
    if (ui->checkBox_signed->isChecked())
    {
        int value = GetValue(REG_CURRENT);
        value=value*-1;
        SetValue(value, REG_CURRENT);
        UpdateDisplay();
    }
}

/****************************************************************************

    Qt Signal Handler

    control:    radioButton_register[A|B|C]
    signal:     clicked

    Select the current register that is being displayed

****************************************************************************/
void HexTool::on_radioButton_registerA_clicked()
{
    SetRegister(REG_A);
    UpdateDisplay();
}

void HexTool::on_radioButton_registerB_clicked()
{
    SetRegister(REG_B);
    UpdateDisplay();
}

void HexTool::on_radioButton_registerC_clicked()
{
    SetRegister(REG_C);
    UpdateDisplay();
}

/****************************************************************************

    This is the generic function to handle register operations after the user
    has clicked on one of the buttons.

****************************************************************************/
int HexTool::HandleOperation(eOpType op)
{
    // Perform the operation on the registers
    int retVal = RegisterOperation(REG_A, REG_B, REG_C, op);

    // change the display to show register C
    SetRegister(REG_C);
    ui->radioButton_registerC->setChecked(true);

    // Update the display
    UpdateDisplay();

    return (retVal);
}

/****************************************************************************

    Qt Signal Handler

    control:    pushButton_XXX
    signal:     pressed

    Where XXX is a register operation like AND, OR, XOR, ...
    These handlers just pass the processing to the generic HandleOperation
    function.

****************************************************************************/
void HexTool::on_pushButton_AND_pressed()
{
    HandleOperation(OP_AND);
}

void HexTool::on_pushButton_OR_pressed()
{
    HandleOperation(OP_OR);
}

void HexTool::on_pushButton_XOR_pressed()
{
    HandleOperation(OP_XOR);
}

void HexTool::on_pushButton_plus_pressed()
{
    HandleOperation(OP_ADD);
}

void HexTool::on_pushButton_minus_pressed()
{
    HandleOperation(OP_SUBTRACT);
}

void HexTool::on_pushButton_multiply_pressed()
{
    HandleOperation(OP_MULTIPLY);
}

void HexTool::on_pushButton_divide_pressed()
{
    HandleOperation(OP_DIVIDE);
}

/****************************************************************************

    This generic handler deals with the user editing the IP Address edit
    boxes.
    It will automatically move the focus to the next control if the user
    enters a 3 digit number, because each part of an IP address is always
    in the range 0...255.
    Users are also forbidden from entering numbers greater than 255.

****************************************************************************/
void HexTool::HandleIpAddrEditBox(int field, unsigned int val)
{
    if (field < 4)
    {
        if ((val & 0xFFFFFF00) == 0)
        {
            int currentValue    = GetValue(REG_CURRENT);    // the current value stored in the current register
            int newValue        = 0;                        // the new value that we want to to set the register to

            switch (field)   // which byte was modified (i.e. which ASCI edit box)
            {
            case 0:
                newValue = (currentValue & 0xFFFFFF00) | (val      );
                break;
            case 1:
                newValue = (currentValue & 0xFFFF00FF) | (val <<  8);
                break;
            case 2:
                newValue = (currentValue & 0xFF00FFFF) | (val << 16);
                break;
            case 3:
                newValue = (currentValue & 0x00FFFFFF) | (val << 24);
                break;
            }

            SetValue(newValue, REG_CURRENT);
            UpdateDisplay();

            // If the value has changed, skip to the next character
            if ((newValue != currentValue) && (val > 99))
            {
                QLineEdit * nextCtrl=nullptr;
                switch (field)
                {
                case 0:
                    switch (GetNumBits())
                    {
                    case NUMBITS_8:
                        nextCtrl = ui->lineEdit_ipaddress0;
                        break;
                    case NUMBITS_16:
                        nextCtrl = ui->lineEdit_ipaddress1;
                        break;
                    case NUMBITS_32:
                        nextCtrl = ui->lineEdit_ipaddress3;
                        break;
                    }
                    break;
                case 1:
                    nextCtrl = ui->lineEdit_ipaddress0;
                    break;
                case 2:
                    nextCtrl = ui->lineEdit_ipaddress1;
                    break;
                case 3:
                    nextCtrl = ui->lineEdit_ipaddress2;
                    break;
                }
                nextCtrl->setFocus();
                nextCtrl->setSelection(0,10);
            }
        }
        else
        {
            // refuse to accept more than 255 in an IP Addr edit box
            UpdateDisplay();
        }
    }
}

/****************************************************************************

    Qt Signal Handler

    control:    lineEdit_ipaddressX
    signal:     textEdited

    When the users edits the ip address fields, these handlers pass control
    to the generic HandleIpAddrEditBox function.

****************************************************************************/
void HexTool::on_lineEdit_ipaddress0_textEdited()
{
    HandleIpAddrEditBox(0, ui->lineEdit_ipaddress0->text().toUInt());
}

void HexTool::on_lineEdit_ipaddress1_textEdited()
{
    HandleIpAddrEditBox(1, ui->lineEdit_ipaddress1->text().toUInt());
}

void HexTool::on_lineEdit_ipaddress2_textEdited()
{
    HandleIpAddrEditBox(2, ui->lineEdit_ipaddress2->text().toUInt());
}

void HexTool::on_lineEdit_ipaddress3_textEdited()
{
    HandleIpAddrEditBox(3, ui->lineEdit_ipaddress3->text().toUInt());
}

/****************************************************************************

    The generic handler called when the users types into the ASCII edit boxes.
    The UI is programmed to only allow 1 character using the form designer.
    This function automatically moves focus to the next ASCII edit control,
    wrapping back to the first.

****************************************************************************/
void HexTool::HandleAsciiEditBox(int field, unsigned int val)
{
    if (field < 4)
    {
        int newByte         = val & 0x000000FF;// the byte that was just typed in the ASCII edit box
        int currentValue    = GetValue(REG_CURRENT);    // the current value stored in the current register
        int newValue        = 0;                        // the new value that we want to to set the register to

        switch (field)   // which byte was modified (i.e. which ASCI edit box)
        {
        case 0:
            newValue = (currentValue & 0xFFFFFF00) | (newByte      );
            break;
        case 1:
            newValue = (currentValue & 0xFFFF00FF) | (newByte <<  8);
            break;
        case 2:
            newValue = (currentValue & 0xFF00FFFF) | (newByte << 16);
            break;
        case 3:
            newValue = (currentValue & 0x00FFFFFF) | (newByte << 24);
            break;
        }

        SetValue(newValue, REG_CURRENT);
        UpdateDisplay();

        // If the value has changed, skip to the next character
        if (newValue != currentValue)
        {
            QLineEdit * nextCtrl=nullptr;
            switch (field)
            {
            case 0:
                switch (GetNumBits())
                {
                case NUMBITS_8:
                    nextCtrl = ui->lineEdit_ascii0;
                    break;
                case NUMBITS_16:
                    nextCtrl = ui->lineEdit_ascii1;
                    break;
                case NUMBITS_32:
                    nextCtrl = ui->lineEdit_ascii3;
                    break;
                }
                break;
            case 1:
                nextCtrl = ui->lineEdit_ascii0;
                break;
            case 2:
                nextCtrl = ui->lineEdit_ascii1;
                break;
            case 3:
                nextCtrl = ui->lineEdit_ascii2;
                break;
            }
            nextCtrl->setFocus();
            nextCtrl->setSelection(0,1);
        }
    }
}

/****************************************************************************

    Qt Signal Handler

    control:    lineEdit_asciiX
    signal:     textEdited

    Passes control on to a generic handler after extracting the new ASCII
    value that was entered.

****************************************************************************/
void HexTool::on_lineEdit_ascii0_textEdited()
{
    QString text;
    text = ui->lineEdit_ascii0->text();
    int val = (int)text[0].unicode();
    HandleAsciiEditBox(0, val);
}
void HexTool::on_lineEdit_ascii1_textEdited()
{
    QString text;
    text = ui->lineEdit_ascii1->text();
    int val = (int)text[0].unicode();
    HandleAsciiEditBox(1, val);
}
void HexTool::on_lineEdit_ascii2_textEdited()
{
    QString text;
    text = ui->lineEdit_ascii2->text();
    int val = (int)text[0].unicode();
    HandleAsciiEditBox(2, val);
}
void HexTool::on_lineEdit_ascii3_textEdited()
{
    QString text;
    text = ui->lineEdit_ascii3->text();
    int val = (int)text[0].unicode();
    HandleAsciiEditBox(3, val);
}

/****************************************************************************

    clearAll function clears values from all register

****************************************************************************/
void HexTool::clearAll()
{
    int value;
    value=0;
    SetValue(value, REG_A);
    SetValue(value, REG_B);
    SetValue(value, REG_C);
    UpdateDisplay();
}

/****************************************************************************

    About MessageBox

****************************************************************************/
void HexTool::about()
{
    QPixmap logo(":/new/ownresource/hextool.png");
    //QMessageBox::about(this,tr("HexTool Maemo Version 1.7.1+leste1"),tr("Ported and code changes by Halftux.\nBased on Version 1.7.0.1 by Ryan Harkin."));
    QMessageBox about;

    about.setWindowTitle("HexTool Maemo Version 1.7.2+leste1");
    about.setText("Ported and code changes by Halftux.\nBased on Version 1.7.0.1 by Ryan Harkin.");
    //about.setInformativeText();
    about.setStandardButtons(QMessageBox::Ok);
    about.setIconPixmap(logo);
    about.setDefaultButton(QMessageBox::Ok);
    about.show();
    about.exec();

    //QMessageBox::about.setIconPixmap(QPixMap(":/new/onwresource/hextool48.png"));
}

/****************************************************************************

    Help MessageBox

****************************************************************************/
void HexTool::help()
{
    QMessageBox::information(this,tr("Help"),
                             "You can put different numbers in register <b>A</b> and <b>B</b> and make logical and mathematical operations. The result is shown in register <b>C</b>.<br>The <i>arrow</i> buttons: shift operations (current register).<br>The <i>flip</i> button: endian swap (hexadecimal value).<br>The <i>not</i> button: invert (binary).<br>The small <i>+</i> and <i>-</i> button: increases/decreases (dezimal value).<br>The <i>one/zero</i> button: change the label 0/1 (binary).<br>The <i>+-</i> button: inverse (dezimal value [signed])");
}

/****************************************************************************

    Save function

    Saves status of radiobuttons, checkbox, all register values and
    the options in an ini file

****************************************************************************/
void HexTool::save_ini()
{
    QSettings settings(QDir::homePath() + "/.config/hextool/settings.ini", QSettings::IniFormat);
    settings.beginGroup("General");
    settings.setValue("signed", ui->checkBox_signed->isChecked());
    settings.setValue("bitsize", ui->rbuttonGroup_bitsize->checkedId());
    settings.setValue("lastreg", ui->rbuttonGroup_reg->checkedId());
    settings.setValue("valuerega", GetValue(REG_A));
    settings.setValue("valueregb", GetValue(REG_B));
    settings.setValue("valueregc", GetValue(REG_C));
    settings.endGroup();
    settings.beginGroup("Options");
    settings.setValue("optsl", optsl);
    settings.setValue("optsignext", optsignext);
    settings.endGroup();


#if defined(Q_WS_MAEMO_5)
    QMaemo5InformationBox::information(this, "settings saved", QMaemo5InformationBox::DefaultTimeout);
#endif
}

/****************************************************************************

    Load function

    Loads status of radiobuttons, checkbox, all register values and
    the options from an ini file

****************************************************************************/
void HexTool::load_ini()
{
    QFileInfo fi(QDir::homePath() + "/.config/hextool/settings.ini");
    if (!fi.exists())
    {
#if defined(Q_WS_MAEMO_5)
        QMaemo5InformationBox::information(this, "no file found", QMaemo5InformationBox::DefaultTimeout);
#else
        QMessageBox::warning(this, "File not found","The settings file was not found.",QMessageBox::Ok,0);
#endif

    }
    else
    {
        int bitsize;
        int reg_id;
        int valueA;
        int valueB;
        int valueC;
        QSettings settings(QDir::homePath() + "/.config/hextool/settings.ini", QSettings::IniFormat);
        settings.beginGroup("General");
        ui->checkBox_signed->setChecked(settings.value("signed").toBool());
        bitsize=settings.value("bitsize").toInt();
        reg_id=settings.value("lastreg").toInt();
        valueA=settings.value("valuerega").toInt();
        valueB=settings.value("valueregb").toInt();
        valueC=settings.value("valueregc").toInt();
        settings.endGroup();
        settings.beginGroup("Options");
        optsl=settings.value("optsl").toBool();
        optsignext=settings.value("optsignext").toBool();
        settings.endGroup();

        SetValue(valueA, REG_A);
        SetValue(valueB, REG_B);
        SetValue(valueC, REG_C);

        switch (bitsize)
        {
        case 32:
            ui->radioButton_32bit->setChecked(true);
            on_radioButton_32bit_clicked();
            break;
        case 16:
            ui->radioButton_16bit->setChecked(true);
            on_radioButton_16bit_clicked();
            break;
        case 8:
            ui->radioButton_8bit->setChecked(true);
            on_radioButton_8bit_clicked();
            break;
        default:
            qDebug() << "switch:" << QString::number(bitsize);
        }
        switch (reg_id)
        {
        case 1:
            ui->radioButton_registerA->setChecked(true);
            SetRegister(REG_A);
            break;
        case 2:
            ui->radioButton_registerB->setChecked(true);
            SetRegister(REG_B);
            break;
        case 3:
            ui->radioButton_registerC->setChecked(true);
            SetRegister(REG_C);
            break;
        default:
            qDebug() << "switch:" << QString::number(reg_id);
        }

        UpdateDisplay();

#if defined(Q_WS_MAEMO_5)
        QMaemo5InformationBox::information(this, "settings loaded", QMaemo5InformationBox::DefaultTimeout);
#endif
    }
}

/****************************************************************************

    QDialog: Options

    Shows the option checkboxes and returns bool statements

****************************************************************************/
void HexTool::optDialog()
{
    optionsDialog showopt(optsl,optsignext,this);
    if (showopt.exec()==optionsDialog::Accepted)
    {
        optsl=showopt.saveload();
        optsignext=showopt.signext();
    }
}

/****************************************************************************

    QDialog: Copy functions

    Shows the copy function and returns a QString

****************************************************************************/
void HexTool::showcopyDialog()
{
    QString psip, pstext;
    psip=ui->lineEdit_ipaddress0->text()+"."+ui->lineEdit_ipaddress1->text()+"."+ui->lineEdit_ipaddress2->text()+"."+ui->lineEdit_ipaddress3->text();
    pstext=ui->lineEdit_ascii0->text() + ui->lineEdit_ascii1->text() + ui->lineEdit_ascii2->text() + ui->lineEdit_ascii3->text();

    copyDialog showcopy(ui->lineEdit_hex->text(), ui->lineEdit_oct->text(), ui->lineEdit_dec->text(), psip, pstext, this);
    if (showcopy.exec()==copyDialog::Accepted)
    {
        this->setclipboard(showcopy.getaction());
    }
}

/****************************************************************************

    Set clipboard function

    Copies to clipboard or values to other registers

****************************************************************************/
void HexTool::setclipboard(QString clip)
{
    if (clip.startsWith("cp") || clip.startsWith("sp"))
    {
        if (clip.startsWith("cpab"))
        {
            int value = GetValue(REG_A);
            SetValue(value, REG_B);
            ui->radioButton_registerB->setChecked(true);
#if defined(Q_WS_MAEMO_5)
            QMaemo5InformationBox::information(this, "Copy: [A->B] Register B selected",QMaemo5InformationBox::DefaultTimeout);
#endif
        }
        else if (clip.startsWith("cpba"))
        {
            int value = GetValue(REG_B);
            SetValue(value, REG_A);
            ui->radioButton_registerA->setChecked(true);
#if defined(Q_WS_MAEMO_5)
            QMaemo5InformationBox::information(this, "Copy: [B->A] Register A selected",QMaemo5InformationBox::DefaultTimeout);
#endif
        }
        else if (clip.startsWith("cpca"))
        {
            int value = GetValue(REG_C);
            SetValue(value, REG_A);
            ui->radioButton_registerA->setChecked(true);
#if defined(Q_WS_MAEMO_5)
            QMaemo5InformationBox::information(this, "Copy: [C->A] Register A selected",QMaemo5InformationBox::DefaultTimeout);
#endif
        }
        else if (clip.startsWith("cpcb"))
        {
            int value = GetValue(REG_C);
            SetValue(value, REG_B);
            ui->radioButton_registerB->setChecked(true);
#if defined(Q_WS_MAEMO_5)
            QMaemo5InformationBox::information(this, "Copy: [C->B] Register B selected",QMaemo5InformationBox::DefaultTimeout);
#endif
        }
        else if (clip.startsWith("spab"))
        {
            int value1 = GetValue(REG_A);
            int value2 = GetValue(REG_B);
            SetValue(value1, REG_B);
            SetValue(value2, REG_A);
#if defined(Q_WS_MAEMO_5)
            QMaemo5InformationBox::information(this, "Swap: [A<->B]",QMaemo5InformationBox::DefaultTimeout);
#endif
        }
        UpdateDisplay();
    }
    else
    {
        QClipboard *clipboard = QApplication::clipboard();
        clip=clip.right(clip.length()-clip.indexOf(":")-2);
#ifdef QT_DEBUG
        qDebug()<< clip;
#endif

        clipboard->setText(clip);
#if defined(Q_WS_MAEMO_5)
        QMaemo5InformationBox::information(this, "Clipboard: "+clip,QMaemo5InformationBox::DefaultTimeout);
#endif

#ifdef QT_DEBUG
        //qDebug()<< "clipboard: " << clipboard->text();
#endif
    }
}
