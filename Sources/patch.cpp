#include "Headers/patch.h"

ShopCodeOffsets_s Retail = {0xA0,0x90,0x60,0xAC,false, true};
ShopCodeOffsets_s Nooklings[5] = {{0x84,0x88,0x5C,0x274,true, false},{0x10C,0xFC,0xCC,0x274,false, true},{0x168,0x16C,0x140,0x274,true, false},{0x1F0,0x1E0,0x1B0,0x274,false, true},{0x24C,0x250,0x224,0x274,true, false}};
ShopCodeOffsets_s Garden = {0x88,0x8C,0x60,0xB4,true, false};
ShopCodeOffsets_s Ables = {0x5C,0x60,0x34,0x7C,true, false};
ShopCodeOffsets_s Sham = {0x68,0x6C,0x34,0x48,true, false};
ShopCodeOffsets_s Kicks = {0x5C,0x60,0x34,0x9C,true, false};
ShopCodeOffsets_s Nooks = {0xA8,0xAC,0x80,0xD0,true, false};
ShopCodeOffsets_s Katrina = {0x8C,0x90,0x64,0xAC,true, false};
ShopCodeOffsets_s Redd = {0xD0,0xC0,0x90,0xDC,false, true};

//JPN, USA, EUR, KOR, WA_JPN, WA_USA, WA_EUR, WA_KOR, Value
/* Exefs->General */

const Patch  Patch::NativeFruitPrice(0x30576C, 0x3056AC, 0x3056B8, KOR, 0x30576C, 0x305714, 0x3056B8, WAKOR, NOP);
const Patch  Patch::ReeseBuy(0x768EE0, 0x76A740, 0x769748, KOR, 0x768EB8, 0x769724, 0x769720, WAKOR, NOP);
const Patch  Patch::NooklingsBuy(0x769148, 0x76A9A8, 0x7699B0, KOR, 0x769120, 0x76998C, 0x769988, WAKOR, 0xE1A041A0, NOP);
const Patch  Patch::LeilaBuy(0x768884, 0x76A0E4, 0x7690EC, KOR, 0x76885C, 0x7690C8, 0x7690C4, WAKOR, 0xE1A00004);
const Patch  Patch::NoResetti(0x5CC4A0, 0x5CDB68, 0x5CCBB0, KOR, 0x5CC4A0, 0x5CD098, 0x5CCBB0, WAKOR, 0xE3C11002); //Pattern: 57 0C 80 E2 0A 10 D0 E5  02 10 81 E3 0A 10 C0 E5
const Patch  Patch::Confetti(0x266BF8, 0x266CDC, 0x266CD8, KOR, 0x266BF8, 0x266720, 0x266CD8, WAKOR, 0xE3A00001, 0xE3A00078); //Pattern: 16 5B 84 E2 AC 50 85 E2  00 00 95 E5 00 00 50 E3
const Patch  Patch::CherryBlossom(0x296960, 0x296A64, 0x296A60, KOR, 0x296960, 0x2964A8, 0x296A60, WAKOR, 0xE3A00001, 0xE3A00004, 0xE3A01001); //Pattern: 10 D0 4D E2 02 00 A0 E3  16 6B 84 E2 AC 60 86 E2
const Patch  Patch::Weather(0x62E728, 0x62FC30, 0x62EC68, KOR, 0x62E728, 0x62F158, 0x62EC68, WAKOR, 0xE3A00000); //Pattern: 06 10 41 E2 00 00 51 E3  18 10 81 B2 34 00 50 E3 + 0x88
const Patch  Patch::Season(0x56B090, 0x56C758, 0x56B7A0, KOR, 0x56B090, 0x56BC70, 0x56B7A0, WAKOR, 0xE3A01000); //Pattern: 00 00 A0 E3 0C 34 83 E1 00 10 A0 E1 73 30 FF E6 + 0x18
const Patch  Patch::CherryBTrees(0x599524, 0x59ABEC, 0x599C34, KOR, 0x599524, 0x59A104, 0x599C34, WAKOR, 0xE3A08005, 0xE28F2074); //Pattern: 00 80 A0 E1  20 10 A0 E3 50 00 8D E2 + 0x0
//const Patch  Patch::NoGrassDecay(JPN, USA, EUR, KOR, JPN, USA, EUR, WAKOR, 0xE3A0000FF);

