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
public slots:
	void laserSelectedChange(int index);
	void framesPerTurnChanged(int value);
	void laserThreshChanged(int value);
	void groundHeightChanged(int value);
	void stabilityDelayChanged(int value);
	void maxLaserWidthChanged(int value);
	void minLaserWidthChanged(int value);

private:
    Ui::FreeLSSMainWindow *ui;
};

#endif // FREELSSMAINWINDOW_H
