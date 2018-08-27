#include "shophourseditor.h"
#include "ui_shophourseditor.h"
#include <QDebug>
#include <QTime>
#include <QPushButton>
#include <QWidget>

ShopsHours_s g_NormalTimes    = {{9,23},{{8,22},{7,0},{9,20},{10,23},{9,21}}, {9,20}, {10,21}, {10,20}, {10,20}, {10,20}, {9,19}, {10,0}};
ShopsHours_s g_NightOwlTimes  = {{9,2},{{8,0},{7,5},{9,0},{10,3},{9,0}}, {9,23}, {10,0}, {10,23}, {10,23}, {10,0}, {9,23}, {10,4}};
ShopsHours_s g_EarlyBirdTimes = {{6,23},{{6,22},{6,0},{6,20},{6,23},{6,21}}, {7,20}, {7,21}, {7,20}, {7,20}, {7,20}, {6,19}, {7,0}};
AlwaysOpen_s g_AlwaysOpen     = {false, {false, false, false, false, false}, false, false, false, false, false, false, false};

ShopHoursEditor::ShopHoursEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShopHoursEditor)
{
    ui->setupUi(this);
    SetupNormalTimes();
    SetupEarlyTimes();
    SetupNightTimes();
    SetupAlwaysOpen();
}

ShopHoursEditor::~ShopHoursEditor() {
    delete ui;
}

void ShopHoursEditor::SetupNormalTimes() {
    ui->CMB_RetailOpen->setCurrentIndex(g_NormalTimes.Retail.OpenHour);
    ui->CMB_RetailClose->setCurrentIndex(g_NormalTimes.Retail.CloseHour);
    /* Nooklings */
    ui->CMB_NooklingsType->setCurrentIndex(0);
    ui->CMB_NooklingsOpen->setCurrentIndex(g_NormalTimes.Nooklings[0].OpenHour);
    ui->CMB_NooklingsClose->setCurrentIndex(g_NormalTimes.Nooklings[0].CloseHour);
    for (int i = 0; i < 5; i++)
        m_NormalNooklings[i] = g_NormalTimes.Nooklings[i];
    /* Garden Centre */
    ui->CMB_GardenOpen->setCurrentIndex(g_NormalTimes.Garden.OpenHour);
    ui->CMB_GardenClose->setCurrentIndex(g_NormalTimes.Garden.CloseHour);
    /* Ables */
    ui->CMB_AblesOpen->setCurrentIndex(g_NormalTimes.Ables.OpenHour);
    ui->CMB_AblesClose->setCurrentIndex(g_NormalTimes.Ables.CloseHour);
    /* Shampoodle */
    ui->CMB_ShamOpen->setCurrentIndex(g_NormalTimes.Sham.OpenHour);
    ui->CMB_ShamClose->setCurrentIndex(g_NormalTimes.Sham.CloseHour);
    /* Kicks */
    ui->CMB_KicksOpen->setCurrentIndex(g_NormalTimes.Kicks.OpenHour);
    ui->CMB_KicksClose->setCurrentIndex(g_NormalTimes.Kicks.CloseHour);
    /* Nooks */
    ui->CMB_NooksOpen->setCurrentIndex(g_NormalTimes.Nooks.OpenHour);
    ui->CMB_NooksClose->setCurrentIndex(g_NormalTimes.Nooks.CloseHour);
    /* Katrina */
    ui->CMB_KatrinaOpen->setCurrentIndex(g_NormalTimes.Katrina.OpenHour);
    ui->CMB_KatrinaClose->setCurrentIndex(g_NormalTimes.Katrina.CloseHour);
    /* Redds */
    ui->CMB_ReddOpen->setCurrentIndex(g_NormalTimes.Redd.OpenHour);
    ui->CMB_ReddClose->setCurrentIndex(g_NormalTimes.Redd.CloseHour);
}

