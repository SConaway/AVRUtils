/*
 AVRUtils.h - AVRUtils library
 Copyright (c) 2017 Steven Conaway.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _AVRUTILS_
#define _AVRUTILS_

#include <Arduino.h>

class AVRUtils {
	public:


		/**
		 * Creates an instance to monitor voltage and temperature.
		 */
		AVRUtils(void);

		/**
		 * Initializes the library by optionally setting the AVR's temperature correction factor,
		 * and the AVR's voltage correction factor.
		 */
		void begin(double AVRTempCorr = 324.31, double AVRVccCorr = 1125300L);

		/**
		 * Returns the current temperature in Celsius.
		 */
		double getAVRTemp(void);

		/**
		 * Returns the current temperature in Fahrenheit.
		 */
		double getAVRTempF(void);

		/**
		 * Returns the current voltage in millivolts.
		 */
		double getAVRVcc(void);


	private:
		double AVRTempCorr;
        double AVRVccCorr;
};

#endif // _AVRUtils_
