#include "Headers/patch.h"
#include "Headers/mainwindow.h"
#include "Headers/game.h"

ShopCodeOffsets_s off_Retail = {0xA0,0x90,0x60,0xAC,false, true};
ShopCodeOffsets_s off_Nooklings[5] = {{0x84,0x88,0x5C,0x26C,true, false},{0x10C,0xFC,0xCC,0x26C,false, true},{0x168,0x16C,0x140,0x26C,true, false},{0x1F0,0x1E0,0x1B0,0x26C,false, true},{0x24C,0x250,0x224,0x26C,true, false}};
ShopCodeOffsets_s off_Garden = {0x88,0x8C,0x60,0xB4,true, false};
ShopCodeOffsets_s off_Ables = {0x5C,0x60,0x34,0x7C,true, false};
ShopCodeOffsets_s off_Sham = {0x68,0x6C,0x34,0x48,true, false};
ShopCodeOffsets_s off_Kicks = {0x5C,0x60,0x34,0x9C,true, false};
ShopCodeOffsets_s off_Nooks = {0xA8,0xAC,0x80,0xD0,true, false};
ShopCodeOffsets_s off_Katrina = {0x8C,0x90,0x64,0xAC,true, false};
ShopCodeOffsets_s off_Redd = {0xD0,0xC0,0x90,0xDC,false, true};

static const QVector<PatchValues> NOPVec({{NOP, 0}});
static const QVector<PatchValues> FFVec({{0xFFFFFFFF, 0}});

static const QVector<quint8> NoResettiPattern = {
0x57, 0x0C, 0x80, 0xE2, 0x0A, 0x10, 0xD0, 0xE5,
0x02, 0x10, 0x81, 0xE3, 0x0A, 0x10, 0xC0, 0xE5
};

static const QVector<quint8> ConfettiPattern = {
0x16, 0x5B, 0x84, 0xE2, 0xAC, 0x50, 0x85, 0xE2,
0x00, 0x00, 0x95, 0xE5, 0x00, 0x00, 0x50, 0xE3
};

static const QVector<quint8> CherryBPattern = {
0x10, 0xD0, 0x4D, 0xE2, 0x02, 0x00, 0xA0, 0xE3,
0x16, 0x6B, 0x84, 0xE2, 0xAC, 0x60, 0x86, 0xE2
};

static const QVector<quint8> CherryBTreePattern = {
0x00, 0x80, 0xA0, 0xE1, 0x20, 0x10, 0xA0, 0xE3,
0x50, 0x00, 0x8D, 0xE2, 0xF8, 0x04, 0xCD, 0xE1
};

static const QVector<quint8> XmasTreePattern = { //Thumb
0xF8, 0xB5, 0x7B, 0x21,
0x14, 0x46, 0x06, 0x9E
};

static const QVector<quint8> Island3NumsPattern = { //ARM
0x56, 0x10, 0xA0, 0xE3, 0x4C, 0x40, 0x86, 0xE2,
0x00, 0x10, 0x80, 0xE5, 0x11, 0x1E, 0xA0, 0xE3
};

static const QVector<quint8> AllToursPattern = {
0xF0, 0x4F, 0x2D, 0xE9, 0x00, 0x50, 0xA0, 0xE1,
0x00, 0xB0, 0xA0, 0xE3, 0x02, 0x8B, 0x2D, 0xED
};

static const QVector<quint8> MusicEchoPattern = {
0x6C, 0x02, 0x95, 0xE5, 0x06, 0x00, 0x50, 0xE1,
0x01, 0x00, 0x76, 0x13
};

static const QVector<quint8> SaveChecksPattern = {
0x08, 0xD0, 0x4D, 0xE2, 0x0B, 0xB0, 0xA0, 0xE3,
0x05, 0x80, 0xA0, 0xE3, 0x0C, 0x60, 0xA0, 0xE3
};

