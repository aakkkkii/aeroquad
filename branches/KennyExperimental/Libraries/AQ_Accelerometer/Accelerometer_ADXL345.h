/*
  AeroQuad v3.0 - May 2011
  www.AeroQuad.com
  Copyright (c) 2011 Ted Carancho.  All rights reserved.
  An Open Source Arduino based multicopter.
 
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef _AEROQUAD_ACCELEROMETER_ADXL345_H_
#define _AEROQUAD_ACCELEROMETER_ADXL345_H_

#include <Accelerometer.h>

#define ACCEL_ADDRESS 0x53

void initializeAccel() {
  accelScaleFactor = G_2_MPS2(4.0/1024.0);  		// +/- 2G at 10bits of ADC

  if (readWhoI2C(ACCEL_ADDRESS) !=  0xE5) 			// page 14 of datasheet
    Serial.println("Accelerometer not found!");
	
  updateRegisterI2C(ACCEL_ADDRESS, 0x2D, 1<<3); 	// set device to *measure*
  updateRegisterI2C(ACCEL_ADDRESS, 0x31, 0x08); 	// set full range and +/- 2G
  updateRegisterI2C(ACCEL_ADDRESS, 0x2C, 8+2+1);    // 200hz sampling
  delay(10); 
}
  
void measureAccel() {

  sendByteI2C(ACCEL_ADDRESS, 0x32);
  Wire.requestFrom(ACCEL_ADDRESS, 6);
  for (byte axis = XAXIS; axis < LASTAXIS; axis++) {
    if (axis == XAXIS)
      meterPerSec[axis] = ((Wire.receive()|(Wire.receive() << 8)) * accelScaleFactor) - accelZero[axis];
    else
      meterPerSec[axis] = accelZero[axis] - ((Wire.receive()|(Wire.receive() << 8)) * accelScaleFactor);
  }
}

void measureAccelSum() {

  sendByteI2C(ACCEL_ADDRESS, 0x32);
  Wire.requestFrom(ACCEL_ADDRESS, 6);
  for (byte axis = XAXIS; axis < LASTAXIS; axis++) {
    accelSample[axis] += ((Wire.receive()|(Wire.receive() << 8)));
  }
  accelSampleCount++;
}

void evaluateMeterPerSec() {
	
  for (byte axis = XAXIS; axis < LASTAXIS; axis++) {
    if (axis == XAXIS)
      meterPerSec[axis] = ((accelSample[axis] * accelScaleFactor)/accelSampleCount) - accelZero[axis];
    else
      meterPerSec[axis] = accelZero[axis] - ((accelSample[axis] * accelScaleFactor)/accelSampleCount);
	accelSample[axis] = 0.0;
  }
  accelSampleCount = 0;
}

void calibrateAccel() {

  int findZero[FINDZERO];
  int dataAddress;
    
  for (byte calAxis = XAXIS; calAxis < ZAXIS; calAxis++) {
    if (calAxis == XAXIS) dataAddress = 0x32;
    if (calAxis == YAXIS) dataAddress = 0x34;
    if (calAxis == ZAXIS) dataAddress = 0x36;
    for (int i=0; i<FINDZERO; i++) {
      sendByteI2C(ACCEL_ADDRESS, dataAddress);
      findZero[i] = readReverseWordI2C(ACCEL_ADDRESS);
      delay(10);
    }
    accelZero[calAxis] = findMedianInt(findZero, FINDZERO);
  }

  // replace with estimated Z axis 0g value
  accelZero[ZAXIS] = (accelZero[XAXIS] + accelZero[PITCH]) / 2;
  // store accel value that represents 1g
  measureAccel();
  accelOneG = -meterPerSec[ZAXIS];
}

#endif
