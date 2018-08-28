#include "Headers/itemrandomiser.h"
#include "ui_itemrandomiser.h"
#include <QDebug>
#include <QTime>
#include <QPushButton>

static int g_max = 1500000;
static int g_min = 1;

ItemRandomiser::ItemRandomiser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemRandomiser)
{
    ui->setupUi(this);
    ui->LE_MaxPrice->setText(QString::number(g_max)); //Set max value to last saved max
    ui->LE_MinPrice->setText(QString::number(g_min)); //Set min value to last saved min
}

ItemRandomiser::~ItemRandomiser() {
    delete ui;
}

ItemPrice_s* ItemRandomiser::SplitItemPrice(quint32 Rand) {
    quint32 ItemPrice = Rand;
    quint8 multiplier = 1;
    ItemPrice_s* price = new ItemPrice_s;

    while (ItemPrice > 0xFFFF && multiplier < 0xFF) {
        multiplier++;
        ItemPrice = (Rand / multiplier);
    }
    price->ItemPrice = ItemPrice&0xFFFF;
    price->Multiplier = multiplier;
    return price;

}

void ItemRandomiser::Save() {
    QTime time;
    QVector<quint32> randnums;
    QRandomGenerator gen(static_cast<quint32>(time.msecsSinceStartOfDay())); //Use time in msecs as seed

    randnums.resize(0x172B); //Amount of items in Item.bin
    m_Prices.clear(); //Clear any previous values

    for (int i = 0; i < randnums.size(); i++) {
        randnums[i] = static_cast<quint32>(gen.bounded(ui->LE_MinPrice->text().toInt(), ui->LE_MaxPrice->text().toInt()));
        if (randnums[i]) //Make sure it's not 0
            m_Prices.push_back(SplitItemPrice(randnums[i]));
    }

    g_min = ui->LE_MinPrice->text().toInt();
    g_max = ui->LE_MaxPrice->text().toInt();
}

void ItemRandomiser::SetCanAccept(bool CanAccept) {
    m_CanSave = CanAccept;
    if (ui->LE_MinPrice->text() == "" || ui->LE_MaxPrice->text() == "")
        m_CanSave = false;

    ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(m_CanSave);
}

bool ItemRandomiser::FixRange(int min, int max) {
    QString maxstring = QString::number(max);
    QString minstring = QString::number(min);
    bool res = true;

    if (min < 1) { //If min is lowest possible value
        minstring = "";  //reset min string
        res = false;
    }

    if (max < 1) { //If max is lowest possible value
        maxstring = "";  //reset max string
        minstring = "";  //reset min string
        res = false;
    }

    if (max >= 1500000)
        maxstring = "1500000";

    if (min >= 1499999)
        minstring = "1499999";

    if (min >= max) {
        if (min < 1) { //If min is lowest possible value
            minstring = ""; //reset min string
            res = false;
        }

        else
            minstring = QString::number(max-1); // min can't go past max value
    }

    ui->LE_MaxPrice->setText(maxstring);
    ui->LE_MinPrice->setText(minstring);
    return res;
}

QVector<ItemPrice_s*> ItemRandomiser::GetPrices(void) {
    return m_Prices;
}

void ItemRandomiser::on_LE_MaxPrice_textChanged(const QString &maxtxt) {
    int max = maxtxt.toInt();
    int min = ui->LE_MinPrice->text().toInt();

    bool accept = FixRange(min, max);
    SetCanAccept(accept);
}

void ItemRandomiser::on_LE_MinPrice_textChanged(const QString &mintxt) {
    int max = ui->LE_MaxPrice->text().toInt();
    int min = mintxt.toInt();

    bool accept = FixRange(min, max);
    SetCanAccept(accept);
}

void ItemRandomiser::on_buttonBox_clicked(QAbstractButton *button)
{
    if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Close)) {
        qDebug() << "Closed";
        this->close();
    }

    else if (reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Save)) {
        qDebug() << "Saved";
        Save();
        QMessageBox::information(nullptr, "Saved!", "The Item Randomiser Settings have been saved.");
    }
}