static const QVector<quint8> RegionChecksPattern = {
0x62, 0x5A, 0x84, 0xE2, 0x4E, 0x01, 0xD5, 0xE5,
0x0F, 0x00, 0x00, 0xE2
};

/*
 * Pattern for 'MoveVillagerOutOfTown' function;
 * This is called by only 'CheckVillagersThatNeedToMoveOut' in code.bin which we also patch
 * But just incase a CRO uses this function or something, give it the same patch
 */
static const QVector<quint8> VillagersNeverMovePattern = { //Thumb
0x06, 0x98, 0x05, 0x99, 0x0A, 0x28
};

//Pattern for 'CheckVillagersThatNeedToMoveOut' function
static const QVector<quint8> VillagersNeverMovePattern2 = { //Thumb
0x2F, 0x18, 0x03, 0x20,
0x38, 0x56, 0x00, 0x28
};


/*
 * Custom Function to replace the 'MoveVillagerOutOfTown' function
 * This function removes the boxed status from all villagers
*/
static const QVector<quint8> VillagersNeverMoveFunction = {
0x01, 0xA1, 0x08, 0x47, // BX to our actual function; Needed to go from Thumb -> ARM
0x00, 0x1C, 0x70, 0x47, //Padding (MOV R0, R0 and BX LR in Thumb just incase somehow used)
0x00, 0xC0, 0xE0, 0xE3, 0x30, 0x20, 0x9F, 0xE5,
0x25, 0x3C, 0x80, 0xE2, 0x2C, 0x10, 0x9F, 0xE5,
0x02, 0xC0, 0xC0, 0xE7, 0x10, 0x30, 0x83, 0xE2,
0x66, 0x0B, 0x80, 0xE2, 0x00, 0x20, 0x93, 0xE5,
0x01, 0x20, 0x02, 0xE0, 0x00, 0x20, 0x83, 0xE5,
0x25, 0x3C, 0x83, 0xE2, 0x18, 0x30, 0x83, 0xE2,
0x03, 0x00, 0x50, 0xE1, 0xF8, 0xFF, 0xFF, 0x1A,
0x1E, 0xFF, 0x2F, 0xE1, //End of Function Code
0x63, 0x97, 0x01, 0x00, 0xFC, 0xF9, 0xFF, 0xFF //Const values
};

//Check if button is newly pressed
static const QVector<quint8> hidKeysDownPattern = { // -0x30 for function start
0x00, 0x00, 0xA0, 0x93, 0x04, 0x00, 0x90, 0xE5,
0x04, 0x00, 0x10, 0xE0, 0x01, 0x00, 0xA0, 0x13
};

//Check if button is being held
static const QVector<quint8> hidKeysHeldPattern = { // -0x38 for function start
0x0C, 0x00, 0x90, 0xE5, 0x01, 0x00, 0x80, 0xE1,
0x04, 0x00, 0x10, 0xE0, 0x01, 0x00, 0xA0, 0x13
};

//Check if button is newly released
static const QVector<quint8> hidKeysUpPattern = { // -0x30 for function start
0x00, 0x00, 0xA0, 0x93, 0x08, 0x00, 0x90, 0xE5,
0x04, 0x00, 0x10, 0xE0, 0x01, 0x00, 0xA0, 0x13
};

static const QVector<quint8> BTN_SaveMenuPattern = { //+4
0x24, 0x00, 0x9F, 0xE5, 0x40, 0x10, 0xA0, 0xE3,
0x00, 0x00, 0x90, 0xE5
};

static const QVector<quint8> BTN_ScreenshotPattern = { //+0x10 for L, +0x24 for R
0x00, 0x04, 0x94, 0xE5, 0x00, 0x00, 0x50, 0xE3,
0x01, 0x00, 0x40, 0xC2, 0x00, 0x00, 0x81, 0xC5
};

