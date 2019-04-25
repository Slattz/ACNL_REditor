#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "Headers/aboutwindow.h"
#include "Headers/itemrandomiser.h"
#include "Headers/buttonremap.h"
#include "Headers/shophourseditor.h"
#include "Headers/file.h"
#include "Headers/game.h"
#include "Headers/patch.h"

static const QString SpeedStrings[5] = {"Normal: x1", "Fast: x1.25", "Faster: x1.5", "Speedy: x2", "Speeding Bullet: x3"};
static QVector<ItemPrice_s*> ItemPrices;

MainWindow* MainWindowInstance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_PlyrSpeed->setText(SpeedStrings[0]);
#ifdef QT_NO_DEBUG_OUTPUT //Only set for release ∴ can auto disable debug tab for release
    ui->menuDebug->menuAction()->setVisible(false);
#endif
    exedir = QCoreApplication::applicationDirPath();
    MainWindowInstance = this;
}

MainWindow::~MainWindow() {
    delete ui;
}

Game* MainWindow::GetGameInstance(void) {
    return MainWindowInstance->game;
}

void MainWindow::on_FileOpen_triggered() {
    dir = QFileDialog::getExistingDirectory(this, tr("Open \"rom\" Directory"),
                    QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir == "\0") { //If no dir selected
        ui->statusBar->showMessage("No folder was opened!", 10000);
        return;
    }

    game = new Game(dir);
    if (!game->codebin->Exists() && !game->m_romfs.exist) {
        if (game)
            delete game;

        game = nullptr;
        ui->statusBar->showMessage("No exefs or romfs existed!", 10000);
        return;
    }

    Patch::Init();

    ui->FileOpen->setEnabled(false); //Can't open more files
    ui->FileSave->setEnabled(true); //Can now save files
    ui->FileClose->setEnabled(true); //Can now close files

    ui->tab_romfs->setEnabled(game->m_romfs.exist); //Can only use Romfs tab if folder exists
    ui->tab_exefs->setEnabled(game->m_exefs.exist); //Can only use Exefs tab if folder exists
    ui->tab_cro->setEnabled(false); //No CRO cheats atm [Can only use CRO tab if folder exists]

    ui->BTN_RandomItemPrice->setEnabled(game->m_Itembin.meta.exist); //Can only use if Item.bin exists
    ui->statusBar->showMessage("Folder opened successfully! Current Game: " + game->GetCurrentGame(), 10000);
}

void MainWindow::on_FileSave_triggered()
{
    bool res = true;
    quint32 errors = 0;
    qDebug() << "Save Pressed";

    game->SetCodebinExists(dir); //Check if code.bin exists again as may be deleted since last check
    game->SetRomfsExists(dir); //Check if romfs exists again as may be deleted since last check
    game->SetFilesExist(); //Check if romfs files exists again as may be deleted since last check
    outdir = exedir + "/luma/titles/" + TIDs[game->m_GameType];

    if (!game->SetupOutDir(exedir, outdir)) {
        QMessageBox::warning(nullptr, "Error Creating Folder", "The Output Folder couldn't be created.\nNo modifications will be saved.");
        ui->statusBar->showMessage("Saving Failed!", 10000);
        return;
    }

    if (game->m_exefs.exist) {
        File *codebin_o = new File(outdir + "/code.bin", QFile::ReadWrite);
        res = game->ApplyPatches(ui, codebin_o);
        if(!res) {
            errors++;
            QMessageBox::warning(nullptr, "ApplyPatches Error", "At least one PatchCode Failed in ApplyPatches.");
        }

        game->ApplyCustomShopCode(codebin_o);
        res = game->ApplyAlwaysOpenPatches(codebin_o);
        if(!res) {
            errors++;
            QMessageBox::warning(nullptr, "ApplyAlwaysOpenPatches Error", "At least one PatchCode Failed in ApplyAlwaysOpenPatches.");
        }

        res = game->ApplyNormalShopTimes(codebin_o);
        if(!res) {
            errors++;
            QMessageBox::warning(nullptr, "ApplyNormalShopTimes Error", "At least one PatchCode Failed in ApplyNormalShopTimes.");
        }

        res = game->ApplyNightShopTimes(codebin_o);
        if(!res) {
            errors++;
            QMessageBox::warning(nullptr, "ApplyNightShopTimes Error", "At least one PatchCode Failed in ApplyNightShopTimes.");
        }

        res = game->ApplyEarlyShopTimes(codebin_o);
        if(!res) {
            errors++;
            QMessageBox::warning(nullptr, "ApplyEarlyShopTimes Error", "At least one PatchCode Failed in ApplyEarlyShopTimes.");
        }

        codebin_o->m_file->flush();
        codebin_o->m_file->close();
    }

    if (game->m_Itembin.meta.exist) {
        File *ItemBin_o = new File(outdir + "/romfs/Item/Param/Item.bin", QFile::ReadWrite);
        game->ApplyItemRandomiser(ItemBin_o, ItemPrices);
        ItemBin_o->m_file->flush();
        ItemBin_o->m_file->close();
    }
    ui->statusBar->showMessage("Saving Has Succeeded! There was " + QString::number(errors) + " errors!", 10000);
}

void MainWindow::on_FileClose_triggered() {
    qDebug() << "Close Pressed";
    ui->FileOpen->setEnabled(true); //Can open files
    ui->FileSave->setEnabled(false); //Can't save files
    ui->FileClose->setEnabled(false); //Can't close files
    ui->tab_romfs->setEnabled(false); //Disable Romfs tab
    ui->tab_exefs->setEnabled(false); //Disable Exefs tab
    ui->tab_cro->setEnabled(false); //Disable CRO tab
    if (game)
        delete game;
}

void MainWindow::on_dial_PlyrSpeed_valueChanged(int value) {
    ui->label_PlyrSpeed->setText(SpeedStrings[value-1]);
}

void MainWindow::on_BTN_RandomItemPrice_clicked() {
    qDebug() << "Random Items Pressed";
    ItemRandomiser randWindow;
    randWindow.setModal(false);
    randWindow.exec();
    ItemPrices.clear();
    ItemPrices = randWindow.GetPrices();
}

void MainWindow::on_BTN_ShopTimes_clicked()
{
    qDebug() << "Shop Hours Pressed";
    ShopHoursEditor shopWindow;
    shopWindow.setModal(false);
    shopWindow.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    qDebug() << "About Pressed";
    AboutWindow aboutWindow;
    aboutWindow.setModal(false);
    aboutWindow.exec();
}

/* Debug Stuff */
void MainWindow::on_actionEnableAll_triggered()
{
    ui->tab_romfs->setEnabled(true);
    ui->tab_exefs->setEnabled(true);
    ui->tab_cro->setEnabled(true);
}

void MainWindow::on_actionListItemPrices_triggered()
{
    qDebug() << "ItemPrices.size(): " << ItemPrices.size();
    for (int i = 0; i < ItemPrices.size(); i++) {
        qDebug() << i+1 << ":  Price: " << ItemPrices[i]->ItemPrice * ItemPrices[i]->Multiplier;
    }
}

void MainWindow::on_actionSetup_OutDir_triggered()
{
    game->SetupOutDir(exedir, exedir + "/luma/titles/" + TIDs[game->m_GameType]);
}

void MainWindow::on_BTN_Remapper_clicked()
{
    ButtonRemap remapWindow;
    remapWindow.setModal(false);
    remapWindow.exec();
}
