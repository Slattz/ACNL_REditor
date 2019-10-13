#include "Headers/game.h"
#include "Headers/mainwindow.h"
#include "Headers/itempricechanger.h"

/* Each entry relative to Game_s */
QString TIDs[8] = {"0004000000086200", "0004000000086300", "0004000000086400", "0004000000086500",  //Orig Game
                   "0004000000198d00", "0004000000198e00", "0004000000198f00", "0004000000199000"}; //WA Preinstalled Game

static const QByteArray CodebinMD5Hashes[8] = {"361749f1011b7a4deacc1cba3f1553fe", "32a2472809962d13cc49a4fc07ef4624",
                                 "4c487a97506dba4adb2fd4b439f4ed90", "fffffffffffffffffffffffffffffffe",
                                 "244f373ef2e8c37417534173f1c9c7e1", "0d5bebb090e4d7169c2f6be636b76291",
                                 "846d694886bcab0fdd43f62a02a0c24c", "ffffffffffffffffffffffffffffffff"};

static const quint32    PlayerSpeeds[5]   = {0x3F800000, 0x3fa00000, 0x3fc00000, 0x40000000, 0x40400000};
static const quint32    CameraZoomOutVals[6] = {0x3F800000, 0x3F700000, 0x3F600000, 0x3F500000, 0x3F400000, 0x3F300000};
static const quint8     ConfettiAmount[5] = {0x0F, 0x3C, 0x78, 0xB4, 0xF0};
static const quint8     CherryBAmount[5]  = {0x0F, 0x1E, 0x3C, 0x78, 0xF0};
static const quint8     SeasonVals[4]  = {0x3, 0x7, 0x12, 0x16};

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

QString Game::GetCurrentGame(void)
{
    QString Games[8] { "NL_JPN", "NL_USA", "NL_EUR", "NL_KOR",
                       "WA_JPN", "WA_USA", "WA_EUR", "WA_KOR" };

    if (m_codemd5Hash.isEmpty()) return "Error: Code.bin has no hash!";

    for (int i = 0; i < 8; i++) {
        if (m_codemd5Hash.toHex() == CodebinMD5Hashes[i]) {
            return Games[i];
        }
    }

    return "Reach End of GetCurrentGame";
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

    if(!codebin->Exists() || !offset || !code || offset-0x100000 >= codebin->m_file->size())
        return false;

    return codebin->Write(offset-0x100000, code, QDataStream::LittleEndian); //-0x100000 is actual code.bin file offset
}

quint32 Game::ReadCode(File *codebin, quint32 offset) {

    if(!codebin->Exists() || !offset || offset >= codebin->m_file->size())
        return false;

    return codebin->Read<quint32>(offset-0x100000, QDataStream::LittleEndian); //-0x100000 is actual code.bin file offset
}