/*
RunB: +0x94; RunL: +0xAC; RunR: +0xC0; Look Up +0x154; ItemSwitchR: +0x1D4
ItemSwitchL: +0x208; Unequip: +0x240; Interact: +0x11C; Pickup: +0xE4
*/
static const QVector<quint8> BTN_General = { //0x68D140 in EUR orig 1.5
0x11, 0x00, 0xD5, 0xE5, 0x11, 0x00, 0xC4, 0xE5,
0x12, 0x00, 0xD5, 0xE5, 0x12, 0x00, 0xC4, 0xE5,
0x13, 0x00, 0xD5, 0xE5, 0x13, 0x00, 0xC4, 0xE5,
0x14, 0xD0, 0x8D, 0xE2, 0x00, 0x00, 0xA0, 0xE3
};

/* Exefs->General */

Patch NativeFruitPrice;
Patch ReeseBuy;
Patch NooklingsBuy;
Patch LeilaBuy;
Patch NoResetti; //Pattern: 57 0C 80 E2 0A 10 D0 E5  02 10 81 E3 0A 10 C0 E5 + 0x8
Patch Confetti; //Pattern: 16 5B 84 E2 AC 50 85 E2  00 00 95 E5 00 00 50 E3 + 0x8
Patch CherryBlossom; //Pattern: 10 D0 4D E2 02 00 A0 E3  16 6B 84 E2 AC 60 86 E2 + 0x8
Patch Weather; //Pattern: 1st find of [06 10 41 E2 00 00 51 E3  18 10 81 B2 34 00 50 E3] + 0x88
Patch Season; //Pattern: 1st find of [00 00 A0 E3 0C 34 83 E1 00 10 A0 E1 73 30 FF E6] + 0x18
Patch CherryBTrees; //Pattern: 00 80 A0 E1  20 10 A0 E3 50 00 8D E2 + 0x0
Patch AlwaysXmasTrees; //Thumb; Pattern: F8 B5 7B 21 14 46 06 9E + 0xA
Patch Island3Nums; //Pattern: 56 10 A0 E3 4C 40 86 E2 00 10 80 E5 11 1E A0 E3 - 0x10
Patch PickAllTours; //Pattern: F0 4F 2D E9 00 50 A0 E1 00 B0 A0 E3  02 8B 2D ED + 0x8
Patch MusicHasEcho; //Pattern:
Patch VillagersNeverMove; //Thumb; Pattern: 06 98 05 99 0A 28
Patch VillagersNeverMove2; //Thumb; Pattern: 2F 18 03 20 38 56 00 28
//Patch NoGrassDecay(JPN, USA, EUR, KOR, JPN, USA, EUR, WAKOR, 0xE3A0000FF);

/* Exefs->Player */
Patch PlayerSpeed;
Patch EditPattern;
Patch FlowersNoTrample;
Patch NoMosquito; //Pattern: 10 40 BD E8 03 20 A0 E1 A6 10 A0 E3

/* Exefs->Utilities */
Patch RegionCheck; //Pattern: 62 5A 84 E2 4E 01 D5 E5 0F 00 00 E2 - 0xC
Patch ChecksumCheck; //Pattern: 08 D0 4D E2 0B B0 A0 E3 05 80 A0 E3 0C 60 A0 E3 + 0xB4
Patch SecureValueCheck; //Pattern: 08 D0 4D E2 0B B0 A0 E3 05 80 A0 E3 0C 60 A0 E3 + 0xE0

/* Shop Times: Addresses are start of function */
Patch Retail;
Patch Nooklings; //Pattern: 00 10 A0 E1  14 00 D4 E5 04 40 91 E5
Patch Garden;
Patch Ables;
Patch Sham;
Patch Kicks;
Patch Nooks;
Patch Katrina;
Patch Redd;
Patch NightOwl;

/* Button Remapper */
Patch Button_RunB;
Patch Button_RunL;
Patch Button_RunR;
Patch Button_Unequip;
Patch Button_ItemSwitchDL;
Patch Button_ItemSwitchDR;
Patch Button_LookUp;
Patch Button_Pickup;
Patch Button_Interact;
Patch Button_ScreenshotL;
Patch Button_ScreenshotR;
Patch Button_SaveMenu;

