/*
 AVRUtils.cpp - AVRUtils library
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

#include "AVRUtils.h"
#include <Arduino.h>

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#else
    #error "AVRUtils does not support this processor. Please verify you have the right board and/or the right processor selected."
#endif

AVRUtils::AVRUtils(void) {
    
}

void AVRUtils::begin(double AVRTempCorr, double AVRVccCorr) {
    this->AVRTempCorr = AVRTempCorr;
    this->AVRVccCorr = AVRVccCorr;

}

double AVRUtils::getAVRTemp(void) {
    double result[100];
    for (uint8_t i=0; i < 100; i++) {
        // Read temperature sensor against 1.1V reference
        #if defined(__AVR_ATmega32U4__)
            ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
            ADCSRB = _BV(MUX5); // the MUX5 bit is in the ADCSRB register
        #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
            ADMUX = _BV(REFS1) | _BV(MUX5) | _BV(MUX1);
        #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
            ADMUX = _BV(REFS1) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
        #else
            ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);
        #endif
        
        delay(2); // Wait for ADMUX setting to settle
        ADCSRA |= _BV(ADSC); // Start conversion
        while (bit_is_set(ADCSRA,ADSC)); // measuring
        
        uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
        uint8_t high = ADCH; // unlocks both
        result[i] = (high << 8) | low; // combine the two
    }
    double average = 0;
    for (uint8_t i=0; i< 100; i++) {
        average += result[i];
    }
    average /= 100;
    average = (average - AVRTempCorr ) / 1.22;
    return (1.8 * average) + 32;
}

double AVRUtils::getAVRVcc(void) {
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
    #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV(MUX5) | _BV(MUX0);
    #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        ADMUX = _BV(MUX3) | _BV(MUX2);
    #else
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    #endif
    
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA,ADSC)); // measuring
    
    uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both

    return AVRVccCorr / ((high<<8) | low); // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
}