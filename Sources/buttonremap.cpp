#include "Headers/buttonremap.h"
#include "ui_buttonremap.h"

ButtonRemap::ButtonRemap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ButtonRemap)
{
    ui->setupUi(this);
}

ButtonRemap::~ButtonRemap()
{
    delete ui;
}
