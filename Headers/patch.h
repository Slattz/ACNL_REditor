#ifndef PATCH_H
#define PATCH_H

#include <QtGlobal>
#include <QVector>

#define KOR 0
#define WAKOR 0

#define NOP 0xE1A00000
#define SHOPALWAYSOPEN 0xE3A00001

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

extern ShopCodeOffsets_s Retail;
extern ShopCodeOffsets_s Nooklings[5];
extern ShopCodeOffsets_s Garden;
extern ShopCodeOffsets_s Ables;
extern ShopCodeOffsets_s Sham;
extern ShopCodeOffsets_s Kicks;
extern ShopCodeOffsets_s Nooks;
extern ShopCodeOffsets_s Katrina;
extern ShopCodeOffsets_s Redd;

struct Patch
{
    Patch(quint32 NL_JPN, quint32 NL_USA, quint32 NL_EUR, quint32 NL_KOR,
          quint32 WA_JPN, quint32 WA_USA, quint32 WA_EUR, quint32 WA_KOR, quint32 Value1, quint32 Value2=0, quint32 Value3=0,
          quint32 Value4=0, quint32 Value5=0);

    struct {
        quint32 Offsets[8]; //Offsets are code.bin + 0x100000, therefore same offsets as code.bin in ram on the 3ds
        quint32 Value[5];
    } ;

    /* Exefs->General */
    static const Patch  NativeFruitPrice;
    static const Patch  ReeseBuy;
    static const Patch  NooklingsBuy;
    static const Patch  LeilaBuy;
    static const Patch  NoResetti;
    static const Patch  Confetti;
    static const Patch  CherryBlossom;
    static const Patch  Weather;
    static const Patch  Season;
    static const Patch  CherryBTrees;
    static const Patch  AlwaysXmasTrees;
    static const Patch  NoGrassDecay;

    /* Exefs->Player */
    static const Patch  PlayerSpeed;
    static const Patch  EditPattern;
    static const Patch  FlowersNoTrample;
    static const Patch  NoMosquito;

    /* Exefs->Utilities */
    static const Patch  RegionCheck;
    static const Patch  ChecksumCheck;

    /* Shop Times: Addresses are start of function */
    static const Patch  Retail;
    static const Patch  Nooklings;
    static const Patch  Garden;
    static const Patch  Ables;
    static const Patch  Sham;
    static const Patch  Kicks;
    static const Patch  Nooks;
    static const Patch  Katrina;
    static const Patch  Redd;
    static const Patch  NightOwl;

};

#endif // PATCH_H