void ShopHoursEditor::SetupEarlyTimes() {
    ui->CMB_RetailOpen_E->setCurrentIndex(g_EarlyBirdTimes.Retail.OpenHour);
    ui->CMB_RetailClose_E->setCurrentIndex(g_EarlyBirdTimes.Retail.CloseHour);
    /* Nooklings */
    ui->CMB_NooklingsType_E->setCurrentIndex(0);
    ui->CMB_NooklingsOpen_E->setCurrentIndex(g_EarlyBirdTimes.Nooklings[0].OpenHour);
    ui->CMB_NooklingsClose_E->setCurrentIndex(g_EarlyBirdTimes.Nooklings[0].CloseHour);
    for (int i = 0; i < 5; i++)
        m_EarlyNooklings[i] = g_EarlyBirdTimes.Nooklings[i];
    /* Garden Centre */
    ui->CMB_GardenOpen_E->setCurrentIndex(g_EarlyBirdTimes.Garden.OpenHour);
    ui->CMB_GardenClose_E->setCurrentIndex(g_EarlyBirdTimes.Garden.CloseHour);
    /* Ables */
    ui->CMB_AblesOpen_E->setCurrentIndex(g_EarlyBirdTimes.Ables.OpenHour);
    ui->CMB_AblesClose_E->setCurrentIndex(g_EarlyBirdTimes.Ables.CloseHour);
    /* Shampoodle */
    ui->CMB_ShamOpen_E->setCurrentIndex(g_EarlyBirdTimes.Sham.OpenHour);
    ui->CMB_ShamClose_E->setCurrentIndex(g_EarlyBirdTimes.Sham.CloseHour);
    /* Kicks */
    ui->CMB_KicksOpen_E->setCurrentIndex(g_EarlyBirdTimes.Kicks.OpenHour);
    ui->CMB_KicksClose_E->setCurrentIndex(g_EarlyBirdTimes.Kicks.CloseHour);
    /* Nooks */
    ui->CMB_NooksOpen_E->setCurrentIndex(g_EarlyBirdTimes.Nooks.OpenHour);
    ui->CMB_NooksClose_E->setCurrentIndex(g_EarlyBirdTimes.Nooks.CloseHour);
    /* Katrina */
    ui->CMB_KatrinaOpen_E->setCurrentIndex(g_EarlyBirdTimes.Katrina.OpenHour);
    ui->CMB_KatrinaClose_E->setCurrentIndex(g_EarlyBirdTimes.Katrina.CloseHour);
    /* Redds */
    ui->CMB_ReddOpen_E->setCurrentIndex(g_EarlyBirdTimes.Redd.OpenHour);
    ui->CMB_ReddClose_E->setCurrentIndex(g_EarlyBirdTimes.Redd.CloseHour);
}