/* Exefs->Player */
const Patch  Patch::PlayerSpeed(0x651708, 0x652C10, 0x651C48, KOR, 0x651708, 0x652138, 0x651C48, WAKOR, 0x3F800000);
const Patch  Patch::EditPattern(0x2FEC78, 0x2FEC44, 0x2FECCC, KOR, 0x2FEC78, 0x2FE9C0, 0x2FECCC, WAKOR, 0xE3A00001);
const Patch  Patch::FlowersNoTrample(0x596890, 0x597F58, 0x596FA0, KOR, 0x596890, 0x597470, 0x596FA0, WAKOR, 0xE3A0001D);
const Patch  Patch::NoMosquito(0x5C245C, 0x5C3B24, 0x5C2B6C, KOR, 0x5C245C, 0x5C3054, 0x5C2B6C, WAKOR, 0xE3A01006); //Pattern: 10 40 BD E8 03 20 A0 E1 A6 10 A0 E3

/* Exefs->Utilities */
const Patch  Patch::RegionCheck(0x7577C4, 0x759024, 0x75802C, KOR, 0x75779C, 0x758008, 0x758004, WAKOR, 0xE1A00005);
const Patch  Patch::ChecksumCheck(0x1D4300, 0x1D43A4, 0x1D43C4, KOR, 0x1D4300, 0x1D3DE8, 0x1D43C4, WAKOR, 0xE3A00001);

/* Shop Times: Addresses are start of function */
const Patch  Patch::Retail(   0x309310, 0x30929C, 0x309298, KOR, 0x309310, 0x309384, 0x309298, WAKOR, 0xFFFFFFFF);
const Patch  Patch::Nooklings(0x710054, 0x7118A0, 0x7108A8, KOR, 0x71002C, 0x710D50, 0x710880, WAKOR, 0xFFFFFFFF); //Pattern: 00 10 A0 E1  14 00 D4 E5 04 40 91 E5
const Patch  Patch::Garden(   0x7102D0, 0x711B1C, 0x710B24, KOR, 0x7102A8, 0x710FCC, 0x710AFC, WAKOR, 0xFFFFFFFF);
const Patch  Patch::Ables(    0x7125E8, 0x713E34, 0x712E3C, KOR, 0x7125C0, 0x7132E4, 0x712E14, WAKOR, 0xFFFFFFFF);
const Patch  Patch::Sham(     0x71BB98, 0x71D3E4, 0x71C3EC, KOR, 0x71BB70, 0x71C72C, 0x71C3C4, WAKOR, 0xFFFFFFFF);
const Patch  Patch::Kicks(    0x70FF64, 0x7117B0, 0x7107B8, KOR, 0x70FF3C, 0x710C60, 0x710790, WAKOR, 0xFFFFFFFF);
const Patch  Patch::Nooks(    0x71DD38, 0x71F584, 0x71E58C, KOR, 0x71DD10, 0x71E8CC, 0x71E564, WAKOR, 0xFFFFFFFF);
const Patch  Patch::Katrina(  0x7167A0, 0x717FEC, 0x716FF4, KOR, 0x716778, 0x71749C, 0x716FCC, WAKOR, 0xFFFFFFFF);
const Patch  Patch::Redd(     0x716B1C, 0x718368, 0x717370, KOR, 0x716AF4, 0x717818, 0x717348, WAKOR, 0xFFFFFFFF);
const Patch  Patch::NightOwl( 0x7585C4, 0x759E24, 0x758E2C, KOR, 0x75859C, 0x758E08, 0x758E04, WAKOR, 0xFFFFFFFF);

Patch::Patch(quint32 NL_JPN, quint32 NL_USA, quint32 NL_EUR, quint32 NL_KOR,
             quint32 WA_JPN, quint32 WA_USA, quint32 WA_EUR, quint32 WA_KOR, quint32 Value1, quint32 Value2, quint32 Value3, quint32 Value4, quint32 Value5) :
    Offsets{NL_JPN, NL_USA, NL_EUR, NL_KOR, WA_JPN, WA_USA, WA_EUR, WA_KOR}, Value{Value1, Value2, Value3, Value4, Value5}
{
}
