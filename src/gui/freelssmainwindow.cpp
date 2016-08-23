#include "freelssmainwindow.h"
#include "ui_freelssmainwindow.h"

FreeLSSMainWindow::FreeLSSMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FreeLSSMainWindow)
{
    ui->setupUi(this);
}

FreeLSSMainWindow::~FreeLSSMainWindow()
{
    delete ui;
}