void ShopHoursEditor::SetupNightTimes() {
    ui->CMB_RetailOpen_N->setCurrentIndex(g_NightOwlTimes.Retail.OpenHour);
    ui->CMB_RetailClose_N->setCurrentIndex(g_NightOwlTimes.Retail.CloseHour);
    /* Nooklings */
    ui->CMB_NooklingsType_N->setCurrentIndex(0);
    ui->CMB_NooklingsOpen_N->setCurrentIndex(g_NightOwlTimes.Nooklings[0].OpenHour);
    ui->CMB_NooklingsClose_N->setCurrentIndex(g_NightOwlTimes.Nooklings[0].CloseHour);
    for (int i = 0; i < 5; i++)
        m_NightNooklings[i] = g_NightOwlTimes.Nooklings[i];
    /* Garden Centre */
    ui->CMB_GardenOpen_N->setCurrentIndex(g_NightOwlTimes.Garden.OpenHour);
    ui->CMB_GardenClose_N->setCurrentIndex(g_NightOwlTimes.Garden.CloseHour);
    /* Ables */
    ui->CMB_AblesOpen_N->setCurrentIndex(g_NightOwlTimes.Ables.OpenHour);
    ui->CMB_AblesClose_N->setCurrentIndex(g_NightOwlTimes.Ables.CloseHour);
    /* Shampoodle */
    ui->CMB_ShamOpen_N->setCurrentIndex(g_NightOwlTimes.Sham.OpenHour);
    ui->CMB_ShamClose_N->setCurrentIndex(g_NightOwlTimes.Sham.CloseHour);
    /* Kicks */
    ui->CMB_KicksOpen_N->setCurrentIndex(g_NightOwlTimes.Kicks.OpenHour);
    ui->CMB_KicksClose_N->setCurrentIndex(g_NightOwlTimes.Kicks.CloseHour);
    /* Nooks */
    ui->CMB_NooksOpen_N->setCurrentIndex(g_NightOwlTimes.Nooks.OpenHour);
    ui->CMB_NooksClose_N->setCurrentIndex(g_NightOwlTimes.Nooks.CloseHour);
    /* Katrina */
    ui->CMB_KatrinaOpen_N->setCurrentIndex(g_NightOwlTimes.Katrina.OpenHour);
    ui->CMB_KatrinaClose_N->setCurrentIndex(g_NightOwlTimes.Katrina.CloseHour);
    /* Redds */
    ui->CMB_ReddOpen_N->setCurrentIndex(g_NightOwlTimes.Redd.OpenHour);
    ui->CMB_ReddClose_N->setCurrentIndex(g_NightOwlTimes.Redd.CloseHour);
}

void ShopHoursEditor::SetupAlwaysOpen() {
     ui->CB_Retail->setChecked(g_AlwaysOpen.Retail);
     ui->CMB_NooklingsType_AO->setCurrentIndex(0);
     ui->CB_Nooklings->setChecked(g_AlwaysOpen.Nooklings[0]);
     for (int i = 0; i < 5; i++)
         m_AONooklings[i] = g_AlwaysOpen.Nooklings[i];
    ui->CB_Garden->setChecked(g_AlwaysOpen.Garden);
    ui->CB_Ables->setChecked(g_AlwaysOpen.Ables);
    ui->CB_Sham->setChecked(g_AlwaysOpen.Sham);
    ui->CB_Kicks->setChecked(g_AlwaysOpen.Kicks);
    ui->CB_Nooks->setChecked(g_AlwaysOpen.Nooks);
    ui->CB_Katrina->setChecked(g_AlwaysOpen.Katrina);
    ui->CB_Redd->setChecked(g_AlwaysOpen.Redd);
}

void ShopHoursEditor::SaveNormalTimes() {
    g_NormalTimes.Retail.OpenHour = static_cast<quint8>(ui->CMB_RetailOpen->currentIndex());
    g_NormalTimes.Retail.CloseHour = static_cast<quint8>(ui->CMB_RetailClose->currentIndex());
    /* Nooklings */
    for (int i = 0; i < 5; i++) {
        g_NormalTimes.Nooklings[i].OpenHour = m_NormalNooklings[i].OpenHour;
        g_NormalTimes.Nooklings[i].CloseHour = m_NormalNooklings[i].CloseHour;
    }
    /* Garden Centre */
    g_NormalTimes.Garden.OpenHour = static_cast<quint8>(ui->CMB_GardenOpen->currentIndex());
    g_NormalTimes.Garden.CloseHour = static_cast<quint8>(ui->CMB_GardenClose->currentIndex());
    /* Ables */
    g_NormalTimes.Ables.OpenHour = static_cast<quint8>(ui->CMB_AblesOpen->currentIndex());
    g_NormalTimes.Ables.CloseHour = static_cast<quint8>(ui->CMB_AblesClose->currentIndex());
    /* Shampoodle */
    g_NormalTimes.Sham.OpenHour = static_cast<quint8>(ui->CMB_ShamOpen->currentIndex());
    g_NormalTimes.Sham.CloseHour = static_cast<quint8>(ui->CMB_ShamClose->currentIndex());
    /* Kicks */
    g_NormalTimes.Kicks.OpenHour = static_cast<quint8>(ui->CMB_KicksOpen->currentIndex());
    g_NormalTimes.Kicks.CloseHour = static_cast<quint8>(ui->CMB_KicksClose->currentIndex());
    /* Nooks */
    g_NormalTimes.Nooks.OpenHour = static_cast<quint8>(ui->CMB_NooksOpen->currentIndex());
    g_NormalTimes.Nooks.CloseHour = static_cast<quint8>(ui->CMB_NooksClose->currentIndex());
    /* Katrina */
    g_NormalTimes.Katrina.OpenHour = static_cast<quint8>(ui->CMB_KatrinaOpen->currentIndex());
    g_NormalTimes.Katrina.CloseHour = static_cast<quint8>(ui->CMB_KatrinaClose->currentIndex());
    /* Redds */
    g_NormalTimes.Redd.OpenHour = static_cast<quint8>(ui->CMB_ReddOpen->currentIndex());
    g_NormalTimes.Redd.CloseHour = static_cast<quint8>(ui->CMB_ReddClose->currentIndex());
}

