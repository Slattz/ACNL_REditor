#include "Headers/buttonremap.h"
#include "ui_buttonremap.h"

static constexpr ACNL_Key ComboBoxToBTN[17] = {ACNL_Key::A, ACNL_Key::B, ACNL_Key::X, ACNL_Key::Y, ACNL_Key::Select, ACNL_Key::L, ACNL_Key::R, ACNL_Key::Touchpad, ACNL_Key::DPadUp, ACNL_Key::DPadDown, ACNL_Key::DPadLeft, ACNL_Key::DPadRight, ACNL_Key::CPadUp, ACNL_Key::CPadDown, ACNL_Key::CPadLeft, ACNL_Key::CPadRight, ACNL_Key::None};
static constexpr quint32 ComboBoxToPatch[17] = {
0xE3A00001, 0xE3A00002, 0xE3A00008, 0xE3A00010, 0xE3A00B02, 0xE3A00A02, 0xE3A00901, 0xE3A00902,
0xE3A00801, 0xE3A00802, 0xE3A00701, 0xE3A00702, 0xE3A00601, 0xE3A00602, 0xE3A00501, 0xE3A00502, 0xE3A00000
};

static constexpr Buttons_s DefaultButtons = {ACNL_Key::B, ACNL_Key::L, ACNL_Key::R, ACNL_Key::DPadDown, ACNL_Key::DPadLeft, ACNL_Key::DPadRight, ACNL_Key::DPadUp, ACNL_Key::B, ACNL_Key::A, ACNL_Key::L, ACNL_Key::R, ACNL_Key::Select};
static Buttons_s g_RemapButtons = {ACNL_Key::B, ACNL_Key::L, ACNL_Key::R, ACNL_Key::DPadDown, ACNL_Key::DPadLeft, ACNL_Key::DPadRight, ACNL_Key::DPadUp, ACNL_Key::B, ACNL_Key::A, ACNL_Key::L, ACNL_Key::R, ACNL_Key::Select};

ButtonRemap::ButtonRemap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ButtonRemap)
{
    ui->setupUi(this);
    this->SetupAll();
}

ButtonRemap::~ButtonRemap()
{
    delete ui;
}

bool ButtonRemap::Apply(File *codebin) {
    bool res = false;

    res |= Button_RunB.Apply(codebin);
    res |= Button_RunL.Apply(codebin);
    res |= Button_RunR.Apply(codebin);
    res |= Button_Unequip.Apply(codebin);
    res |= Button_ItemSwitchDL.Apply(codebin);
    res |= Button_ItemSwitchDR.Apply(codebin);
    res |= Button_LookUp.Apply(codebin);
    res |= Button_Pickup.Apply(codebin);
    res |= Button_Interact.Apply(codebin);
    res |= Button_ScreenshotL.Apply(codebin);
    res |= Button_ScreenshotR.Apply(codebin);

    //Update Branch Value For SaveMenu Button Check
    Button_SaveMenu.m_Values[2].Value = MAKE_BRANCH_LINK((Button_SaveMenu.m_Offset + Button_SaveMenu.m_Values[2].OffsetFromBaseAddr), hidKeysDown.m_Offset);
    res |= Button_SaveMenu.Apply(codebin);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

void ButtonRemap::Save(const QComboBox* cmb, PatchValues& Val, ACNL_Key& Key) {
    int index = cmb->currentIndex();
    if (index >= 17 || index < 0) index = 0;
    Val.Value = ComboBoxToPatch[index];
    Key = ComboBoxToBTN[index];
}

void ButtonRemap::Setup(QComboBox* cmb, const ACNL_Key Key) {
    for (int i = 0; i < 17; i++) {
        if (ComboBoxToBTN[i] == Key) {
            cmb->setCurrentIndex(i);
            return;
        }
    }

    cmb->setCurrentIndex(0);
}

void ButtonRemap::SetupAll() {
    Setup(ui->CMB_RunB, g_RemapButtons.RunB);
    Setup(ui->CMB_RunL, g_RemapButtons.RunL);
    Setup(ui->CMB_RunR, g_RemapButtons.RunR);
    Setup(ui->CMB_Unequip, g_RemapButtons.Unequip);
    Setup(ui->CMB_SwitchLeft, g_RemapButtons.ItemSwitchDL);
    Setup(ui->CMB_SwitchRight, g_RemapButtons.ItemSwitchDR);
    Setup(ui->CMB_LookUp, g_RemapButtons.LookUp);
    Setup(ui->CMB_Pickup, g_RemapButtons.Pickup);
    Setup(ui->CMB_Interact, g_RemapButtons.Interact);
    Setup(ui->CMB_ScreenshotL, g_RemapButtons.ScreenshotL);
    Setup(ui->CMB_ScreenshotR, g_RemapButtons.ScreenshotR);
    Setup(ui->CMB_SaveMenu, g_RemapButtons.SaveMenu);
}

void ButtonRemap::SaveAll() {
    Save(ui->CMB_RunB, Button_RunB.m_Values[0], g_RemapButtons.RunB);
    Save(ui->CMB_RunL, Button_RunL.m_Values[0], g_RemapButtons.RunL);
    Save(ui->CMB_RunR, Button_RunR.m_Values[0], g_RemapButtons.RunR);
    Save(ui->CMB_Unequip, Button_Unequip.m_Values[0], g_RemapButtons.Unequip);
    Save(ui->CMB_SwitchLeft, Button_ItemSwitchDL.m_Values[0], g_RemapButtons.ItemSwitchDL);
    Save(ui->CMB_SwitchRight, Button_ItemSwitchDR.m_Values[0], g_RemapButtons.ItemSwitchDR);
    Save(ui->CMB_LookUp, Button_LookUp.m_Values[0], g_RemapButtons.LookUp);
    Save(ui->CMB_Pickup, Button_Pickup.m_Values[0], g_RemapButtons.Pickup);
    Save(ui->CMB_Interact, Button_Interact.m_Values[0], g_RemapButtons.Interact);
    Save(ui->CMB_ScreenshotL, Button_ScreenshotL.m_Values[0], g_RemapButtons.ScreenshotL);
    Save(ui->CMB_ScreenshotR, Button_ScreenshotR.m_Values[0], g_RemapButtons.ScreenshotR);
    Save(ui->CMB_SaveMenu, Button_SaveMenu.m_Values[1], g_RemapButtons.SaveMenu);
}

void ButtonRemap::on_buttonBox_clicked(QAbstractButton *button)
{
    if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Close)) {
        qDebug() << "ButtonRemap Closed";
        this->close();
    }

    if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Reset)) {
        qDebug() << "ButtonRemap Reset";
        this->SetupAll();
    }


    else if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        qDebug() << "ButtonRemap Defaults";
        memcpy(&g_RemapButtons, &DefaultButtons, sizeof(Buttons_s));
        this->SetupAll();
    }

    else if (reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Save)) {
        qDebug() << "ButtonRemap Saved";
        this->SaveAll();
        QMessageBox::information(nullptr, "Saved!", "The Button Remap Settings have been saved.");
    }
}
