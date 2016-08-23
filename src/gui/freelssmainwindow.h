#ifndef FREELSSMAINWINDOW_H
#define FREELSSMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class FreeLSSMainWindow;
}

class FreeLSSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FreeLSSMainWindow(QWidget *parent = 0);
    ~FreeLSSMainWindow();

private:
    Ui::FreeLSSMainWindow *ui;
};

#endif // FREELSSMAINWINDOW_H
