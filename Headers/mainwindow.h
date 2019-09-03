#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Headers/game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static Game* GetGameInstance(void);

private slots:
    void on_FileOpen_triggered();
    void on_FileSave_triggered();
    void on_FileClose_triggered();

    void on_dial_PlyrSpeed_valueChanged(int value);
    void on_BTN_ShopTimes_clicked();
    void on_actionAbout_triggered();
    void on_BTN_Remapper_clicked();
    void on_BTN_ItemPriceChanger_clicked();

    /* Debug Stuff */
    void on_actionEnableAll_triggered();
    void on_actionListItemPrices_triggered();
    void on_actionSetup_OutDir_triggered();
    void on_actionQuick_Open_EUR_triggered();

private:
    Ui::MainWindow *ui = nullptr;
    QString dir = "\0";
    Game *game = nullptr;
    QString exedir;
    QString outdir;

    void OpenGame(const QString setupdir);
};

extern MainWindow* MainWindowInstance;

#endif // MAINWINDOW_H
