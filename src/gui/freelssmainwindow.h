#ifndef FREELSSMAINWINDOW_H
#define FREELSSMAINWINDOW_H

#include "Image.h"
#include <QMainWindow>
#include <QTimer>

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
	void updateSerialPorts();
	void serialPortChanged(const QString &text);
	void connectToScanner();
	void displayImage(freelss::Image &image);

private:
    Ui::FreeLSSMainWindow *ui;
	 QTimer enumerateTimer;
};

#endif // FREELSSMAINWINDOW_H