bool Game::ApplyPatches(Ui::MainWindow *mainui, File *codebin) {
    bool res = false;
    /* Exefs->General */

    if(mainui->CB_CherryBTrees->isChecked()) {
        res |= CherryBTrees.Apply(codebin);
    }

    if(mainui->CB_XmasTrees->isChecked()) {
        res |= AlwaysXmasTrees.Apply(codebin);
    }

    if(mainui->CB_Island3Nums->isChecked()) {
        res |= Island3Nums.Apply(codebin);
    }

    if(mainui->CB_AnyTours->isChecked()) {
        res |= PickAllTours.Apply(codebin);
    }

    if(mainui->CB_MusicEcho->isChecked()) {
        res |= MusicHasEcho.Apply(codebin);
    }

    if(mainui->CB_VillagersNMove->isChecked()) {
        res |= VillagersNeverMove.Apply(codebin);
        res |= VillagersNeverMove2.Apply(codebin);
    }

    if(mainui->CB_GrassDecay->isChecked()) {
        res |= NoGrassDecay.Apply(codebin);
    }

    if (mainui->CB_FlowersNoWilt->checkState() == Qt::PartiallyChecked) {
        res |= OnlyBlackRoseWilts.Apply(codebin);
    }

    else if (mainui->CB_FlowersNoWilt->checkState() == Qt::Checked) {
        res |= FlowersNoWilt.Apply(codebin);
    }

    if(mainui->CB_Confetti->isChecked()) {
        res |= Confetti.Apply(codebin);
        if(mainui->CMB_Confetti->currentIndex() != 2) { //If not set at normal
            quint32 code = ReadCode(codebin, Confetti.m_Offset+0x30); //Read existing code
            code = (code & 0xFFFFFF00) | ConfettiAmount[mainui->CMB_Confetti->currentIndex()];
            res |= !PatchCode(codebin, Confetti.m_Offset+0x30, code);
        }
    }

    if(mainui->CB_CherryB->isChecked()) {
        res |= CherryBlossom.Apply(codebin);
        if(mainui->CMB_CherryB->currentIndex() != 2) { //If not set at normal
            quint32 code = ReadCode(codebin, CherryBlossom.m_Offset+0x68); //Read existing code
            code = (code & 0xFFFFFF00) | CherryBAmount[mainui->CMB_CherryB->currentIndex()];
            res |= !PatchCode(codebin, CherryBlossom.m_Offset+0x68, code);
        }
    }

    if(mainui->CMB_Season->currentIndex() != 4) { //If not set at normal
        res |= Season.Apply(codebin, SeasonVals[mainui->CMB_Season->currentIndex()]);
    }

    if(mainui->CMB_Weather->currentIndex() != 7) //If not set at no changes
        res |= Weather.Apply(codebin, static_cast<quint8>(mainui->CMB_Weather->currentIndex()));

    if(mainui->CMB_FishyBiteWhen->currentIndex() != 8) { //If not set at no changes
        FishyBiteWhen.m_Values[0].Value |= (mainui->CMB_FishyBiteWhen->currentIndex()&0xFF);
        res |= FishyBiteWhen.Apply(codebin);
    }

    if(mainui->CB_NoScareFishy->isChecked())
        res |= NoScareFishy.Apply(codebin);


    /* Exefs->Player */
    res |= !PatchCode(codebin, PlayerSpeed.m_Offset, PlayerSpeeds[mainui->dial_PlyrSpeed->value()]);

    if (mainui->CB_Resetti->isChecked())
        res |= NoResetti.Apply(codebin);

    if (mainui->CB_EditPatterns->isChecked())
        res |= EditPattern.Apply(codebin);

    if(mainui->CB_TrampleFlowers->isChecked())
        res |= FlowersNoTrample.Apply(codebin);

    if(mainui->CB_Mosquito->isChecked())
        res |= NoMosquito.Apply(codebin);

    if(mainui->CB_WalkOverThings->isChecked())
        res |= WalkOverThings.Apply(codebin);

    if (mainui->CB_QRUnlocked->isChecked())
        res |= QRMachineAlwaysUnlocked.Apply(codebin);

    /* Exefs->Shops */
    if(mainui->CB_NativeFruitSellPrice->isChecked())
        res |= NativeFruitPrice.Apply(codebin);

    if(mainui->CB_ReeseBuy->isChecked())
        res |= ReeseBuy.Apply(codebin);

    if(mainui->CB_NooklingBuy->isChecked())
        res |= NooklingsBuy.Apply(codebin);

    if(mainui->CB_LeilaBuy->isChecked())
        res |= LeilaBuy.Apply(codebin);

    /* Exefs->Utilities */
    if(mainui->CB_RegionPass->isChecked())
        res |= RegionCheck.Apply(codebin);

    if(mainui->CB_ChecksumPass->isChecked())
        res |= ChecksumCheck.Apply(codebin);

    if(mainui->CB_SecureValPass->isChecked())
        res |= SecureValueCheck.Apply(codebin);

    if(mainui->CB_InstantText->isChecked())
        res |= InstantText.Apply(codebin);

    if (mainui->CB_CMDKeyboard->isChecked()) {
        //Moves the game's functions down two instructions to make room for custom code
        if (Game::MoveExistingCode(codebin, CmdKeyboardHook.m_Offset, CmdKeyboardHook.m_Offset+0x3C, 2)) {
            qDebug() << "off_Rodata: 0x" + QString::number(UnusedRoData.m_Offset, 16);

            CmdKeyboard_Text2Item.m_Offset = UnusedCode.m_Offset;
            int size = CmdKeyboard_Text2Item.m_Values.size();
            CmdKeyboard_Text2Item.m_Values[size-3].Value = GetBasePlayerAddr.m_Offset; //Get_Base_Player_Address
            CmdKeyboard_Text2Item.m_Values[size-4].Value = wcstoulAddr.m_Offset; //wcstoul
            res |= CmdKeyboard_Text2Item.Apply(codebin);
            UnusedCode.m_Offset += static_cast<quint32>(size)*sizeof(CmdKeyboard_Text2Item.m_Values[0].Value);

            CmdKeyboard_ClearInv.m_Offset = UnusedCode.m_Offset;
            size = CmdKeyboard_ClearInv.m_Values.size();
            CmdKeyboard_ClearInv.m_Values[size-2].Value = GetBasePlayerAddr.m_Offset; //Get_Base_Player_Address
            res |= CmdKeyboard_ClearInv.Apply(codebin);
            UnusedCode.m_Offset += static_cast<quint32>(size)*sizeof(CmdKeyboard_ClearInv.m_Values[0].Value);

            CmdKeyboard_DupeAll.m_Offset = UnusedCode.m_Offset;
            size = CmdKeyboard_DupeAll.m_Values.size();
            CmdKeyboard_DupeAll.m_Values[size-1].Value = GetBasePlayerAddr.m_Offset; //Get_Base_Player_Address
            res |= CmdKeyboard_DupeAll.Apply(codebin);
            UnusedCode.m_Offset += static_cast<quint32>(size)*sizeof(CmdKeyboard_DupeAll.m_Values[0].Value);

            /*
            CmdKeyboard_ChangeTime.m_Offset = UnusedCode.m_Offset;
            size = CmdKeyboard_ChangeTime.m_Values.size();
            CmdKeyboard_ChangeTime.Apply(codebin);
            UnusedCode.m_Offset += static_cast<quint32>(size)*sizeof(CmdKeyboard_ChangeTime.m_Values[0].Value);
            */

            CmdKeyboardRoData.m_Offset = UnusedRoData.m_Offset;
            CmdKeyboardRodata* rodata = reinterpret_cast<CmdKeyboardRodata*>(CmdKeyboardRoData.m_Valuesu8.data());
            UpdateCmdKeyboardRodata(*rodata);
            res |= CmdKeyboardRoData.Apply(codebin);
            UnusedRoData.m_Offset += static_cast<quint32>(CmdKeyboardRoData.m_Values.size())*sizeof(CmdKeyboardRoData.m_Valuesu8[0]);

            CmdKeyboardFunction.m_Offset = UnusedCode.m_Offset;
            size = CmdKeyboardFunction.m_Values.size();
            CmdKeyboardFunction.m_Values[size-2].Value = CmdKeyboardRoData.m_Offset;
            res |= CmdKeyboardFunction.Apply(codebin);
            UnusedCode.m_Offset += static_cast<quint32>(CmdKeyboardFunction.m_Values.size())*sizeof(CmdKeyboardFunction.m_Values[0].Value);

            CmdKeyboardHook.m_Values[0].Value = MAKE_BRANCH_LINK(CmdKeyboardHook.m_Offset, CmdKeyboardFunction.m_Offset);
            res |= CmdKeyboardHook.Apply(codebin);
        }
    }

    res |= !PatchCode(codebin, CameraZoomOut.m_Offset, CameraZoomOutVals[mainui->dial_CameraZoomOut->value()]);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::ApplyItemRandomiser(File *Itembin) {
    if(!Itembin->Exists())
        return false;

    QDataStream fout(Itembin->m_file);
    fout.setByteOrder(QDataStream::LittleEndian);

    QVector<ItemPrice_s *> ItemPrices = ItemPriceChanger::GetPrices();

    if (ItemPrices.size() > 0x172B)
        ItemPrices.resize(0x172B);

    for (int i = 0; i < ItemPrices.size(); i++) { //Start at 1 to skip setting price for invalid items
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
    res |= !PatchCode(codebin, offset+8, MAKE_BRANCH_LINK(offset+8, NightOwl.m_Offset)); //BL: Sets up BL to IsNightOwlOrdinance
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
      if (off_Nooklings[i].RequiresCustomCode)
          off_Nooklings[i].HasNecessaryCode = WriteCustomShopCode(codebin, Nooklings.m_Offset+off_Nooklings[i].off_NormalOpen-0x14);
    }

    if (off_Garden.RequiresCustomCode)
        off_Garden.HasNecessaryCode = WriteCustomShopCode(codebin, Garden.m_Offset+off_Garden.off_NormalOpen-0x14);

    if (off_Ables.RequiresCustomCode)
        off_Ables.HasNecessaryCode = WriteCustomShopCode(codebin, Ables.m_Offset+off_Ables.off_NormalOpen-0x14);

    if (off_Sham.RequiresCustomCode)
        off_Sham.HasNecessaryCode = WriteCustomShopCode(codebin, Sham.m_Offset+off_Sham.off_NormalOpen-0x14);

    if (off_Kicks.RequiresCustomCode)
        off_Kicks.HasNecessaryCode = WriteCustomShopCode(codebin, Kicks.m_Offset+off_Kicks.off_NormalOpen-0x14);

    if (off_Nooks.RequiresCustomCode)
        off_Nooks.HasNecessaryCode = WriteCustomShopCode(codebin, Nooks.m_Offset+off_Nooks.off_NormalOpen-0x14);

    if (off_Katrina.RequiresCustomCode)
        off_Katrina.HasNecessaryCode = WriteCustomShopCode(codebin, Katrina.m_Offset+off_Katrina.off_NormalOpen-0x14);
}

bool Game::ApplyAlwaysOpenPatches(File *codebin) {
    bool res = false;

    if(g_AlwaysOpen.Retail)
        res |= !PatchCode(codebin, Retail.m_Offset+off_Retail.off_AlwaysOpen, SHOPALWAYSOPEN);

    for (int i = 0; i < 5; i++) { /* For each nookling shop */
        static const quint8  off_BLEarly[5] = {0x3C, 0x54, 0x3C, 0x54, 0x3C};
        quint32 off_AlwaysOpen = Nooklings.m_Offset+off_Nooklings[i].off_NormalOpen-off_BLEarly[i];

        if(g_AlwaysOpen.Nooklings[i])
            res |= !PatchCode(codebin, off_AlwaysOpen, MAKE_BRANCH(off_AlwaysOpen, Nooklings.m_Offset+off_Nooklings[i].off_AlwaysOpen));
    }

    if(g_AlwaysOpen.Garden)
        res |= !PatchCode(codebin, Garden.m_Offset+off_Garden.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Ables)
        res |= !PatchCode(codebin, Ables.m_Offset+off_Ables.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Sham)
        res |= !PatchCode(codebin, Sham.m_Offset+off_Sham.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Kicks)
        res |= !PatchCode(codebin, Kicks.m_Offset+off_Kicks.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Nooks)
        res |= !PatchCode(codebin, Nooks.m_Offset+off_Nooks.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Katrina)
        res |= !PatchCode(codebin, Katrina.m_Offset+off_Katrina.off_AlwaysOpen, SHOPALWAYSOPEN);

    if(g_AlwaysOpen.Redd)
        res |= !PatchCode(codebin, Redd.m_Offset+off_Redd.off_AlwaysOpen, SHOPALWAYSOPEN);

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
        checkcode = ReadCode(codebin, ShopBaseAddr.m_Offset+off_ShopOpen[HourType]+off_ShopClose+4); //Read existing code
        if (checkcode>>24 == 0x3A) //If instruction is a BCC
            checkcode = 0x2A000000 | (checkcode & 0x00FFFFFF); //BCC instruction --> BCS instruction
    }

    else {
        subcode = subASM | (Hours.OpenHour&0xFF); //Make SUB instruction
        cmpcode = 0xE3500000 | ((ClosingHour - Hours.OpenHour)&0xFF); //Make CMP instruction
        checkcode = ReadCode(codebin, ShopBaseAddr.m_Offset+off_ShopOpen[HourType]+off_ShopClose+4); //Read existing code
        //No need for BCS --> BCC here, game's orig code works fine! Having this implemented caused alot of breakage! :/
    }

    res |= !PatchCode(codebin, ShopBaseAddr.m_Offset+off_ShopOpen[HourType], subcode);
    res |= !PatchCode(codebin, ShopBaseAddr.m_Offset+off_ShopOpen[HourType]+off_ShopClose, cmpcode);
    res |= !PatchCode(codebin, ShopBaseAddr.m_Offset+off_ShopOpen[HourType]+off_ShopClose+4, checkcode);

    if (res == true)
        return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::ApplyShopTimes(File *codebin) {
    bool res = false;

    if (off_Retail.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Retail, off_Retail, g_NormalTimes.Retail, NormalTime);
        res |= !MakeShopHourCode(codebin, Retail, off_Retail, g_EarlyBirdTimes.Retail, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Retail, off_Retail, g_NightOwlTimes.Retail, NightOwlTime);
    }

    for (int i = 0; i < 5; i++) { /* For each nookling shop */
        if (off_Nooklings[i].HasNecessaryCode) {
            res |= !MakeShopHourCode(codebin, Nooklings, off_Nooklings[i], g_NormalTimes.Nooklings[i], NormalTime);
            res |= !MakeShopHourCode(codebin, Nooklings, off_Nooklings[i], g_EarlyBirdTimes.Nooklings[i], EarlyBirdTime);
            res |= !MakeShopHourCode(codebin, Nooklings, off_Nooklings[i], g_NightOwlTimes.Nooklings[i], NightOwlTime);
        }
    }

    if (off_Garden.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Garden, off_Garden, g_NormalTimes.Garden, NormalTime);
        res |= !MakeShopHourCode(codebin, Garden, off_Garden, g_EarlyBirdTimes.Garden, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Garden, off_Garden, g_NightOwlTimes.Garden, NightOwlTime);
    }

    if (off_Ables.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Ables, off_Ables, g_NormalTimes.Ables, NormalTime);
        res |= !MakeShopHourCode(codebin, Ables, off_Ables, g_EarlyBirdTimes.Ables, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Ables, off_Ables, g_NightOwlTimes.Ables, NightOwlTime);
    }

    if (off_Sham.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Sham, off_Sham, g_NormalTimes.Sham, NormalTime);
        res |= !MakeShopHourCode(codebin, Sham, off_Sham, g_EarlyBirdTimes.Sham, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Sham, off_Sham, g_NightOwlTimes.Sham, NightOwlTime);
    }

    if (off_Kicks.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Kicks, off_Kicks, g_NormalTimes.Kicks, NormalTime);
        res |= !MakeShopHourCode(codebin, Kicks, off_Kicks, g_EarlyBirdTimes.Kicks, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Kicks, off_Kicks, g_NightOwlTimes.Kicks, NightOwlTime);
    }

    if (off_Nooks.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Nooks, off_Nooks, g_NormalTimes.Nooks, NormalTime);
        res |= !MakeShopHourCode(codebin, Nooks, off_Nooks, g_EarlyBirdTimes.Nooks, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Nooks, off_Nooks, g_NightOwlTimes.Nooks, NightOwlTime);
    }

    if (off_Katrina.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Katrina, off_Katrina, g_NormalTimes.Katrina, NormalTime);
        res |= !MakeShopHourCode(codebin, Katrina, off_Katrina, g_EarlyBirdTimes.Katrina, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Katrina, off_Katrina, g_NightOwlTimes.Katrina, NightOwlTime);
    }

    if (off_Redd.HasNecessaryCode) {
        res |= !MakeShopHourCode(codebin, Redd, off_Redd, g_NormalTimes.Redd, NormalTime);
        res |= !MakeShopHourCode(codebin, Redd, off_Redd, g_EarlyBirdTimes.Redd, EarlyBirdTime);
        res |= !MakeShopHourCode(codebin, Redd, off_Redd, g_NightOwlTimes.Redd, NightOwlTime);
    }

    if (res == true) return false; //At least one PatchCode Failed

    return true; //No PatchCode Failed
}