/* Button Remapper Utils */
Patch hidKeysDown;
Patch hidKeysHeld;
Patch hidKeysUp;


Patch::Patch() { }

Patch::Patch(quint32 NL_JPN, quint32 NL_USA, quint32 NL_EUR, quint32 NL_KOR,
      quint32 WA_JPN, quint32 WA_USA, quint32 WA_EUR, quint32 WA_KOR,
      QVector<PatchValues> Values)
{
    quint32 Offsets[8] = {NL_JPN, NL_USA, NL_EUR, NL_KOR, WA_JPN, WA_USA, WA_EUR, WA_KOR};
    Game* game = MainWindow::GetGameInstance();
    if (game == nullptr || game->m_GameType > 8)
        m_Offset = 0;

    else {
        m_Offset = Offsets[game->m_GameType];
        m_Values = Values;
    }
}

Patch::Patch(const QVector<quint8> Pattern, QVector<PatchValues> Values, quint32 OffsetFromPattern) {
    Game* game = MainWindow::GetGameInstance();
    game->codebin->m_file->seek(0);
    QByteArray data = game->codebin->m_file->readAll();

    //Nanquitas search function is 2x faster than std::search in this instance
    quint32 offset = Search<quint8>(data.constData(), data.size(), Pattern);
    if (offset != 0) {
        m_Offset = 0x100000+offset+OffsetFromPattern;
        m_Values = Values;
    }

    else m_Offset = 0;
    qDebug() << "Pattern found offset: 0x" + QString::number(m_Offset, 16);
}

//Returns false if no patches fail; true if one or more patches fail
bool Patch::Apply(File* code, quint8 OR) {
    bool res = false;
    if (m_Offset == 0) return true;
    if (m_Values.empty()) return true;

    for (auto patch : this->m_Values) {
        res |= !Game::PatchCode(code, m_Offset + patch.OffsetFromBaseAddr, patch.Value|OR);
    }

    return res;
}

QVector<PatchValues> Patch::MakeARMPatchValuesFromFunctionBytes(QVector<quint8> function) {
    static const quint8 codesize = 4;
    QVector<PatchValues> vec;

    quint32 size = static_cast<quint32>(function.size());
    if ((size % codesize) != 0) { //Check if function bytes has fully completed instructions
        qDebug() << "Function bytes are missing, not full function!";
        return vec;
    }

    quint32 i = 0;
    while (i < size) {
        quint32 instruction = 0;
        for (quint8 j = 0; j < codesize; j++) {
            quint8 byte = function[i+j];
            instruction |= byte<<(8*j);
        }
        vec.push_back({instruction, i});
        i+=4;
    }

    return vec;
}

