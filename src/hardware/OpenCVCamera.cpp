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

#include <chrono>
#include "Main.h"
#include "OpenCVCamera.h"
#include "Thread.h"
#include "opencv2/opencv.hpp"

using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using namespace std::chrono;
using namespace cv;
namespace freelss
{

OpenCVCamera::OpenCVCamera(int devid) :
	m_devid(devid),
	m_camera(devid)
{
	try {
		if (!m_camera.isOpened()) {
			throw Exception ("Failed to open OpenCV Camera");
		}
		m_imageHeight = m_camera.get(CV_CAP_PROP_FRAME_HEIGHT);
		m_imageWidth = m_camera.get(CV_CAP_PROP_FRAME_WIDTH);
		m_frameRate = m_camera.get(CV_CAP_PROP_FPS);
//m_camera.set(CV_CAP_PROP_CONVERT_RGB, true);

		std::cout << "Target Image Width: " << m_imageWidth << std::endl;
		std::cout << "Target Image Height: " << m_imageHeight << std::endl;

		std::cout << "Initialized camera" << std::endl;
	}
	catch (...) {
		throw;
	}
}

OpenCVCamera::~OpenCVCamera()
{
	m_camera.release();
}

Image * OpenCVCamera::acquireImage()
{
	int delay = 0;
	// Wait for the camera to be ready
	handleAcquisitionDelay();

	// Grab the image
	Image * image = new Image(m_imageWidth, m_imageHeight, 3);
	unsigned char* pixels = image->getPixels();
	while (delay < 10) {
		time_point<Clock> start = Clock::now();
		m_camera >> m_matImage; 
		time_point<Clock> end = Clock::now();
		milliseconds diff = duration_cast<milliseconds>(end - start);
		delay = diff.count();
	}
	cvtColor(m_matImage, m_matImage, COLOR_BGR2RGB);
//	image->assignPixels(m_matImage.data);
	memcpy(pixels, m_matImage.data, image->getPixelBufferSize());

	return image;
}

void OpenCVCamera::releaseImage(Image * image)
{
	delete image;
}

int OpenCVCamera::getImageHeight() const
{
	return m_imageHeight;
}

int OpenCVCamera::getImageWidth() const
{
	return m_imageWidth;
}

int OpenCVCamera::getImageComponents() const
{
	return 3;
}

real OpenCVCamera::getSensorWidth() const
{
	return 3.629;
}

real OpenCVCamera::getSensorHeight() const
{
	return 2.722;
}

real OpenCVCamera::getFocalLength() const
{
	return 3.6;
}
} // end ns scanner
