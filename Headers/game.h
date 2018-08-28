#ifndef GAME_H
#define GAME_H

#include <QFile>
#include <QFileDialog>
#include "file.h"
#include "Headers/itemrandomiser.h"
#include "ui_mainwindow.h"
#include "Headers/shophourseditor.h"
#include "Headers/patch.h"

struct Dir_s {
    bool exist = false;
    QString dir = nullptr;
};

struct ACFile_s {
    Dir_s meta;
    File *file = nullptr;
};

enum GameType : int {
    NL_JPN = 0,
    NL_USA,
    NL_EUR,
    NL_KOR,
    WA_JPN,
    WA_USA,
    WA_EUR,
    WA_KOR
};

extern QString TIDs[8];

class Game
{
public:
    Game();
    ~Game();
    Game(const QString &dir);

    int DetermineGame(void);
    bool SetRomfsExists(const QString &dir);
    bool SetFilesExist(void);
    bool SetCodebinExists(const QString &dir);
    bool SetupOutDir(const QString &exedir, const QString &outdir);
    bool PatchCode(File *codebin, quint32 offset, quint32 code);
    quint32 ReadCode(File *codebin, quint32 offset);

    bool ApplyPatches(Ui::MainWindow *mainui, File *codebin);
    bool ApplyItemRandomiser(File *Itembin, QVector<ItemPrice_s *> ItemPrices);
    void ApplyCustomShopCode(File *codebin);
    bool ApplyAlwaysOpenPatches(File *codebin);
    bool MakeShopHourCode(File *codebin, Patch ShopBaseAddr, ShopCodeOffsets_s Shop, HoursInfo_s Hours, quint8 HourType);
    bool ApplyNormalShopTimes(File *codebin);
    bool ApplyEarlyShopTimes(File *codebin);
    bool ApplyNightShopTimes(File *codebin);

    File *codebin = nullptr;
    Dir_s m_romfs;
    Dir_s m_exefs;
    ACFile_s m_Itembin;
    GameType m_GameType;

private:
    bool WriteCustomShopCode(File *codebin, quint32 offset);

    QByteArray m_codemd5Hash;
};

#endif // GAME_H
