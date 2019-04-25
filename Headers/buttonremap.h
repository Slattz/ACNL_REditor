#ifndef BUTTONREMAP_H
#define BUTTONREMAP_H

#include <QDialog>

enum ACNL_Key
{
    A           = 0x1,
    B           = 0x2,
    X           = 0x8,
    Y           = 0x10,

    Select      = 0x800,
    Start       = 0x800,

    L           = 0x2000,
    R           = 0x4000,
    Touchpad    = 0x8000,

    DPadUp      = 0x10000,
    DPadDown    = 0x20000,
    DPadLeft    = 0x40000,
    DPadRight   = 0x80000,

    CPadUp      = 0x100000,
    CPadDown    = 0x200000,
    CPadLeft    = 0x400000,
    CPadRight   = 0x800000
};

namespace Ui {
class ButtonRemap;
}

class ButtonRemap : public QDialog
{
    Q_OBJECT

public:
    explicit ButtonRemap(QWidget *parent = nullptr);
    ~ButtonRemap();

private:
    Ui::ButtonRemap *ui;
};

#endif // BUTTONREMAP_H
