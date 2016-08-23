/*
 ****************************************************************************
 *  Copyright (c) 2014 Uriah Liggett <freelaserscanner@gmail.com>           *
 *	This file is part of FreeLSS.                                           *
 *                                                                          *
 *  FreeLSS is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  FreeLSS is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with FreeLSS.  If not, see <http://www.gnu.org/licenses/>.       *
 ****************************************************************************
*/

#pragma once

#include "TurnTable.h"
#include "Laser.h"
#include "TimeoutSerial.h"

namespace freelss
{

/**
 * An implementation of the TurnTable class that works
 * with the A4988 motor driver.
 */
class SardauHardware : public TurnTable, public Laser
{
public:
	SardauHardware();
	~SardauHardware();

	/** Rotates this amount in radians */
	int rotate(real theta);

	/** Enable/Disable the stepper motor */
	void setMotorEnabled(bool enabled);

	/** Initialize the turn table */
	void initialize();

	void turnOn(Laser::LaserSide laser);
	void turnOff(Laser::LaserSide laser);
	bool isOn(Laser::LaserSide laser);

	static SardauHardware *getInstance();

private:

	// Sardau
	std::string sendCommand(std::string command, int timeout);
	//TurnTable
	/** Move to the next step */
	void step();

	/** The time to sleep between steps in microseconds */
	int m_stepDelay;

	/** The time for the stepper driver to detect a voltage transition */
	int m_responseDelay;

	/** The enable pin */
	int m_enablePin;

	/** The step pin */
	int m_stepPin;

	/** The direction pin */
	int m_directionPin;

	/** The number of steps per revolution */
	int m_stepsPerRevolution;

	/** The time to sleep between steps in microseconds */
	int m_stabilityDelay;

	// Laser
	int m_rightLaserPin;
	int m_leftLaserPin;
	int m_laserOnValue;
	int m_laserOffValue;
	bool m_rightLaserOn;
	bool m_leftLaserOn;
	char m_wdata[256];
	char m_rdata[256];
	std::string m_portname;
	TimeoutSerial m_port;
	static SardauHardware *m_instance;
};

}
