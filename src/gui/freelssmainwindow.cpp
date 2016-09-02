#include "Utils.h"
#include "Preset.h"
#include "Setup.h"
#include "SardauHardware.h"
#include "Laser.h"
#include "PresetManager.h"
#include "freelssmainwindow.h"
#include "ui_freelssmainwindow.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QPixmap>
#include <QImage>

using namespace freelss;

FreeLSSMainWindow::FreeLSSMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FreeLSSMainWindow),
	enumerateTimer(this)
{

	LoadProperties();
	PresetManager *presetMgr = PresetManager::get();
	Preset& preset = presetMgr->getActivePreset();
	ui->setupUi(this);

	////////// Settings //////////////
	// Laser Selection
	for (int i = 0; i < (int)Laser::NUM_LASERSIDE; i++) {
		ui->laserSelectBox->addItem(Laser::toString((Laser::LaserSide)i).c_str());
	}
	ui->laserSelectBox->setCurrentIndex(preset.laserSide);

	connect (ui->laserSelectBox, SIGNAL(currentIndexChanged(int)), 
			this, SLOT(laserSelectedChange(int)));

	//Frames per Revolution
	ui->framesPerTurnValue->setRange(0,16383);
	ui->framesPerTurnValue->setValue(preset.framesPerRevolution);
	connect (ui->framesPerTurnValue, SIGNAL(valueChanged(int)), 
			this, SLOT(framesPerTurnChanged(int)));

	//Laser detection threshold
	ui->laserThreshValue->setRange(0,255);
	ui->laserThreshValue->setValue(preset.laserThreshold);
	connect (ui->laserThreshValue, SIGNAL(valueChanged(int)), 
			this, SLOT(laserThreshChanged(int)));
	

	//Ground Height
	ui->groundHeightValue->setMinimum(-10.0);
	ui->groundHeightValue->setMaximum(10.0);
	ui->groundHeightValue->setSingleStep(0.1);
	ui->groundHeightValue->setPageStep(5.0);
	ui->groundHeightValue->setTickPosition(QSlider::TicksBothSides);
	ui->groundHeightValue->setValue(preset.groundPlaneHeight);
	connect (ui->groundHeightValue, SIGNAL(valueChanged(int)), 
			this, SLOT(groundHeightChanged(int)));

	//Stability delay
	ui->stabilityDelayValue->setRange(0,500);
	ui->stabilityDelayValue->setValue(preset.stabilityDelay);
	connect (ui->stabilityDelayValue, SIGNAL(valueChanged(int)), 
			this, SLOT(stabilityDelayChanged(int)));

	//Max Laser Width
	ui->maxLaserWidthValue->setRange(1,255);
	ui->maxLaserWidthValue->setValue(preset.maxLaserWidth);
	connect (ui->maxLaserWidthValue, SIGNAL(valueChanged(int)), 
			this, SLOT(maxLaserWidthChanged(int)));

	//Min Laser Width
	ui->minLaserWidthValue->setRange(1,255);
	ui->minLaserWidthValue->setValue(preset.minLaserWidth);
	connect (ui->minLaserWidthValue, SIGNAL(valueChanged(int)), 
			this, SLOT(minLaserWidthChanged(int)));


	// Serial Port for SardauScan
	//Get available serial port. This should be repeated periodically...
	connect(&enumerateTimer, SIGNAL(timeout()),
			this,SLOT(updateSerialPorts()));
	enumerateTimer.setInterval(1000);
	updateSerialPorts();
	ui->serialPortValue->setCurrentText(QString::fromStdString(preset.sardauSerialPort));

	connect (ui->serialPortValue, SIGNAL(currentTextChanged(const QString &)), 
			this, SLOT(serialPortChanged(const QString &)));
	// BaudRate for SardauScan 
#if 0
	QList<qint32>::iterator i;
	for (i = QSerialPortInfo::standardBaudRates().begin(); i != QSerialPortInfo::standardBaudRates().end();
			++i) {
		ui->baudRateValue->addItem(QString::number((int)*i));
	}
#endif

	if (Camera::getCameraType() == Camera::CT_OPENCV) {
	// Camera Selection for OpenCV Camera
	}

	
	////////// CamView //////////////
	Camera *camera = Camera::getInstance();
	if (camera == NULL) {
		std::cout << "No Camera found!" << std::endl;
	}
	Image *img = camera->acquireImage();
	QImage camImage(img->getPixels(),img->getWidth(),img->getHeight(),QImage::Format_RGB888);
	ui->camView->setPixmap(QPixmap::fromImage(camImage));
}

void FreeLSSMainWindow::laserSelectedChange(int index) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.laserSide = (Laser::LaserSide)index;
	SaveProperties();
}

void FreeLSSMainWindow::framesPerTurnChanged(int value) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.framesPerRevolution = value;
	SaveProperties();
}

void FreeLSSMainWindow::laserThreshChanged(int value) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.framesPerRevolution = value;
	SaveProperties();
}

void FreeLSSMainWindow::groundHeightChanged(int value) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.groundPlaneHeight = value;
	SaveProperties();
}

void FreeLSSMainWindow::stabilityDelayChanged(int value) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.stabilityDelay = value;
	SaveProperties();
}

void FreeLSSMainWindow::maxLaserWidthChanged(int value) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.maxLaserWidth = value;
	SaveProperties();
}

void FreeLSSMainWindow::minLaserWidthChanged(int value) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.minLaserWidth = value;
	SaveProperties();
}


void FreeLSSMainWindow::updateSerialPorts() {
	const auto infos = QSerialPortInfo::availablePorts();
	ui->serialPortValue->clear();
	for (const QSerialPortInfo &info : infos) {
			ui->serialPortValue->addItem(info.systemLocation());
	}
	enumerateTimer.start();
}	

void FreeLSSMainWindow::serialPortChanged(const QString &text) {
	Preset& preset = PresetManager::get()->getActivePreset();
	preset.sardauSerialPort = text.toStdString();
	SaveProperties();
}


FreeLSSMainWindow::~FreeLSSMainWindow()
{
	delete ui;
}
