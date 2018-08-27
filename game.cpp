#include "game.h"
#include "mainwindow.h"

/* Each entry relative to Game_s */
QString TIDs[8] = {"0004000000086200", "0004000000086300", "0004000000086400", "0004000000086500",  //Orig Game
                   "0004000000198d00", "0004000000198e00", "0004000000198f00", "0004000000199000"}; //WA Preinstalled Game

static const QByteArray CodebinMD5Hashes[8] = {"361749f1011b7a4deacc1cba3f1553fe", "0d5bebb090e4d7169c2f6be636b76291",
                                 "4c487a97506dba4adb2fd4b439f4ed90", "fffffffffffffffffffffffffffffffe",
                                 "244f373ef2e8c37417534173f1c9c7e1", "0d5bebb090e4d7169c2f6be636b76291",
                                 "846d694886bcab0fdd43f62a02a0c24c", "ffffffffffffffffffffffffffffffff"};

static const quint32    PlayerSpeeds[5]   = {0x3F800000, 0x3fa00000, 0x3fc00000, 0x40000000, 0x40400000};
static const quint8     ConfettiAmount[5] = {0x0F, 0x3C, 0x78, 0xB4, 0xF0};
static const quint8     CherryBAmount[5]  = {0x0F, 0x1E, 0x3C, 0x78, 0xF0};

const QString notcodebin = "code.bin is not from ACNL / the latest version of ACNL!\nSome features may be disabled as a result.";

Game::Game() { }
Game::~Game() {
    delete codebin;
}

Game::Game(const QString &dir)
    : codebin(new File(dir + "/exefs/code.bin", QFile::ReadOnly))
{
    SetRomfsExists(dir);

    if (!codebin->Exists() && !m_romfs.exist)
        QMessageBox::warning(nullptr, "No Files Exist!", "There is no exefs folder or romfs folder in the folder you have provided.");

    if (codebin->Exists() && SetCodebinExists(dir)) { //TODO: Check if code.bin is compressed or not
       if (DetermineGame() < 0)
           QMessageBox::warning(nullptr, "Invalid code.bin", notcodebin);
    }
}

int Game::DetermineGame(void)
{
    m_codemd5Hash = codebin->getHash();
    for (int i = 0; i < 8; i++) {
        if (m_codemd5Hash.toHex() == CodebinMD5Hashes[i]) {
            qDebug() << "Code.bin is GameType" << i;
            m_GameType = static_cast<GameType>(i);
            return i;
        }
    }
    /* Not ACNL */
    m_exefs.exist = false;
    return -1;
}

bool Game::SetRomfsExists(const QString &dir) {
    if (QDir(dir + "/romfs").exists()) {
        m_romfs.exist = true;
        m_romfs.dir = dir + "/romfs";
        SetFilesExist();
    }

    else {
        m_romfs.exist = false;
    }

    return m_romfs.exist;
}

bool Game::SetFilesExist(void) {
    if (QDir(m_romfs.dir + "/Item/Param").exists()) {
        m_Itembin.file = new File(m_romfs.dir + "/Item/Param/Item.bin", QFile::ReadOnly);
        if (m_Itembin.file->Exists()) {
            m_Itembin.meta.exist = true;
            m_Itembin.meta.dir = m_romfs.dir + "/Item/Param/Item.bin";
        }

        else {
            m_Itembin.meta.exist = false;
        }
    }
    return true;
}

bool Game::SetCodebinExists(const QString &dir) {
    if (QFile(dir + "/exefs/code.bin").exists()) {
        m_exefs.exist = true;
        m_exefs.dir = dir + "/exefs";
    }

    return m_exefs.exist;
}

bool Game::SetupOutDir(const QString &exedir, const QString &outdir) {
    /* Create Output Folder Base */
    if (!QDir(exedir + "/luma/").exists()) {
        if(!QDir().mkdir(exedir + "/luma/"))
            return false;
    }

    if (!QDir(exedir + "/luma/titles").exists()) {
        if(!QDir().mkdir(exedir + "/luma/titles"))
            return false;
    }

    if (!QDir(outdir).exists()) {
        if(!QDir().mkdir(outdir))
            return false;
    }

    /* Create romfs folder if it existed */
    if (m_romfs.exist) {
        if (!QDir(outdir + "/romfs").exists()) {
            if(!QDir().mkdir(outdir + "/romfs"))
                return false;
        }

        if (!QDir(outdir + "/romfs/Item").exists()) {
            if(!QDir().mkdir(outdir + "/romfs/Item"))
                return false;
        }

        if (!QDir(outdir + "/romfs/Item/Param").exists()) {
            if(!QDir().mkdir(outdir + "/romfs/Item/Param"))
                return false;
        }
    }

    /* Copy code.bin to output folder if it existed */
    if (m_exefs.exist && codebin->Exists()) {
        QFile old(outdir + "/code.bin");
        if(old.exists())
            old.remove(); //No need to close. File closed before removed.

       codebin->m_file->copy(outdir + "/code.bin");
    }

    if (m_romfs.exist) {
        if (m_Itembin.meta.exist) {
            QFile old(outdir + "/romfs/Item/Param/Item.bin");
            if(old.exists())
                old.remove(); //No need to close. File closed before removed.

           m_Itembin.file->m_file->copy(outdir + "/romfs/Item/Param/Item.bin");
        }
    }

    return true;
}