void ShopHoursEditor::SaveEarlyTimes() {
    g_EarlyBirdTimes.Retail.OpenHour = static_cast<quint8>(ui->CMB_RetailOpen_E->currentIndex());
    g_EarlyBirdTimes.Retail.CloseHour = static_cast<quint8>(ui->CMB_RetailClose_E->currentIndex());
    /* Nooklings */
    for (int i = 0; i < 5; i++) {
        g_EarlyBirdTimes.Nooklings[i].OpenHour = m_EarlyNooklings[i].OpenHour;
        g_EarlyBirdTimes.Nooklings[i].CloseHour = m_EarlyNooklings[i].CloseHour;
    }
    /* Garden Centre */
    g_EarlyBirdTimes.Garden.OpenHour = static_cast<quint8>(ui->CMB_GardenOpen_E->currentIndex());
    g_EarlyBirdTimes.Garden.CloseHour = static_cast<quint8>(ui->CMB_GardenClose_E->currentIndex());
    /* Ables */
    g_EarlyBirdTimes.Ables.OpenHour = static_cast<quint8>(ui->CMB_AblesOpen_E->currentIndex());
    g_EarlyBirdTimes.Ables.CloseHour = static_cast<quint8>(ui->CMB_AblesClose_E->currentIndex());
    /* Shampoodle */
    g_EarlyBirdTimes.Sham.OpenHour = static_cast<quint8>(ui->CMB_ShamOpen_E->currentIndex());
    g_EarlyBirdTimes.Sham.CloseHour = static_cast<quint8>(ui->CMB_ShamClose_E->currentIndex());
    /* Kicks */
    g_EarlyBirdTimes.Kicks.OpenHour = static_cast<quint8>(ui->CMB_KicksOpen_E->currentIndex());
    g_EarlyBirdTimes.Kicks.CloseHour = static_cast<quint8>(ui->CMB_KicksClose_E->currentIndex());
    /* Nooks */
    g_EarlyBirdTimes.Nooks.OpenHour = static_cast<quint8>(ui->CMB_NooksOpen_E->currentIndex());
    g_EarlyBirdTimes.Nooks.CloseHour = static_cast<quint8>(ui->CMB_NooksClose_E->currentIndex());
    /* Katrina */
    g_EarlyBirdTimes.Katrina.OpenHour = static_cast<quint8>(ui->CMB_KatrinaOpen_E->currentIndex());
    g_EarlyBirdTimes.Katrina.CloseHour = static_cast<quint8>(ui->CMB_KatrinaClose_E->currentIndex());
    /* Redds */
    g_EarlyBirdTimes.Redd.OpenHour = static_cast<quint8>(ui->CMB_ReddOpen_E->currentIndex());
    g_EarlyBirdTimes.Redd.CloseHour = static_cast<quint8>(ui->CMB_ReddClose_E->currentIndex());
}

