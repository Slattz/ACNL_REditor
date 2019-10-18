#ifndef PATCH_H
#define PATCH_H

#include <QtGlobal>
#include <QCheckBox>
#include <QComboBox>
#include <QVector>
#include <cstring>
#include "file.h"

#define KOR 0
#define WAKOR 0

#define NOP 0xE1A00000
#define SHOPALWAYSOPEN 0xE3A00001

#define CMD_AMOUNT 12 //For Command Keyboard Rodata struct

/* Obtained from patcher.h in Luma3DS: https://github.com/AuroraWright/Luma3DS/blob/master/sysmodules/loader/source/patcher.h */
#define MAKE_BRANCH(src,dst)      (0xEA000000 | (static_cast<quint32>(((reinterpret_cast<quint8 *>(dst) - reinterpret_cast<quint8 *>(src)) >> 2) - 2) & 0xFFFFFF))
#define MAKE_BRANCH_LINK(src,dst) (0xEB000000 | (static_cast<quint32>(((reinterpret_cast<quint8 *>(dst) - reinterpret_cast<quint8 *>(src)) >> 2) - 2) & 0xFFFFFF))

struct ShopCodeOffsets_s {
    quint16 off_NormalOpen;
    quint16 off_NightOpen;
    quint16 off_EarlyOpen;
    quint16 off_AlwaysOpen;
    bool   RequiresCustomCode;
    bool   HasNecessaryCode;
};

enum ShopTime : quint8 {
    NormalTime = 0,
    NightOwlTime = 1,
    EarlyBirdTime = 2
};

struct PatchValues {
    quint32 Value;
    quint32 OffsetFromBaseAddr;
};

struct CmdKeyboardRodata {
    quint32 func_ptrs[CMD_AMOUNT];
    qint8 func_cmd_size[CMD_AMOUNT];
    quint32 func_cmds[CMD_AMOUNT];
    wchar_t func_cmd_strs[256];
};

extern ShopCodeOffsets_s off_Retail;
extern ShopCodeOffsets_s off_Nooklings[5];
extern ShopCodeOffsets_s off_Garden;
extern ShopCodeOffsets_s off_Ables;
extern ShopCodeOffsets_s off_Sham;
extern ShopCodeOffsets_s off_Kicks;
extern ShopCodeOffsets_s off_Nooks;
extern ShopCodeOffsets_s off_Katrina;
extern ShopCodeOffsets_s off_Redd;

//Search function from Nanquitas's CTRPF Utils.h
template <typename T>
static quint32     Search(const char* start, const int size, const QVector<T> &pattern)
{
    if (!start || !size || pattern.empty())
        return (0);

    const quint32   patternSize = pattern.size() * sizeof(T);
    const quint8    *patternc = reinterpret_cast<const quint8 *>(pattern.data());
    const quint8    *startPos = reinterpret_cast<const quint8 *>(start);

    quint32     table[256];

    for (quint32 i = 0; i < 256; i++)
        table[i] = patternSize;

    for (quint32 i = 0; i < patternSize - 1; i++)
        table[patternc[i]] = patternSize - i - 1;

    quint32 j = 0;
    while (j <= size - patternSize)
    {
        const quint8 c = startPos[j + patternSize - 1];
        if (patternc[patternSize - 1] == c && std::memcmp(patternc, startPos + j, patternSize - 1) == 0)
            return j;
        j += table[c];
    }
    return (0);
}

class Patch
{
public:

    Patch();

    Patch(quint32 NL_JPN, quint32 NL_USA, quint32 NL_EUR, quint32 NL_KOR,
          quint32 WA_JPN, quint32 WA_USA, quint32 WA_EUR, quint32 WA_KOR,
          QVector<PatchValues> Values);

    Patch(const QVector<quint8> Pattern, QVector<PatchValues> Values, quint32 OffsetFromPattern = 0);
    Patch(const QVector<quint8> Pattern, QVector<quint8> Values, quint32 OffsetFromPattern = 0);

    bool Apply(File* codebin, quint8 OR = 0);

    static void Init(void);
    static QVector<PatchValues> MakeARMPatchValuesFromFunctionBytes(QVector<quint8> function);

    quint32 m_Offset = 0; //Offsets are code.bin + 0x100000, i.e. same offsets as hardware
    QVector<PatchValues> m_Values;
    QVector<quint8> m_Valuesu8;

};

/* General Utils */
extern Patch UnusedCode;
extern Patch UnusedRoData;
extern Patch GetBasePlayerAddr;
extern Patch wcstoulAddr;

/* Button Remapper Utils */
extern Patch hidKeysDown;
extern Patch hidKeysHeld;
extern Patch hidKeysUp;

/* Command Keyboard Utils */
extern Patch CmdKeyboardFunction;
extern Patch CmdKeyboardRoData;

extern Patch CmdKeyboard_Text2Item;
extern Patch CmdKeyboard_ChangeTime;
extern Patch CmdKeyboard_DupeAll;
extern Patch CmdKeyboard_ClearInv;

/* Exefs->General */
extern Patch Confetti;
extern Patch CherryBlossom;
extern Patch Weather;
extern Patch Season;
extern Patch CherryBTrees;
extern Patch AlwaysXmasTrees;
extern Patch Island3Nums;
extern Patch PickAllTours;
extern Patch MusicHasEcho;
extern Patch VillagersNeverMove;
extern Patch VillagersNeverMove2;
extern Patch NoGrassDecay;
extern Patch FlowersNoWilt;
extern Patch OnlyBlackRoseWilts;
extern Patch FishyBiteWhen;
extern Patch NoScareFishy;

/* Exefs->Player */
extern Patch NoResetti1;
extern Patch NoResetti2;
extern Patch PlayerSpeed;
extern Patch EditPattern;
extern Patch FlowersNoTrample;
extern Patch NoMosquito;
extern Patch WalkOverThings;
extern Patch QRMachineAlwaysUnlocked;
extern Patch PermitAlways100_1;
extern Patch PermitAlways100_2;

/* Exefs->Utilities */
extern Patch RegionCheck;
extern Patch ChecksumCheck;
extern Patch SecureValueCheck;
extern Patch InstantText;
extern Patch CameraZoomOut;
extern Patch CmdKeyboardHook;

/* Exefs->Shops */
extern Patch NativeFruitPrice;
extern Patch ReeseBuy;
extern Patch NooklingsBuy;
extern Patch LeilaBuy;
extern Patch InstantCyrusCustoms;

/* Shop Times: Addresses are start of function */
extern Patch Retail;
extern Patch Nooklings;
extern Patch Garden;
extern Patch Ables;
extern Patch Sham;
extern Patch Kicks;
extern Patch Nooks;
extern Patch Katrina;
extern Patch Redd;
extern Patch NightOwl;

/* Button Remapper */
extern Patch Button_RunB;
extern Patch Button_RunL;
extern Patch Button_RunR;
extern Patch Button_Unequip;
extern Patch Button_ItemSwitchDL;
extern Patch Button_ItemSwitchDR;
extern Patch Button_LookUp;
extern Patch Button_Pickup;
extern Patch Button_Interact;
extern Patch Button_ScreenshotL;
extern Patch Button_ScreenshotR;
extern Patch Button_SaveMenu;

#endif // PATCH_H
