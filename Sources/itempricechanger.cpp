#include "Headers/itempricechanger.h"
#include "ui_itempricechanger.h"
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QRandomGenerator>
#include "Headers/textboxdelegate.h"
#include "Headers/file.h"
#include "Headers/mainwindow.h"
#include "Headers/game.h"

#define MAXITEMID 0x172B

static QVector<ItemPrice_s *> s_ItemPrices;
static bool g_TableIsSetup = false;

ItemPriceChanger::ItemPriceChanger(QWidget *parent) :
    QDialog(parent), ui(new Ui::ItemPriceChanger)
{
    ui->setupUi(this);
    m_IntValidator = new QIntValidator(-5000000, 5000000, this);
    ui->LE_RandomMin->setValidator(m_IntValidator);
    ui->LE_RandomMax->setValidator(m_IntValidator);
    QStringList ItemNameList;

    QFile list(QCoreApplication::applicationDirPath() + "/Resources/Items.txt");
    list.open(QFile::ReadOnly | QFile::Text);

    if (list.isOpen()) {
        QTextStream TextFile(&list);
        while (!TextFile.atEnd()) {
            ItemNameList.append(TextFile.readLine());
        }
        list.close();

        if (s_ItemPrices.empty()) {
            ResetPricesToDefault();
        }

        SetupTable(ItemNameList);
    }

    else {
        ui->tableWidget->clear();
        ui->tableWidget->setColumnCount(1);
        ui->tableWidget->setHorizontalHeaderLabels({"No Items File!"});
    }
}

ItemPriceChanger::~ItemPriceChanger()
{
    g_TableIsSetup = false;
    delete ui;
}

const QVector<ItemPrice_s *>& ItemPriceChanger::GetPrices(void) {
    return s_ItemPrices;
}

void ItemPriceChanger::ClearPrices(void) {
    for (auto price : s_ItemPrices) {
        delete price;
    }

    s_ItemPrices.clear();
    s_ItemPrices.resize(0);
}

//Used to comply with how the game does prices (u16 price * s8 multiplier)
void ItemPriceChanger::FixPrice(int& price, qint8& multiplier) {
    multiplier = (price<0) ? -1 : 1;
    if (price > 65535 || price < -65535) {
        multiplier = GetPriceMultiplier(price);
        while (multiplier == 1 || multiplier == -1) {
            (price<0) ? price++ : price--;
            multiplier = GetPriceMultiplier(price);
        }
    }
}

void ItemPriceChanger::ResetPricesToDefault(void) {
    Game* game = MainWindow::GetGameInstance();
    if (game != nullptr && game->m_Itembin.meta.exist) {
        File* Itembin = game->m_Itembin.file;
        s_ItemPrices.clear();

        for (quint32 i = 0; i < MAXITEMID; i++) { //Start at 1 to skip setting price for invalid items
        ItemPrice_s* price = new ItemPrice_s;
        price->ItemPrice = Itembin->Read<quint16>((0x1E*i)+2, QDataStream::LittleEndian);
        price->Multiplier = Itembin->Read<qint8>((0x1E*i)+0xE, QDataStream::LittleEndian);
        s_ItemPrices.push_back(price);
        }
    }
}

