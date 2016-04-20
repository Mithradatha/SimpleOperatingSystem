#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include "Operations.h"

using namespace std;

class Device {

private:

	bool connected;

public:

	bool isConnected() { return this->connected; }

	void setConnection(bool val) { this->connected = val; }

	virtual void display() = 0;
};

#endif