#ifndef ITEMPRICECHANGER_H
#define ITEMPRICECHANGER_H

#include <QDialog>
#include <QAbstractButton>
#include <QVector>
#include <QTableWidgetItem>

struct ItemPrice_s {
    quint16 ItemPrice;
    qint8  Multiplier;
};

enum FilterType : int {
    ItemName = 0,
    ItemID,
    BuyPrice,
    SellPrice
};

namespace Ui {
class ItemPriceChanger;
}

class ItemPriceChanger : public QDialog
{
    Q_OBJECT

public:
    explicit ItemPriceChanger(QWidget *parent = nullptr);
    ~ItemPriceChanger();
    static const QVector<ItemPrice_s *>& GetPrices(void);
    static void ClearPrices(void);

    enum TableColumns : int {
        ItemID = 0,
        ItemName,
        BuyPrice,
        MedalsBuyPrice,
        RetailSellPrice,
        NooklingSellPrice,
        IslandSellPrice
    };

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_tableWidget_itemChanged(QTableWidgetItem *item);
    void on_CMB_FilterType_currentIndexChanged(int index);
    void on_LE_Filter_textEdited(const QString &arg1);
    void on_CB_CaseSensitive_stateChanged(int arg1);
    void on_BTN_Randomise_clicked();
    void on_LE_RandomMin_textEdited(const QString &arg1);
    void on_LE_RandomMax_textEdited(const QString &arg1);
    void on_LE_RandomMin_editingFinished();
    void on_LE_RandomMax_editingFinished();

private:
    Ui::ItemPriceChanger *ui;
    qint8 GetPriceMultiplier(int price);
    void FixPrice(int& price, qint8& multiplier);
    void FilterVisibleRows(const QString& filter);
    void ResetVisibleRows();
    void ResetPricesToDefault(void);
    void SetupTable(QStringList& ItemNameList);
    void SetupTable(void);
    void SaveTable(void);

    QIntValidator* m_IntValidator = nullptr;
};

#endif // ITEMPRICECHANGER_H