void ShopHoursEditor::SaveNightTimes() {
    g_NightOwlTimes.Retail.OpenHour = static_cast<quint8>(ui->CMB_RetailOpen_N->currentIndex());
    g_NightOwlTimes.Retail.CloseHour = static_cast<quint8>(ui->CMB_RetailClose_N->currentIndex());
    /* Nooklings */
    for (int i = 0; i < 5; i++) {
        g_NightOwlTimes.Nooklings[i].OpenHour = m_NightNooklings[i].OpenHour;
        g_NightOwlTimes.Nooklings[i].CloseHour = m_NightNooklings[i].CloseHour;
    }
    /* Garden Centre */
    g_NightOwlTimes.Garden.OpenHour = static_cast<quint8>(ui->CMB_GardenOpen_N->currentIndex());
    g_NightOwlTimes.Garden.CloseHour = static_cast<quint8>(ui->CMB_GardenClose_N->currentIndex());
    /* Ables */
    g_NightOwlTimes.Ables.OpenHour = static_cast<quint8>(ui->CMB_AblesOpen_N->currentIndex());
    g_NightOwlTimes.Ables.CloseHour = static_cast<quint8>(ui->CMB_AblesClose_N->currentIndex());
    /* Shampoodle */
    g_NightOwlTimes.Sham.OpenHour = static_cast<quint8>(ui->CMB_ShamOpen_N->currentIndex());
    g_NightOwlTimes.Sham.CloseHour = static_cast<quint8>(ui->CMB_ShamClose_N->currentIndex());
    /* Kicks */
    g_NightOwlTimes.Kicks.OpenHour = static_cast<quint8>(ui->CMB_KicksOpen_N->currentIndex());
    g_NightOwlTimes.Kicks.CloseHour = static_cast<quint8>(ui->CMB_KicksClose_N->currentIndex());
    /* Nooks */
    g_NightOwlTimes.Nooks.OpenHour = static_cast<quint8>(ui->CMB_NooksOpen_N->currentIndex());
    g_NightOwlTimes.Nooks.CloseHour = static_cast<quint8>(ui->CMB_NooksClose_N->currentIndex());
    /* Katrina */
    g_NightOwlTimes.Katrina.OpenHour = static_cast<quint8>(ui->CMB_KatrinaOpen_N->currentIndex());
    g_NightOwlTimes.Katrina.CloseHour = static_cast<quint8>(ui->CMB_KatrinaClose_N->currentIndex());
    /* Redds */
    g_NightOwlTimes.Redd.OpenHour = static_cast<quint8>(ui->CMB_ReddOpen_N->currentIndex());
    g_NightOwlTimes.Redd.CloseHour = static_cast<quint8>(ui->CMB_ReddClose_N->currentIndex());
}

void ShopHoursEditor::SaveAlwaysOpen() {
    g_AlwaysOpen.Retail = ui->CB_Retail->isChecked();
    for (int i = 0; i < 5; i++)
        g_AlwaysOpen.Nooklings[i] = m_AONooklings[i];

    g_AlwaysOpen.Garden = ui->CB_Garden->isChecked();
    g_AlwaysOpen.Ables = ui->CB_Ables->isChecked();
    g_AlwaysOpen.Sham = ui->CB_Sham->isChecked();
    g_AlwaysOpen.Kicks = ui->CB_Kicks->isChecked();
    g_AlwaysOpen.Nooks = ui->CB_Nooks->isChecked();
    g_AlwaysOpen.Katrina = ui->CB_Katrina->isChecked();
    g_AlwaysOpen.Redd = ui->CB_Redd->isChecked();
}