bool Game::PatchCode(File *codebin, quint32 offset, quint32 code) {

    if(!codebin->Exists() || !offset || !code || offset >= codebin->m_file->size())
        return false;

    return codebin->Write(offset-0x100000, code, QDataStream::LittleEndian); //-0x100000 is actual code.bin file offset
}

quint32 Game::ReadCode(File *codebin, quint32 offset) {

    if(!codebin->Exists() || !offset || offset >= codebin->m_file->size())
        return false;

    return codebin->Read(offset-0x100000, QDataStream::LittleEndian); //-0x100000 is actual code.bin file offset
}

bool Game::ApplyPatches(Ui::MainWindow *mainui, File *codebin) {
    bool res = false;
    /* Exefs->General */
    if (mainui->CB_Resetti->isChecked())
        res |= !PatchCode(codebin, Patch::NoResetti.Offsets[m_GameType], Patch::NoResetti.Value[0]);

    if(mainui->CB_NativeFruitSellPrice->isChecked())
        res |= !PatchCode(codebin, Patch::NativeFruitPrice.Offsets[m_GameType], Patch::NativeFruitPrice.Value[0]);

    if(mainui->CB_ReeseBuy->isChecked())
        res |= !PatchCode(codebin, Patch::ReeseBuy.Offsets[m_GameType], Patch::ReeseBuy.Value[0]);

    if(mainui->CB_NooklingBuy->isChecked()) {
        res |= !PatchCode(codebin, Patch::NooklingsBuy.Offsets[m_GameType], Patch::NooklingsBuy.Value[0]);
        res |= !PatchCode(codebin, Patch::NooklingsBuy.Offsets[m_GameType]+0x8, Patch::NooklingsBuy.Value[1]);
    }

    if(mainui->CB_LeilaBuy->isChecked())
        res |= !PatchCode(codebin, Patch::LeilaBuy.Offsets[m_GameType], Patch::LeilaBuy.Value[0]);

    if(mainui->CB_Confetti->isChecked()) {
        res |= !PatchCode(codebin, Patch::Confetti.Offsets[m_GameType], Patch::Confetti.Value[0]);
        res |= !PatchCode(codebin, Patch::Confetti.Offsets[m_GameType]+0x30, Patch::Confetti.Value[1]);
    }

    if(mainui->CB_CherryB->isChecked()) {
        res |= !PatchCode(codebin, Patch::CherryBlossom.Offsets[m_GameType], Patch::CherryBlossom.Value[0]);
        res |= !PatchCode(codebin, Patch::CherryBlossom.Offsets[m_GameType]+0x28, Patch::CherryBlossom.Value[0]);
        res |= !PatchCode(codebin, Patch::CherryBlossom.Offsets[m_GameType]+0x50, Patch::CherryBlossom.Value[1]);
        res |= !PatchCode(codebin, Patch::CherryBlossom.Offsets[m_GameType]+0x60, Patch::CherryBlossom.Value[2]);
    }

    if(mainui->CMB_Confetti->currentIndex() != 2) { //If not set at normal
        quint32 code = ReadCode(codebin, Patch::Confetti.Offsets[m_GameType]+0x30); //Read existing code
        code = (code & 0xFFFFFF00) | ConfettiAmount[mainui->CMB_Confetti->currentIndex()];
        res |= !PatchCode(codebin, Patch::Confetti.Offsets[m_GameType]+0x30, code);
    }

    if(mainui->CMB_CherryB->currentIndex() != 2) { //If not set at normal
        quint32 code = ReadCode(codebin, Patch::CherryBlossom.Offsets[m_GameType]+0x68); //Read existing code
        code = (code & 0xFFFFFF00) | CherryBAmount[mainui->CMB_CherryB->currentIndex()];
        res |= !PatchCode(codebin, Patch::CherryBlossom.Offsets[m_GameType]+0x68, code);
    }

    if(mainui->CMB_Weather->currentIndex() != 7) //If not set at no changes
        res |= !PatchCode(codebin, Patch::Weather.Offsets[m_GameType], Patch::Weather.Value[0] | static_cast<quint8>(mainui->CMB_Weather->currentIndex()));

    /* Exefs->Player */
    res |= !PatchCode(codebin, Patch::PlayerSpeed.Offsets[m_GameType], PlayerSpeeds[mainui->dial_PlyrSpeed->value()-1]);
    if (mainui->CB_EditPatterns->isChecked())
        res |= !PatchCode(codebin, Patch::EditPattern.Offsets[m_GameType], Patch::EditPattern.Value[0]);

    if(mainui->CB_TrampleFlowers->isChecked())
        res |= !PatchCode(codebin, Patch::FlowersNoTrample.Offsets[m_GameType], Patch::FlowersNoTrample.Value[0]);

    if(mainui->CB_Mosquito->isChecked())
        res |= !PatchCode(codebin, Patch::NoMosquito.Offsets[m_GameType], Patch::NoMosquito.Value[0]);

    /* Exefs->Utilities */
    if(mainui->CB_RegionPass->isChecked())
        res |= !PatchCode(codebin, Patch::RegionCheck.Offsets[m_GameType], Patch::RegionCheck.Value[0]);

    if(mainui->CB_ChecksumPass->isChecked())
        res |= !PatchCode(codebin, Patch::ChecksumCheck.Offsets[m_GameType], Patch::ChecksumCheck.Value[0]);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::ApplyItemRandomiser(File *Itembin, QVector<ItemPrice_s *> ItemPrices) {
    if(!Itembin->Exists())
        return false;

    QDataStream fout(Itembin->m_file);
    fout.setByteOrder(QDataStream::LittleEndian);

    if (ItemPrices.size() > 0x172B)
        ItemPrices.resize(0x172B);

    for (int i = 1; i < ItemPrices.size()-1; i++) { //Start at 1 to skip setting price for invalid items
        Itembin->m_file->seek((0x1e*i)+2); //u16 ItemPrice
        fout << ItemPrices[i]->ItemPrice;

        Itembin->m_file->seek((0x1e*i)+0xE); //u8 Multiplier
        fout << ItemPrices[i]->Multiplier;
    }

    if (!Itembin->m_file->flush()) //Flush file
        return false;

    return true;
}

bool Game::WriteCustomShopCode(File *codebin, quint32 offset) {
    bool res = false;
    res |= !PatchCode(codebin, offset, 0xE1A00000); //NOP: Removes unused BL
    res |= !PatchCode(codebin, offset+4, 0xE1A00000); //NOP: Removes unused ADD
    res |= !PatchCode(codebin, offset+8, MAKE_BRANCH_LINK(offset+8, Patch::NightOwl.Offsets[m_GameType])); //BL: Sets up BL to IsNightOwlOrdinance
    res |= !PatchCode(codebin, offset+0xC, 0xE3500000); //CMP R0, #0
    res |= !PatchCode(codebin, offset+0x10, 0xE1A00C44); //MOV R0, R4,ASR#24
    res |= !PatchCode(codebin, offset+0x14, 0xE1A00000); //NOP: This is where SUBEQ for normal time will go
    res |= !PatchCode(codebin, offset+0x18, 0xE1A00000); //NOP: This is where SUBNE for night time will go
    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

void Game::ApplyCustomShopCode(File *codebin) {
    for (int i = 0; i < 5; i++) {
      if (Nooklings[i].RequiresCustomCode)
          Nooklings[i].HasNecessaryCode = WriteCustomShopCode(codebin, Patch::Nooklings.Offsets[m_GameType]+Nooklings[i].off_NormalOpen-0x14);
    }

    if (Garden.RequiresCustomCode)
        Garden.HasNecessaryCode = WriteCustomShopCode(codebin, Patch::Garden.Offsets[m_GameType]+Garden.off_NormalOpen-0x14);

    if (Ables.RequiresCustomCode)
        Ables.HasNecessaryCode = WriteCustomShopCode(codebin, Patch::Ables.Offsets[m_GameType]+Ables.off_NormalOpen-0x14);

    if (Sham.RequiresCustomCode)
        Sham.HasNecessaryCode = WriteCustomShopCode(codebin, Patch::Sham.Offsets[m_GameType]+Sham.off_NormalOpen-0x14);

    if (Kicks.RequiresCustomCode)
        Kicks.HasNecessaryCode = WriteCustomShopCode(codebin, Patch::Kicks.Offsets[m_GameType]+Kicks.off_NormalOpen-0x14);

    if (Nooks.RequiresCustomCode)
        Nooks.HasNecessaryCode = WriteCustomShopCode(codebin, Patch::Nooks.Offsets[m_GameType]+Nooks.off_NormalOpen-0x14);

    if (Katrina.RequiresCustomCode)
        Katrina.HasNecessaryCode = WriteCustomShopCode(codebin, Patch::Katrina.Offsets[m_GameType]+Katrina.off_NormalOpen-0x14);
}

bool Game::ApplyAlwaysOpenPatches(File *codebin) {
    bool res = false;

    if(g_AlwaysOpen.Retail)
        res |= !PatchCode(codebin, Patch::Retail.Offsets[m_GameType]+Retail.off_AlwaysOpen, SHOPALWAYSOPEN);

    for (int i = 0; i < 5; i++) { /* For each nookling shop */
        static const quint8  off_BLEarly[5] = {0x3C, 0x54, 0x3C, 0x54, 0x3C};
        quint32 off_AlwaysOpen = Patch::Nooklings.Offsets[m_GameType]+Nooklings[i].off_NormalOpen-off_BLEarly[i];

        if(g_AlwaysOpen.Nooklings[i])
            res |= !PatchCode(codebin, off_AlwaysOpen, MAKE_BRANCH(off_AlwaysOpen, Patch::Nooklings.Offsets[m_GameType]+Nooklings[i].off_AlwaysOpen));
    }

    if(g_AlwaysOpen.Garden)
        res |= !PatchCode(codebin, Patch::Garden.Offsets[m_GameType]+Garden.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Ables)
        res |= !PatchCode(codebin, Patch::Ables.Offsets[m_GameType]+Ables.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Sham)
        res |= !PatchCode(codebin, Patch::Sham.Offsets[m_GameType]+Sham.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Kicks)
        res |= !PatchCode(codebin, Patch::Kicks.Offsets[m_GameType]+Kicks.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Nooks)
        res |= !PatchCode(codebin, Patch::Nooks.Offsets[m_GameType]+Nooks.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Katrina)
        res |= !PatchCode(codebin, Patch::Katrina.Offsets[m_GameType]+Katrina.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Redd)
        res |= !PatchCode(codebin, Patch::Redd.Offsets[m_GameType]+Redd.off_AlwaysOpen, SHOPALWAYSOPEN);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::MakeShopHourCode(File *codebin, Patch ShopBaseAddr, ShopCodeOffsets_s ShopOffsetInfo, HoursInfo_s Hours, quint8 HourType) {
    bool res = false;
    quint32 subcode, cmpcode, checkcode = 0;
    static const quint8 closeoffs[3] = {0x18, 0x8, 0x4}; //Normal, Night, Early
    static const quint32 subvals[3] = {0x02400000, 0x12400000, 0xE2400000}; //SUBEQ for normal, SUBNE for night, SUB for early
    const quint16 off_ShopOpen[3] = {ShopOffsetInfo.off_NormalOpen, ShopOffsetInfo.off_NightOpen, ShopOffsetInfo.off_EarlyOpen};

    quint8 ClosingHour = (Hours.CloseHour == 0) ? 24 : Hours.CloseHour; //All 12am closes are 24
    const quint8 off_ShopClose = (ShopOffsetInfo.RequiresCustomCode == true) ? closeoffs[HourType] : 0x4; //0x4: close code is directly beside open
    const quint32 subASM = (ShopOffsetInfo.RequiresCustomCode == true) ? subvals[HourType] : 0xE2400000;

    if (HourType>EarlyBirdTime)
        return false;

    if (ClosingHour < Hours.OpenHour) { //Used if shop closes in early hours of morning, except 12am
        subcode = subASM | (ClosingHour&0xFF); //Make SUB instruction
        cmpcode = 0xE3500000 | ((Hours.OpenHour - ClosingHour)&0xFF); //Make CMP instruction
        checkcode = ReadCode(codebin, ShopBaseAddr.Offsets[m_GameType]+off_ShopOpen[HourType]+off_ShopClose+4); //Read existing code
        if (checkcode>>24 == 0x3A) //If instruction is a BCC
            checkcode = 0x2A000000 | (checkcode & 0x00FFFFFF); //BCC instruction --> BCS instruction
    }

    else {
        subcode = subASM | (Hours.OpenHour&0xFF); //Make SUB instruction
        cmpcode = 0xE3500000 | ((ClosingHour - Hours.OpenHour)&0xFF); //Make CMP instruction
        checkcode = ReadCode(codebin, ShopBaseAddr.Offsets[m_GameType]+off_ShopOpen[HourType]+off_ShopClose+4); //Read existing code
        //No need for BCS --> BCC here, game's orig code works fine! Having this implemented caused alot of breakage! :/
    }

    res |= !PatchCode(codebin, ShopBaseAddr.Offsets[m_GameType]+off_ShopOpen[HourType], subcode);
    res |= !PatchCode(codebin, ShopBaseAddr.Offsets[m_GameType]+off_ShopOpen[HourType]+off_ShopClose, cmpcode);
    res |= !PatchCode(codebin, ShopBaseAddr.Offsets[m_GameType]+off_ShopOpen[HourType]+off_ShopClose+4, checkcode);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::ApplyNormalShopTimes(File *codebin) {
    bool res = false;

    if (Retail.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Retail, Retail, g_NormalTimes.Retail, NormalTime);

    for (int i = 0; i < 5; i++) { /* For each nookling shop */
        if (Nooklings[i].HasNecessaryCode)
            res |= !MakeShopHourCode(codebin, Patch::Nooklings, Nooklings[i], g_NormalTimes.Nooklings[i], NormalTime);
    }

    if (Garden.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Garden, Garden, g_NormalTimes.Garden, NormalTime);

    if (Ables.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Ables, Ables, g_NormalTimes.Ables, NormalTime);

    if (Sham.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Sham, Sham, g_NormalTimes.Sham, NormalTime);

    if (Kicks.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Kicks, Kicks, g_NormalTimes.Kicks, NormalTime);

    if (Nooks.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Nooks, Nooks, g_NormalTimes.Nooks, NormalTime);

    if (Katrina.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Katrina, Katrina, g_NormalTimes.Katrina, NormalTime);

    if (Redd.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Redd, Redd, g_NormalTimes.Redd, NormalTime);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::ApplyEarlyShopTimes(File *codebin) {
    bool res = false;

    if (Retail.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Retail, Retail, g_EarlyBirdTimes.Retail, EarlyBirdTime);

    for (int i = 0; i < 5; i++) { /* For each nookling shop */
        if (Nooklings[i].HasNecessaryCode)
            res |= !MakeShopHourCode(codebin, Patch::Nooklings, Nooklings[i], g_EarlyBirdTimes.Nooklings[i], EarlyBirdTime);
    }

    if (Garden.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Garden, Garden, g_EarlyBirdTimes.Garden, EarlyBirdTime);

    if (Ables.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Ables, Ables, g_EarlyBirdTimes.Ables, EarlyBirdTime);

    if (Sham.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Sham, Sham, g_EarlyBirdTimes.Sham, EarlyBirdTime);

    if (Kicks.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Kicks, Kicks, g_EarlyBirdTimes.Kicks, EarlyBirdTime);

    if (Nooks.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Nooks, Nooks, g_EarlyBirdTimes.Nooks, EarlyBirdTime);

    if (Katrina.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Katrina, Katrina, g_EarlyBirdTimes.Katrina, EarlyBirdTime);

    if (Redd.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Redd, Redd, g_EarlyBirdTimes.Redd, EarlyBirdTime);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::ApplyNightShopTimes(File *codebin) {
    bool res = false;

    if (Retail.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Retail, Retail, g_NightOwlTimes.Retail, NightOwlTime);

    for (int i = 0; i < 5; i++) { /* For each nookling shop */
        if (Nooklings[i].HasNecessaryCode)
            res |= !MakeShopHourCode(codebin, Patch::Nooklings, Nooklings[i], g_NightOwlTimes.Nooklings[i], NightOwlTime);
    }

    if (Garden.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Garden, Garden, g_NightOwlTimes.Garden, NightOwlTime);

    if (Ables.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Ables, Ables, g_NightOwlTimes.Ables, NightOwlTime);

    if (Sham.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Sham, Sham, g_NightOwlTimes.Sham, NightOwlTime);

    if (Kicks.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Kicks, Kicks, g_NightOwlTimes.Kicks, NightOwlTime);

    if (Nooks.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Nooks, Nooks, g_NightOwlTimes.Nooks, NightOwlTime);

    if (Katrina.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Katrina, Katrina, g_NightOwlTimes.Katrina, NightOwlTime);

    if (Redd.HasNecessaryCode)
        res |= !MakeShopHourCode(codebin, Patch::Redd, Redd, g_NightOwlTimes.Redd, NightOwlTime);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}
