#ifndef BUTTONREMAP_H
#define BUTTONREMAP_H

#include <QDialog>
#include "file.h"
#include "patch.h"

enum ACNL_Key : quint32
{
    None        = 0x0, //Custom

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

struct Buttons_s {
    ACNL_Key RunB;
    ACNL_Key RunL;
    ACNL_Key RunR;
    ACNL_Key Unequip;
    ACNL_Key ItemSwitchDL;
    ACNL_Key ItemSwitchDR;
    ACNL_Key LookUp;
    ACNL_Key Pickup;
    ACNL_Key Interact;
    ACNL_Key ScreenshotL;
    ACNL_Key ScreenshotR;
    ACNL_Key SaveMenu;
};

namespace Ui {
class ButtonRemap;
}

class ButtonRemap : public QDialog
{
    Q_OBJECT

public:
    static bool Apply(File *codebin);

    explicit ButtonRemap(QWidget *parent = nullptr);
    ~ButtonRemap();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ButtonRemap *ui;

    void Save(const QComboBox* cmb, PatchValues& Val, ACNL_Key& Key);
    void Setup(QComboBox* cmb, const ACNL_Key Key);
    void SaveAll();
    void SetupAll();
};

#endif // BUTTONREMAP_H
