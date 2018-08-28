#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