bool Game::MoveExistingCode(File *codebin, quint32 startOffset, quint32 endOffset, qint32 moveByAmount, bool thumbCode) {
    if (codebin == nullptr || startOffset >= endOffset || moveByAmount == 0) return false;
    quint8 instructSize = 2*(!thumbCode ? 2 : 1);
    quint32 size = endOffset-startOffset;
    if (size == 0) return false;

    for (quint32 i = 0; i <= size; i+=instructSize) {
        quint32 curOffset = 0;
        if (moveByAmount < 0) curOffset = startOffset+i; //Code Moved Backwards
        else if (moveByAmount > 0) curOffset = endOffset-i; //Code Moved Forwards
        else return false;

        quint32 origCode = ReadCode(codebin, curOffset); //Read existing code
        quint32 newOffset = curOffset+static_cast<quint32>(moveByAmount*instructSize);

        if (((origCode>>24)&0x0F) == 0xB || ((origCode>>24)&0x0F) == 0xA) //if BL # or B # instruction
            origCode+=static_cast<quint32>(moveByAmount*-1); //*-1 swaps positive <-> negative

        else if (((origCode>>24)&0x0F) == 0x5 && ((origCode>>16)&0x0F) == 0xF) { //if LDR RX, [PC, #X]
            origCode+=static_cast<quint32>((moveByAmount*instructSize)*-1); //*-1 swaps positive <-> negative
        }

        PatchCode(codebin, newOffset, origCode);
    }

    for (qint32 i = 0; i < moveByAmount; i++) { //NOP the now old code, just so it doesn't mess with anything
        if (moveByAmount < 0) { //Code Moved Backwards
            PatchCode(codebin, endOffset-static_cast<quint32>(i*4), 0xE1A00000);
        }

        else if (moveByAmount > 0) { //Code Moved Forwards
            PatchCode(codebin, startOffset+static_cast<quint32>(i*4), 0xE1A00000);
        }
    }

    return true;
}

