#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

class Archiving
{
private:
	int piCamNumber;

	void retrievePiCamNumber();
public:
	Archiving();
	int getPiCamNumber();
	std::string getStringTime();
};

