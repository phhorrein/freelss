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

#include "Main.h"
#include "SardauHardware.h"
#include "Thread.h"
#include "Setup.h"
#include "PresetManager.h"
#include "TimeoutSerial.h"
#include "boost/algorithm/string.hpp"

using namespace boost; 
using namespace std; 
namespace freelss
{

SardauHardware *SardauHardware::m_instance = NULL;

SardauHardware::SardauHardware() :
	m_portname("/dev/ttyUSB0"),
	m_port("/dev/ttyUSB0",38400)
{
	Setup * setup = Setup::get();
	m_responseDelay = setup->motorResponseDelay;
	//m_stepDelay = setup->motorStepDelay;
	//m_enablePin = setup->motorEnablePin;
	//m_stepPin = setup->motorStepPin;
	//m_directionPin = setup->motorDirPin;
	m_stepsPerRevolution = setup->stepsPerRevolution;
	m_stabilityDelay = PresetManager::get()->getActivePreset().stabilityDelay;
	m_port.setTimeout(posix_time::seconds(2));
	int responseDelay = setup->motorResponseDelay;
	// Wait 2 seconds for Arduino board to start
	// This should be replaced by a "wait for announce"
	Thread::usleep(2000000);

	// Initialize
	string welcome_cmd("sardauscan\r\n");
	string ret = sendCommand(welcome_cmd, 100);
	algorithm::to_lower(ret);
	if (ret == "yes") {
		cout << "Correct hardware detected" << endl;
	}
}

SardauHardware::~SardauHardware()
{
	m_port.close();
	Thread::usleep(m_responseDelay);
}

SardauHardware *SardauHardware::getInstance()
{
	if (SardauHardware::m_instance == NULL) {
		SardauHardware::m_instance = new SardauHardware();
	}
	return SardauHardware::m_instance;
}

void SardauHardware::initialize()
{
	// Nothing...
}

string SardauHardware::sendCommand(string command, int timeout)
{
	string ret;
	char dump[256];
	try
	{
		try {
			m_port.setTimeout(posix_time::milliseconds(500));
			while (1)
				m_port.read(dump,256);
		}
		catch (timeout_exception& e)
		{
			cout << "Received dump data (last bytes): " << dump << endl;
			m_port.setTimeout(posix_time::milliseconds(timeout));
			m_port.writeString(command);
			cout << "==> Sent command to controller: " << command << endl;
			ret = m_port.readStringUntil("\r\n");
			cout << "<== Received reply : " << ret << endl;
			return ret;
		}
	} 
	catch (timeout_exception& e)
	{
		cout << "Timeout !" << endl;
		return string("");
	}
	catch (system::system_error& e)
	{
		cout << "Error: " << e.what() << endl;
		return string("");
	}
	cout << "Unexpected end of function" << endl;
	return string("");
}


void SardauHardware::step()
{
	sendCommand("T R 001\r\n",300);
	//digitalWrite(m_stepPin, LOW);
	Thread::usleep(m_responseDelay);

	//digitalWrite(m_stepPin, HIGH);
	Thread::usleep(m_responseDelay);

	// Wait the step delay (this is how speed is controlled)
	Thread::usleep(m_stepDelay);
}

int SardauHardware::rotate(real theta)
{
	// Get the percent of a full revolution that theta is and convert that to number of steps
	int numSteps = (theta / (2 * PI)) * m_stepsPerRevolution;

	// Step the required number of steps
	for (int i = 0; i < numSteps; i++)
	{
		step();
	}

	// Sleep the stability delay amount
	Thread::usleep(m_stabilityDelay);

	return numSteps;
}

void SardauHardware::setMotorEnabled(bool enabled)
{
	//int value = enabled ? LOW : HIGH;

	//digitalWrite (m_enablePin, value);
	Thread::usleep(m_responseDelay);
}

void SardauHardware::turnOn(Laser::LaserSide laser)
{
	if (laser == Laser::RIGHT_LASER || laser == Laser::ALL_LASERS)
	{
//		digitalWrite (m_rightLaserPin, m_laserOnValue);
		sendCommand("L 0 1\r\n",500);
		m_rightLaserOn = true;
	}

	if (laser == Laser::LEFT_LASER || laser == Laser::ALL_LASERS)
	{
//		digitalWrite (m_leftLaserPin, m_laserOnValue);
		sendCommand("L 3 1\r\n",500);
		m_leftLaserOn = true;
	}
}

void SardauHardware::turnOff(Laser::LaserSide laser)
{
	if (laser == Laser::RIGHT_LASER || laser == Laser::ALL_LASERS)
	{
//		digitalWrite (m_rightLaserPin, m_laserOffValue);
		sendCommand("L 0 0\r\n",500);
		m_rightLaserOn = false;
	}

	if (laser == Laser::LEFT_LASER || laser == Laser::ALL_LASERS)
	{
//		digitalWrite (m_leftLaserPin, m_laserOffValue);
		sendCommand("L 3 0\r\n",500);
		m_leftLaserOn = false;
	}
}

bool SardauHardware::isOn(Laser::LaserSide laser)
{
	bool on = false;

	if (laser == Laser::ALL_LASERS)
	{
		on = m_rightLaserOn && m_leftLaserOn;
	}
	else if (laser == Laser::RIGHT_LASER)
	{
		on = m_rightLaserOn;
	}
	else if (laser == Laser::LEFT_LASER)
	{
		on = m_leftLaserOn;
	}

	return on;
}

}
