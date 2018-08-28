#ifndef SHOPHOURSEDITOR_H
#define SHOPHOURSEDITOR_H

#include <QDialog>
#include <QAbstractButton>
#include <QMessageBox>

struct HoursInfo_s {
    quint8 OpenHour;
    quint8 CloseHour;
};

struct ShopsHours_s {
    HoursInfo_s Retail;
    HoursInfo_s Nooklings[5];
    HoursInfo_s Garden;
    HoursInfo_s Ables;
    HoursInfo_s Sham;
    HoursInfo_s Kicks;
    HoursInfo_s Nooks;
    HoursInfo_s Katrina;
    HoursInfo_s Redd;
};

struct AlwaysOpen_s {
    bool Retail;
    bool Nooklings[5];
    bool Garden;
    bool Ables;
    bool Sham;
    bool Kicks;
    bool Nooks;
    bool Katrina;
    bool Redd;
};

extern ShopsHours_s g_NormalTimes;
extern ShopsHours_s g_NightOwlTimes;
extern ShopsHours_s g_EarlyBirdTimes;
extern AlwaysOpen_s g_AlwaysOpen;
namespace Ui {
class ShopHoursEditor;
}

class ShopHoursEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ShopHoursEditor(QWidget *parent = nullptr);
    ~ShopHoursEditor();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_CMB_NooklingsType_currentIndexChanged(int index);
    void on_CMB_NooklingsType_N_currentIndexChanged(int index);
    void on_CMB_NooklingsType_E_currentIndexChanged(int index);
    void on_CMB_NooklingsOpen_currentIndexChanged(int index);
    void on_CMB_NooklingsClose_currentIndexChanged(int index);
    void on_CMB_NooklingsOpen_N_currentIndexChanged(int index);
    void on_CMB_NooklingsClose_N_currentIndexChanged(int index);
    void on_CMB_NooklingsOpen_E_currentIndexChanged(int index);
    void on_CMB_NooklingsClose_E_currentIndexChanged(int index);
    void on_CB_Nooklings_toggled(bool checked);

    void on_CMB_NooklingsType_AO_currentIndexChanged(int index);

private:
    Ui::ShopHoursEditor *ui;
    void SetupNormalTimes();
    void SetupEarlyTimes();
    void SetupNightTimes();
    void SetupAlwaysOpen();

    void SaveNormalTimes();
    void SaveEarlyTimes();
    void SaveNightTimes();
    void SaveAlwaysOpen();

    HoursInfo_s m_NormalNooklings[5];
    HoursInfo_s m_NightNooklings[5];
    HoursInfo_s m_EarlyNooklings[5];
    bool m_AONooklings[5];
};

#endif // SHOPHOURSEDITOR_H
