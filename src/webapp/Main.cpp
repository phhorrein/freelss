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

#include "Utils.h"
#include "SardauHardware.h"
#include "Camera.h"
#include "Scanner.h"
#include "A4988TurnTable.h"
#include "RelayLaser.h"
#include "PresetManager.h"
#include "HttpServer.h"
#include "PresetManager.h"
#include "PropertyReaderWriter.h"
#include "Setup.h"
#include "UpdateManager.h"
#include "Lighting.h"
#include "WifiConfig.h"
#include <curl/curl.h>
#include <algorithm>

/** Initializes and destroys libcurl */
struct InitCurl
{
	InitCurl()
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	~InitCurl()
	{
		curl_global_cleanup();
	}
};

/** Initializes and destroys the singletons */
struct InitSingletons
{
	InitSingletons()
	{
		freelss::PresetManager::get();
		freelss::UpdateManager::get();
		freelss::Setup::get();
#ifdef LIGHTING_IMPLEMENTED
		freelss::Lighting::get();
#endif
		freelss::WifiConfig::get();
		freelss::HttpServer::get();
	}

	~InitSingletons()
	{
		freelss::HttpServer::release();
		freelss::Laser::release();
		freelss::Camera::release();
		freelss::TurnTable::release();
		freelss::UpdateManager::release();
		freelss::PresetManager::release();
#ifdef LIGHTING_IMPLEMENTED
		freelss::Lighting::release();
#endif
		freelss::WifiConfig::release();
		freelss::Setup::release();
	}
};


int main(int argc, char **argv)
{
	int retVal = 0;

	pid_t pid = fork();
	if (pid < 0)
	{
		std::cerr << "Error forking process!!!" << std::endl;
		return 1;
	}
	else if (pid != 0)
	{
		return 0;
	}

	// Initialize the Raspberry Pi hardware
	//InitBcmHost bcmHost;

	try
	{
		// Setup wiring pi
//		wiringPiSetup();

		// Initialize Curl
		InitCurl curl();

		// Load the properties
		freelss::LoadProperties();

		// Initialize the singletons
		InitSingletons singletons;

		int port = freelss::Setup::get()->httpPort;

		std::cout << "Running on port " << port << "..." << std::endl;
		freelss::HttpServer::get()->start(port);

		while (true)
		{
			freelss::Thread::usleep(500000);  // Sleep 500ms
		}
	}
	catch (freelss::Exception& ex)
	{
		std::cerr << "Exception: " << ex << std::endl;
		retVal = -1;
	}
	catch (std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
		retVal = -1;
	}
	catch (...)
	{
		std::cerr << "Unknown Exception Occurred" << std::endl;
		retVal = -1;
	}

	return retVal;
}