void Patch::Init(void) {
    NativeFruitPrice =  Patch(0x30576C, 0x3056AC, 0x3056B8, KOR, 0x30576C, 0x305714, 0x3056B8, WAKOR, NOPVec);
    ReeseBuy =          Patch(0x768EE0, 0x76A740, 0x769748, KOR, 0x768EB8, 0x769724, 0x769720, WAKOR, NOPVec);
    NooklingsBuy =      Patch(0x769148, 0x76A9A8, 0x7699B0, KOR, 0x769120, 0x76998C, 0x769988, WAKOR, QVector<PatchValues>({{0xE1A041A0, 0}, {NOP, 8}}));
    LeilaBuy =          Patch(0x768884, 0x76A0E4, 0x7690EC, KOR, 0x76885C, 0x7690C8, 0x7690C4, WAKOR, QVector<PatchValues>({{0xE1A00004,0}}));
    NoResetti =         Patch(NoResettiPattern, QVector<PatchValues>({{0xE3C11002, 0}}), 8);
    Confetti =          Patch(ConfettiPattern, QVector<PatchValues>({{0xE3A00001, 0}, {0xE3A00078, 0x30}}), 8);
    CherryBlossom =     Patch(CherryBPattern, QVector<PatchValues>({{0xE3A00001, 0}, {0xE3A00001, 0x28}, {0xE3A00004, 0x50}, {0xE3A01001, 0x60}}), 8);
    Weather =           Patch(0x62E728, 0x62FC30, 0x62EC68, KOR, 0x62E728, 0x62F158, 0x62EC68, WAKOR, QVector<PatchValues>({{0xE3A00000,0}})); //Pattern: 1st find of [06 10 41 E2 00 00 51 E3  18 10 81 B2 34 00 50 E3] + 0x88
    Season =            Patch(0x56B090, 0x56C758, 0x56B7A0, KOR, 0x56B090, 0x56BC70, 0x56B7A0, WAKOR, QVector<PatchValues>({{0xE3A01000,0}})); //Pattern: 1st find of [00 00 A0 E3 0C 34 83 E1 00 10 A0 E1 73 30 FF E6] + 0x18
    CherryBTrees =      Patch(CherryBTreePattern, QVector<PatchValues>({{0xE3A08005,0}, {0xE28F2074, 0x150}}), 0);
    AlwaysXmasTrees =   Patch(XmasTreePattern, QVector<PatchValues>({{0x1C002000, 0}}), 0xA);
    Island3Nums =       Patch(Island3NumsPattern, NOPVec, static_cast<quint32>(-0x10));
    PickAllTours =      Patch(AllToursPattern, QVector<PatchValues>({{0xE3E0B000, 0}}), 8);
    MusicHasEcho =      Patch(MusicEchoPattern, QVector<PatchValues>({{0xE1A00000,0}, {0xE3A01003, 4}}), 0xC);
    VillagersNeverMove= Patch(VillagersNeverMovePattern, MakeARMPatchValuesFromFunctionBytes(VillagersNeverMoveFunction), static_cast<quint32>(-4));
    VillagersNeverMove2=Patch(VillagersNeverMovePattern2, MakeARMPatchValuesFromFunctionBytes(VillagersNeverMoveFunction), static_cast<quint32>(-0xA));
    //static const Patch  NoGrassDecay(JPN, USA, EUR, KOR, JPN, USA, EUR, WAKOR, 0xE3A0000FF);

    /* Exefs->Player */
    PlayerSpeed =       Patch(0x651708, 0x652C10, 0x651C48, KOR, 0x651708, 0x652138, 0x651C48, WAKOR, QVector<PatchValues>({{0x3F800000, 0}}));
    EditPattern =       Patch(0x2FEC78, 0x2FEC44, 0x2FECCC, KOR, 0x2FEC78, 0x2FE9C0, 0x2FECCC, WAKOR, QVector<PatchValues>({{0xE3A00001, 0}}));
    FlowersNoTrample =  Patch(0x596890, 0x597F58, 0x596FA0, KOR, 0x596890, 0x597470, 0x596FA0, WAKOR, QVector<PatchValues>({{0xE3A0001D, 0}}));
    NoMosquito =        Patch(0x5C245C, 0x5C3B24, 0x5C2B6C, KOR, 0x5C245C, 0x5C3054, 0x5C2B6C, WAKOR, QVector<PatchValues>({{0xE3A01006,0}})); //Pattern: 10 40 BD E8 03 20 A0 E1 A6 10 A0 E3

    /* Exefs->Utilities */
    RegionCheck =       Patch(RegionChecksPattern, QVector<PatchValues>({{0xE1A00005,0}}), static_cast<quint32>(-0xC));
    ChecksumCheck =     Patch(SaveChecksPattern, QVector<PatchValues>({{0xE3A00001,0}, {0xE3A00001,0x1C}}), 0xB4);
    SecureValueCheck =  Patch(SaveChecksPattern, QVector<PatchValues>({{0xE3A00001, 0}}), 0xE0);

    /* Shop Times: Addresses are start of function */
    Retail =            Patch(0x309310, 0x30929C, 0x309298, KOR, 0x309310, 0x309384, 0x309298, WAKOR, FFVec);
    Nooklings =         Patch(0x710054, 0x7118A0, 0x7108A8, KOR, 0x71002C, 0x710D50, 0x710880, WAKOR, FFVec); //Pattern: 00 10 A0 E1  14 00 D4 E5 04 40 91 E5
    Garden =            Patch(0x7102D0, 0x711B1C, 0x710B24, KOR, 0x7102A8, 0x710FCC, 0x710AFC, WAKOR, FFVec);
    Ables =             Patch(0x7125E8, 0x713E34, 0x712E3C, KOR, 0x7125C0, 0x7132E4, 0x712E14, WAKOR, FFVec);
    Sham =              Patch(0x71BB98, 0x71D3E4, 0x71C3EC, KOR, 0x71BB70, 0x71C72C, 0x71C3C4, WAKOR, FFVec);
    Kicks =             Patch(0x70FF64, 0x7117B0, 0x7107B8, KOR, 0x70FF3C, 0x710C60, 0x710790, WAKOR, FFVec);
    Nooks =             Patch(0x71DD38, 0x71F584, 0x71E58C, KOR, 0x71DD10, 0x71E8CC, 0x71E564, WAKOR, FFVec);
    Katrina =           Patch(0x7167A0, 0x717FEC, 0x716FF4, KOR, 0x716778, 0x71749C, 0x716FCC, WAKOR, FFVec);
    Redd =              Patch(0x716B1C, 0x718368, 0x717370, KOR, 0x716AF4, 0x717818, 0x717348, WAKOR, FFVec);
    NightOwl =          Patch(0x7585C4, 0x759E24, 0x758E2C, KOR, 0x75859C, 0x758E08, 0x758E04, WAKOR, FFVec);

    /* Button Remap */
    Button_RunB =       Patch(BTN_General, QVector<PatchValues>({{0xE3A00002, 0}}), 0x94);
    Button_RunL =       Patch(BTN_General, QVector<PatchValues>({{0xE3A00A02, 0}}), 0xAC);
    Button_RunR =       Patch(BTN_General, QVector<PatchValues>({{0xE3A00901, 0}}), 0xC0);
    Button_Unequip =    Patch(BTN_General, QVector<PatchValues>({{0xE3A00802, 0}}), 0x240);
    Button_ItemSwitchDL=Patch(BTN_General, QVector<PatchValues>({{0xE3A00701, 0}}), 0x208);
    Button_ItemSwitchDR=Patch(BTN_General, QVector<PatchValues>({{0xE3A00702, 0}}), 0x1D4);
    Button_LookUp =     Patch(BTN_General, QVector<PatchValues>({{0xE3A00801, 0}}), 0x154);
    Button_Pickup =     Patch(BTN_General, QVector<PatchValues>({{0xE3A00010, 0}}), 0xE4);
    Button_Interact =   Patch(BTN_General, QVector<PatchValues>({{0xE3A00001, 0}}), 0x11C);

    Button_ScreenshotL= Patch(BTN_ScreenshotPattern, QVector<PatchValues>({{0xE3A00A02, 0}}), 0x10);
    Button_ScreenshotR= Patch(BTN_ScreenshotPattern, QVector<PatchValues>({{0xE3A00901, 0}}), 0x24);
    Button_SaveMenu =   Patch(BTN_SaveMenuPattern, QVector<PatchValues>({{0xE1A00000, 0}, {0xE3A00B02, 4}, {0xEB0E02A8, 8}}), 4);

    /* Button Remapper Utils */
    hidKeysDown =       Patch(hidKeysDownPattern, QVector<PatchValues>({{0xE92D4010,0}}), static_cast<quint32>(-0x30));
    hidKeysHeld =       Patch(hidKeysHeldPattern, QVector<PatchValues>({{0xE92D4010,0}}), static_cast<quint32>(-0x38));
    hidKeysUp =         Patch(hidKeysUpPattern, QVector<PatchValues>({{0xE92D4010,0}}), static_cast<quint32>(-0x30));

}
