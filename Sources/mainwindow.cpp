#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "Headers/aboutwindow.h"
#include "Headers/itempricechanger.h"
#include "Headers/buttonremap.h"
#include "Headers/shophourseditor.h"
#include "Headers/file.h"
#include "Headers/game.h"
#include "Headers/patch.h"

#define VERSION "v1.1"

static const QString SpeedStrings[5] = {"Normal: x1", "Fast: x1.25", "Faster: x1.5", "Speedy: x2", "Speeding Bullet: x3"};
static const QString CameraZoomOutStrings[6] = {"Normal", "Tiny", "Small", "Medium", "Large", "Huge"};
static const QString CameraZoomBaseStr = "Camera Zoom Out Level: ";

MainWindow* MainWindowInstance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString title = "ACNL REditor ";
    title += VERSION;

#ifdef QT_NO_DEBUG_OUTPUT //Only set for release ∴ can auto disable debug tab for release
    ui->menuDebug->menuAction()->setVisible(false);
#else
    title += " [Debug]";
#endif

    this->setWindowTitle(title);
    ui->label_PlyrSpeed->setText(SpeedStrings[0]);
    ui->LBL_CameraZoomOut->setText(CameraZoomBaseStr+CameraZoomOutStrings[0]);
    exedir = QCoreApplication::applicationDirPath();

    QImage CameraZoomImage(exedir + "/Resources/Images/CameraZoomOut/0.png", "PNG");

    if (!CameraZoomImage.isNull()) {
        ui->LBL_CameraZoomOutPic->setPixmap(QPixmap::fromImage(CameraZoomImage));
        ui->LBL_CameraZoomOutPic->show();
    }

    MainWindowInstance = this;
}

MainWindow::~MainWindow() {
    delete ui;
    MainWindowInstance = nullptr;
}

Game* MainWindow::GetGameInstance(void) {
    return MainWindowInstance->game;
}

void MainWindow::OpenGame(const QString setupdir) {
    this->dir = setupdir;
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

    ui->BTN_ItemPriceChanger->setEnabled(game->m_Itembin.meta.exist); //Can only use if Item.bin exists
    ui->statusBar->showMessage("Folder opened successfully! Current Game: " + game->GetCurrentGame(), 10000);
}

void MainWindow::on_FileOpen_triggered() {
    dir = QFileDialog::getExistingDirectory(this, tr("Open \"rom\" Directory"),
                    QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir == "\0") { //If no dir selected
        ui->statusBar->showMessage("No folder was opened!", 10000);
        return;
    }

    this->OpenGame(dir);
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

        res = game->ApplyShopTimes(codebin_o);
        if(!res) {
            errors++;
            QMessageBox::warning(nullptr, "ApplyShopTimes Error", "At least one PatchCode Failed in ApplyShopTimes.");
        }

        res = ButtonRemap::Apply(codebin_o);
        if(!res) {
            errors++;
            QMessageBox::warning(nullptr, "ButtonRemap Error", "At least one PatchCode Failed in ButtonRemap::Apply.");
        }

        codebin_o->m_file->flush();
        codebin_o->m_file->close();
    }

    if (game->m_Itembin.meta.exist) {
        File *ItemBin_o = new File(outdir + "/romfs/Item/Param/Item.bin", QFile::ReadWrite);
        game->ApplyItemRandomiser(ItemBin_o);
        ItemBin_o->m_file->flush();
        ItemBin_o->m_file->close();
    }
    ui->statusBar->showMessage("Saving Has Succeeded! There was " + QString::number(errors) + " errors!", 10000);
}

void MainWindow::on_FileClose_triggered() {
    qDebug() << "Close Pressed";
    ItemPriceChanger::ClearPrices();
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
    ui->label_PlyrSpeed->setText(SpeedStrings[value]);
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

void MainWindow::on_BTN_Remapper_clicked()
{
    ButtonRemap remapWindow;
    remapWindow.setModal(false);
    remapWindow.exec();
}

void MainWindow::on_BTN_ItemPriceChanger_clicked()
{
    qDebug() << "Item Price Changer Pressed";
    ItemPriceChanger priceWindow;
    priceWindow.setModal(false);
    priceWindow.exec();
}

void MainWindow::on_dial_CameraZoomOut_valueChanged(int value)
{
    ui->LBL_CameraZoomOut->setText(CameraZoomBaseStr+CameraZoomOutStrings[value]);
    ui->LBL_CameraZoomOutPic->hide();
    QString basepath = QCoreApplication::applicationDirPath() + "/Resources/Images/CameraZoomOut/";
    QImage image(basepath + QString::number(value, 10) + ".png", "PNG");

    if (!image.isNull()) {
        ui->LBL_CameraZoomOutPic->setPixmap(QPixmap::fromImage(image));
        ui->LBL_CameraZoomOutPic->show();
    }
}


void MainWindow::on_CB_CyrusCustoms_stateChanged(int arg1)
{
    ui->LE_CustomiseSeconds->setEnabled(false); //Nothing or Instant Unlock (nocheck or partially checked)

    if (arg1 == Qt::Checked) { //Custom value
        ui->LE_CustomiseSeconds->setEnabled(true);
    }
}

void MainWindow::on_LE_CustomiseSeconds_textChanged(const QString &arg1)
{
    static constexpr char lblText[] = "Days: %02d | Hours: %02d | Mins: %02d | Seconds: %02d";
    bool res = false;
    quint32 days = 0, hours = 0, mins = 0, secs = 1;
    quint32 val = arg1.toUInt(&res, 10);

    if (res == true) {
        secs = val%60;
        mins = val/(60);
        hours = mins/60;
        mins %= 60;
        days = hours/24;
        hours%= 24;
    }

    ui->LBL_CustomiseParsed->setText(QString::asprintf(lblText, days,hours,mins,secs));
}


/* Debug Stuff */
void MainWindow::on_actionEnableAll_triggered()
{
#ifdef QT_NO_DEBUG_OUTPUT //Only set for release ∴ can auto disable debug tab for release
    return;
#else
    ui->tab_romfs->setEnabled(true);
    ui->tab_exefs->setEnabled(true);
    ui->tab_cro->setEnabled(true);
#endif
}

void MainWindow::on_actionListItemPrices_triggered()
{
#ifdef QT_NO_DEBUG_OUTPUT //Only set for release ∴ can auto disable debug tab for release
    return;
#else
    QVector<ItemPrice_s *> ItemPrices = ItemPriceChanger::GetPrices();
    qDebug() << "ItemPrices.size(): " << ItemPrices.size();
    for (int i = 0; i < ItemPrices.size(); i++) {
        qDebug() << i+1 << ":  Price: " << ItemPrices[i]->ItemPrice * ItemPrices[i]->Multiplier;
    }
#endif
}

void MainWindow::on_actionSetup_OutDir_triggered()
{
#ifdef QT_NO_DEBUG_OUTPUT //Only set for release ∴ can auto disable debug tab for release
    return;
#else
    game->SetupOutDir(exedir, exedir + "/luma/titles/" + TIDs[game->m_GameType]);
#endif
}

void MainWindow::on_actionQuick_Open_EUR_triggered()
{
#ifdef QT_NO_DEBUG_OUTPUT //Only set for release ∴ can auto disable debug tab for release
    return;
#else
    QString path = QFileInfo(exedir + "../../../../../ACNL_REditor/Orig_EUR/rom/").absolutePath();
    qDebug() << path;
    this->OpenGame(path);
#endif
}