void ItemPriceChanger::SetupTable(QStringList& ItemNameList) {
    g_TableIsSetup = false;
    ui->tableWidget->clearSpans();
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setItemDelegateForColumn(2, new TextBoxDelegate(m_IntValidator, this));
    ui->tableWidget->setHorizontalHeaderLabels({"Item ID", "Item Name", "Item Buy Price", "Medals Buy Price",
                                                "Retail Sell Price", "Nookling Sell Price", "Island Sell Price"});

    int rows = 0;
    for (int i = 0; i < ItemNameList.size(); i++) {
        bool ok = false;

        if (ItemNameList[i].startsWith("//")) continue; //Skip Item Categories
        QStringList list = ItemNameList[i].split(",", QString::SkipEmptyParts);

        if ((list.size() == 2 && list[0].toInt(&ok, 16)) && ok == true) {
            QString ItemIDStr = list[0].remove(QRegularExpression("\\s"));
            quint32 itemID = (ItemIDStr.toUInt(&ok, 16)-0x2000);

            if (itemID >= MAXITEMID || itemID == 0) { //Exclude >0x2000 && Mii Mask / other illegal items prices
                //qDebug() << "Illegal: " << QString::number(itemID+0x2000, 16);
                continue;
            }

            int price = s_ItemPrices[static_cast<int>(itemID)]->ItemPrice*s_ItemPrices[static_cast<int>(itemID)]->Multiplier;
            if (price == 0) continue; //Items that can't be bought/sold have price of 0, so skip

            ui->tableWidget->insertRow(rows);
            QTableWidgetItem* IDEntry = new QTableWidgetItem(ItemIDStr);
            QTableWidgetItem* NameEntry = new QTableWidgetItem(list[1].remove(QRegularExpression("\\s{2,}"))); //Remove using regex; 2 or more whitespaces
            QTableWidgetItem* BuyPriceEntry = new QTableWidgetItem(QString::number(price));
            QTableWidgetItem* MedalsPriceEntry = new QTableWidgetItem(QString::number(price/50));
            QTableWidgetItem* RetailSellPriceEntry = new QTableWidgetItem(QString::number(price/4));
            QTableWidgetItem* NooksSellPriceEntry = new QTableWidgetItem(QString::number(price/5));
            QTableWidgetItem* IslandSellPriceEntry = new QTableWidgetItem(QString::number(price/80));

            IDEntry->setFlags(IDEntry->flags() & ~Qt::ItemIsEditable);
            NameEntry->setFlags(NameEntry->flags() & ~Qt::ItemIsEditable);
            BuyPriceEntry->setFlags(BuyPriceEntry->flags() | Qt::ItemIsEditable);
            MedalsPriceEntry->setFlags(NameEntry->flags() & ~Qt::ItemIsEditable);
            RetailSellPriceEntry->setFlags(NameEntry->flags() & ~Qt::ItemIsEditable);
            NooksSellPriceEntry->setFlags(NameEntry->flags() & ~Qt::ItemIsEditable);
            IslandSellPriceEntry->setFlags(NameEntry->flags() & ~Qt::ItemIsEditable);

            ui->tableWidget->setItem(rows, TableColumns::ItemID, IDEntry);
            ui->tableWidget->setItem(rows, TableColumns::ItemName, NameEntry);
            ui->tableWidget->setItem(rows, TableColumns::BuyPrice, BuyPriceEntry);
            ui->tableWidget->setItem(rows, TableColumns::MedalsBuyPrice, MedalsPriceEntry);
            ui->tableWidget->setItem(rows, TableColumns::RetailSellPrice, RetailSellPriceEntry);
            ui->tableWidget->setItem(rows, TableColumns::NooklingSellPrice, NooksSellPriceEntry);
            ui->tableWidget->setItem(rows, TableColumns::IslandSellPrice, IslandSellPriceEntry);
            rows++;
        }
    }
    g_TableIsSetup = true;
}

void ItemPriceChanger::SetupTable(void) {
    QStringList ItemNameList;

    QFile list(QCoreApplication::applicationDirPath() + "/Resources/Items.txt");
    list.open(QFile::ReadOnly | QFile::Text);

    if (list.isOpen()) {
        QTextStream TextFile(&list);
        while (!TextFile.atEnd()) {
            ItemNameList.append(TextFile.readLine());
        }
        list.close();
        SetupTable(ItemNameList);
    }
}

