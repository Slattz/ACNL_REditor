#include "Headers/aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QPushButton>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
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
