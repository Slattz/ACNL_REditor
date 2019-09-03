#ifndef ITEMRANDOMISER_H
#define ITEMRANDOMISER_H

#include <QDialog>
#include <QRandomGenerator>
#include <QAbstractButton>
#include <QMessageBox>

struct ItemPrices_s {
    quint16 ItemPrice;
    quint8  Multiplier;
};

namespace Ui {
class ItemRandomiser;
}

class ItemRandomiser : public QDialog
{
    Q_OBJECT

public:
    explicit ItemRandomiser(QWidget *parent = nullptr);
    ~ItemRandomiser();
    void Save();
    QVector<ItemPrices_s *> GetPrices(void);

private slots:
    void on_LE_MaxPrice_textChanged(const QString &maxtxt);
    void on_LE_MinPrice_textChanged(const QString &mintxt);

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ItemRandomiser *ui;
    ItemPrices_s *SplitItemPrice(quint32 Rand);
    bool FixRange(int min, int max);
    void SetCanAccept(bool CanAccept);

    QVector<ItemPrices_s*> m_Prices;
    bool m_CanSave = false;
};

#endif // ITEMRANDOMISER_H