void ItemPriceChanger::SaveTable(void) {
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        int price = ui->tableWidget->item(i, TableColumns::BuyPrice)->text().toInt();
        int itemid =  ui->tableWidget->item(i, TableColumns::ItemID)->text().toInt(nullptr, 16);
        ItemPrice_s* bin = s_ItemPrices[itemid-0x2000];

        FixPrice(price, bin->Multiplier);
        bin->ItemPrice = (price/bin->Multiplier)&0xFFFF;
    }
}

void ItemPriceChanger::on_buttonBox_clicked(QAbstractButton *button)
{
    if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Close)) {
        qDebug() << "ItemPriceChanger: Closed";
        g_TableIsSetup = false;
        this->close();
    }

    else if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        qDebug() << "ItemPriceChanger: Restore Defaults";
        ResetPricesToDefault();
        SetupTable();
    }

    else if (reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Save)) {
        qDebug() << "ItemPriceChanger: Saved";
        SaveTable();
        QMessageBox::information(nullptr, "Saved!", "The Item Randomiser Settings have been saved.");
    }
}

qint8 ItemPriceChanger::GetPriceMultiplier(int price) {
    if (price >= 0) {
        for(qint8 i = 2; i < 127; i++) { //Start at 2 as 1 always factor
            if(price % i == 0) {
                if(price/i <= 0xFFFF) return i;
            }
        }
    }

    else if (price < 0) {
        for (qint8 i = -2; i > -128; i--) {
            if(price % i == 0) {
                if(price/i <= 0xFFFF) return i;
            }
        }
    }

    return 1;
}

void ItemPriceChanger::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    //bool stops the function being recursively called when we use QTableWidgetItem::setText below
    static bool running = false;
    if (running) return;
    running = true;

    int price = item->text().toInt();
    qint8 mul = 1; //FixPrice will set to -1 if needed
    FixPrice(price, mul);

    int ownrown = item->row();
    if (g_TableIsSetup) {
        ui->tableWidget->item(ownrown, TableColumns::BuyPrice)->setText(QString::number(price)); //BuyPriceEntry
        ui->tableWidget->item(ownrown, TableColumns::MedalsBuyPrice)->setText(QString::number(price/50)); //MedalsPriceEntry
        ui->tableWidget->item(ownrown, TableColumns::RetailSellPrice)->setText(QString::number(price/4)); //RetailSellPriceEntry
        ui->tableWidget->item(ownrown, TableColumns::NooklingSellPrice)->setText(QString::number(price/5)); //NooksSellPriceEntry
        ui->tableWidget->item(ownrown, TableColumns::IslandSellPrice)->setText(QString::number(price/80)); //IslandSellPriceEntry
    }

    QList<QTableWidgetItem *> selitems = ui->tableWidget->selectedItems();
    for (auto selitem : selitems) {
        int currow = selitem->row();
        ui->tableWidget->item(currow, TableColumns::BuyPrice)->setText(QString::number(price)); //BuyPriceEntry
        ui->tableWidget->item(currow, TableColumns::MedalsBuyPrice)->setText(QString::number(price/50)); //MedalsPriceEntry
        ui->tableWidget->item(currow, TableColumns::RetailSellPrice)->setText(QString::number(price/4)); //RetailSellPriceEntry
        ui->tableWidget->item(currow, TableColumns::NooklingSellPrice)->setText(QString::number(price/5)); //NooksSellPriceEntry
        ui->tableWidget->item(currow, TableColumns::IslandSellPrice)->setText(QString::number(price/80)); //IslandSellPriceEntry
    }
    running = false;
}

void ItemPriceChanger::on_CMB_FilterType_currentIndexChanged(int index)
{
    ui->CB_CaseSensitive->setEnabled(false);
    if (index == FilterType::ItemName) ui->LE_Filter->setPlaceholderText("Filter by Item Name");
    else if (index == FilterType::ItemID) ui->LE_Filter->setPlaceholderText("Filter by Item ID");
    else if (index == FilterType::BuyPrice) ui->LE_Filter->setPlaceholderText("Filter by Buy Price");
    else if (index == FilterType::SellPrice) ui->LE_Filter->setPlaceholderText("Filter by Retail Sell Price");
    else ui->LE_Filter->setPlaceholderText("Filter...");

    if (index == FilterType::ItemName) ui->CB_CaseSensitive->setEnabled(true);
    ui->LE_Filter->clear();
    ResetVisibleRows();
}

