/*
  AeroQuad v3.0 - April 2011
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


#ifndef _AEROQUAD_MOTORS_I2C_H_
#define _AEROQUAD_MOTORS_I2C_H_

#include <WProgram.h>

#include "Motors.h"

#define MOTORBASE 0x28              // I2C controller base address
#define FRONTMOTORID MOTORBASE + 1  // define I2C controller addresses per your configuration
#define REARMOTORID  MOTORBASE + 3  // these addresses are for Phifun controllers
#define RIGHTMOTORID MOTORBASE + 2  // as installed on jihlein's homebrew AeroQuad 3.0
#define LEFTMOTORID  MOTORBASE + 4  // inspired frame

class Motors_I2C : public Motors {
private:
  float mMotorCommand;
  float bMotorCommand;
  
public:

  Motors_I2C();
	
  void initialize();
  void write();
  void commandAllMotors(int command);
  
};

#endif