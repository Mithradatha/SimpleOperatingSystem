#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include "Operations.h"

using namespace std;

/*
* Abstract class for Peripheral Devices
*
* All derived classes inherit the 'connected' state,
* as well as a getter/setter for that member.
*
* All derived classes MUST implement the 'display' function,
* or pass the responsibility to their children.
*
* All derived classes CAN implement behaviors from the 
* included 'Operations' namespace
*/

class Device {

private:

	bool connected;

public:

	bool isConnected() { return this->connected; }	// is the device currenty connected?

	void setConnection(bool val) { this->connected = val; }	// set the device's connection state

	virtual void display() = 0;
};

#endif