void Game::UpdateCmdKeyboardRodata(CmdKeyboardRodata& roData) {
    static const wchar_t* func_cmds[CMD_AMOUNT] = {L"t2i ", L"clearinv", L"dupeall", L"", L"", L"",
                                                   L"", L"", L"", L"", L"", L""};

    roData.func_ptrs[0]  = CmdKeyboard_Text2Item.m_Offset; //Text2item (/t2i ID)
    roData.func_ptrs[1]  = CmdKeyboard_ClearInv.m_Offset; //ClearInv (/clearinv)
    roData.func_ptrs[2]  = CmdKeyboard_DupeAll.m_Offset; //DupeAll (/dupeall)
    /* Don't feel like implementing these */
    roData.func_ptrs[3]  = 0xFFFFFFFF; //CmdKeyboard_ChangeTime.m_Offset;
    roData.func_ptrs[4]  = 0xFFFFFFFF; //RTWE (/rtwe ID)
    roData.func_ptrs[5]  = 0xFFFFFFFF; //Search & Replace (/sr SEARCH_ID REPLACE_ID)
    roData.func_ptrs[6]  = 0xFFFFFFFF; //Faint (/faint)
    roData.func_ptrs[7]  = 0xFFFFFFFF; //Player Name Changer (/pname NAME)
    roData.func_ptrs[8]  = 0xFFFFFFFF; //Town Name Changer (/tname NAME)
    roData.func_ptrs[9]  = 0xFFFFFFFF; //?
    roData.func_ptrs[10] = 0xFFFFFFFF; //?
    roData.func_ptrs[11] = 0xFFFFFFFF; //?

    roData.func_cmd_size[0]  = 5; //t2i ID
    roData.func_cmd_size[1]  = 8; //clearinv
    roData.func_cmd_size[2]  = 7; //dupeinv
    roData.func_cmd_size[3]  = -1; //time
    roData.func_cmd_size[4]  = -1;
    roData.func_cmd_size[5]  = -1; //e.g: sr 1 2
    roData.func_cmd_size[6]  = -1;
    roData.func_cmd_size[7]  = -1;
    roData.func_cmd_size[8]  = -1;
    roData.func_cmd_size[9]  = -1;
    roData.func_cmd_size[10] = -1;
    roData.func_cmd_size[11] = -1;

    wchar_t* cmd_str_ptr = roData.func_cmd_strs;
    quint32 baseOffset = CmdKeyboardRoData.m_Offset+(4*CMD_AMOUNT)+(1*CMD_AMOUNT)+(4*CMD_AMOUNT);

    //qDebug() << "baseOffset: 0x" + QString::number(baseOffset, 16);
    for (int i = 0; i < CMD_AMOUNT; i++) {
        size_t size = wcslen(func_cmds[i]);

        wcsncpy(cmd_str_ptr, func_cmds[i], size&0xFF);
        roData.func_cmds[i] = baseOffset;

        cmd_str_ptr+=(size+1); //+1 for null terminator
        baseOffset+=(size+1)*sizeof(wchar_t);
    }
}