void ShopHoursEditor::on_buttonBox_clicked(QAbstractButton *button)
{
    if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Close)) {
        qDebug() << "Closed";
        this->close();
    }

    if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Reset)) {
        qDebug() << "Reset";
        SetupNormalTimes();
        SetupEarlyTimes();
        SetupNightTimes();
        SetupAlwaysOpen();
    }

    else if (reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Save)) {
        qDebug() << "Saved";
        SaveNormalTimes();
        SaveEarlyTimes();
        SaveNightTimes();
        SaveAlwaysOpen();
        QMessageBox::information(nullptr, "Saved!", "The Shop Hour Settings have been saved.");
    }
}

void ShopHoursEditor::on_CMB_NooklingsType_currentIndexChanged(int index)
{
    ui->CMB_NooklingsOpen->setCurrentIndex(m_NormalNooklings[index].OpenHour);
    ui->CMB_NooklingsClose->setCurrentIndex(m_NormalNooklings[index].CloseHour);
}

void ShopHoursEditor::on_CMB_NooklingsType_N_currentIndexChanged(int index)
{
    ui->CMB_NooklingsOpen_N->setCurrentIndex(m_NightNooklings[index].OpenHour);
    ui->CMB_NooklingsClose_N->setCurrentIndex(m_NightNooklings[index].CloseHour);
}

void ShopHoursEditor::on_CMB_NooklingsType_E_currentIndexChanged(int index)
{
    ui->CMB_NooklingsOpen_E->setCurrentIndex(m_EarlyNooklings[index].OpenHour);
    ui->CMB_NooklingsClose_E->setCurrentIndex(m_EarlyNooklings[index].CloseHour);
}

void ShopHoursEditor::on_CMB_NooklingsOpen_currentIndexChanged(int index)
{
    if (index < 0 || index > 23)
        return;

    int shop = ui->CMB_NooklingsType->currentIndex();
    m_NormalNooklings[shop].OpenHour = static_cast<quint8>(index);
}

void ShopHoursEditor::on_CMB_NooklingsClose_currentIndexChanged(int index)
{
    if (index < 0 || index > 23)
        return;

    int shop = ui->CMB_NooklingsType->currentIndex();
    m_NormalNooklings[shop].CloseHour = static_cast<quint8>(index);
}

void ShopHoursEditor::on_CMB_NooklingsOpen_N_currentIndexChanged(int index)
{
    if (index < 0 || index > 23)
        return;

    int shop = ui->CMB_NooklingsType_N->currentIndex();
    m_NightNooklings[shop].OpenHour = static_cast<quint8>(index);
}

void ShopHoursEditor::on_CMB_NooklingsClose_N_currentIndexChanged(int index)
{
    if (index < 0 || index > 23)
        return;

    int shop = ui->CMB_NooklingsType_N->currentIndex();
    m_NightNooklings[shop].CloseHour = static_cast<quint8>(index);
}

void ShopHoursEditor::on_CMB_NooklingsOpen_E_currentIndexChanged(int index)
{
    if (index < 0 || index > 23)
        return;

    int shop = ui->CMB_NooklingsType_E->currentIndex();
    m_EarlyNooklings[shop].OpenHour = static_cast<quint8>(index);
}

void ShopHoursEditor::on_CMB_NooklingsClose_E_currentIndexChanged(int index)
{
    if (index < 0 || index > 23)
        return;

    int shop = ui->CMB_NooklingsType_E->currentIndex();
    m_EarlyNooklings[shop].CloseHour = static_cast<quint8>(index);
}

void ShopHoursEditor::on_CB_Nooklings_toggled(bool checked)
{
    int shop = ui->CMB_NooklingsType_AO->currentIndex();
    m_AONooklings[shop] = checked;
}

void ShopHoursEditor::on_CMB_NooklingsType_AO_currentIndexChanged(int index)
{
    ui->CB_Nooklings->setChecked(m_AONooklings[index]);
}
