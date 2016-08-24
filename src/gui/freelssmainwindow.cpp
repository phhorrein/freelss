#include "Utils.h"
#include "Preset.h"
#include "Setup.h"
#include "SardauHardware.h"
#include "Laser.h"
#include "PresetManager.h"
#include "freelssmainwindow.h"
#include "ui_freelssmainwindow.h"

using namespace freelss;

FreeLSSMainWindow::FreeLSSMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FreeLSSMainWindow)
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
			this, SLOT(stabilityValueChanged(int)));

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
	ui->serialPortValue->addItem("/dev/ttyUSB0");
	ui->serialPortValue->addItem("/dev/ttyACM0");
	// BaudRate for SardauScan 
	// Nothing to do
	// Camera Selection for OpenCV Camera
	// Nothing to do
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



FreeLSSMainWindow::~FreeLSSMainWindow()
{
	delete ui;
}
