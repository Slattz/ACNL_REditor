#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QPushButton>

AboutWindow::AboutWindow(QString exedir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    QPixmap pix(exedir + "/resources/icon.bmp");
    ui->icon->setPixmap(pix);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_buttonBox_clicked(QAbstractButton *button)
{
    if(reinterpret_cast<QPushButton *>(button) == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        this->close();
    }

}