void ItemPriceChanger::FilterVisibleRows(const QString& filter) {
    ResetVisibleRows();
    if (filter == "") {
        return;
    }

    TableColumns column = TableColumns::ItemName;
    int index = ui->CMB_FilterType->currentIndex();

    if (index == FilterType::ItemID) column = TableColumns::ItemID;
    else if (index == FilterType::BuyPrice) column = TableColumns::BuyPrice;
    else if (index == FilterType::SellPrice) column = TableColumns::RetailSellPrice;

    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QTableWidgetItem *item = ui->tableWidget->item(i, column);
        if (column == TableColumns::BuyPrice || column == TableColumns::RetailSellPrice) {
            if(item->text().toInt(nullptr, 16) != filter.toInt(nullptr, 16)) {
                ui->tableWidget->setRowHidden(i, true);
            }
        }

        else {
            Qt::CaseSensitivity casesensitive = Qt::CaseInsensitive;
            if (column == TableColumns::ItemName)
                casesensitive = (ui->CB_CaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

            if(!item->text().contains(filter,  casesensitive)) {
                ui->tableWidget->setRowHidden(i, true);
            }
        }
    }
}

void ItemPriceChanger::ResetVisibleRows() {
    for( int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHidden(i, false);
    }
}

void ItemPriceChanger::on_LE_Filter_textEdited(const QString &arg1)
{
    FilterVisibleRows(arg1);
}

void ItemPriceChanger::on_CB_CaseSensitive_stateChanged(int arg1)
{
    (void)arg1; //Suppress unused warning
    FilterVisibleRows(ui->LE_Filter->text());
}

void ItemPriceChanger::on_LE_RandomMin_textEdited(const QString &arg1)
{
    ui->BTN_Randomise->setEnabled(false);
    if (ui->LE_RandomMax->text() != "" && arg1 != "") {
        ui->BTN_Randomise->setEnabled(true);
    }
}

void ItemPriceChanger::on_LE_RandomMax_textEdited(const QString &arg1)
{
    ui->BTN_Randomise->setEnabled(false);
    if (ui->LE_RandomMin->text() != "" && arg1 != "") {
        ui->BTN_Randomise->setEnabled(true);
    }
}

void ItemPriceChanger::on_LE_RandomMin_editingFinished()
{
    if (ui->LE_RandomMax->text() != "") {
        if (ui->LE_RandomMin->text().toInt() > ui->LE_RandomMax->text().toInt()) {
            ui->LE_RandomMin->setText(ui->LE_RandomMax->text());
        }
    }
}

void ItemPriceChanger::on_LE_RandomMax_editingFinished()
{
    if (ui->LE_RandomMin->text() != "") {
        if (ui->LE_RandomMax->text().toInt() < ui->LE_RandomMin->text().toInt()) {
            ui->LE_RandomMax->setText(ui->LE_RandomMin->text());
        }
    }
}

void ItemPriceChanger::on_BTN_Randomise_clicked()
{
    if (ui->LE_RandomMin->text() != "" && ui->LE_RandomMax->text() != "") {
        for( int i = 0; i < ui->tableWidget->rowCount(); ++i) {
            QRandomGenerator *gen = QRandomGenerator::global();
            int price = gen->bounded(ui->LE_RandomMin->text().toInt(), ui->LE_RandomMax->text().toInt());
            qint8 mul = 1; //unneeded
            FixPrice(price, mul);
            ui->tableWidget->item(i, TableColumns::BuyPrice)->setText(QString::number(price));
        }
    }
}
