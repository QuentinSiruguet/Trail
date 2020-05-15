#include "Archiving.h"

void Archiving::retrievePiCamNumber()
{
	std::ifstream piCamNumberFile("/piCamNumberFile.txt");
	if (piCamNumberFile.is_open())
	{
		char buffer[2];
		piCamNumberFile.getline(buffer, 2);
		this->piCamNumber = std::stoi(buffer);
		piCamNumberFile.close();
	}
	else
		this->piCamNumber = 0;

	std::cout << "STARTING PICAM " << piCamNumber << std::endl;
}

Archiving::Archiving()
{
	this->retrievePiCamNumber();
}

int Archiving::getPiCamNumber()
{
	return this->piCamNumber;
}

std::string Archiving::getStringTime()
{
	std::time_t	currentDate = std::time(nullptr);
	std::stringstream returnedString;
	char str[5];

	strftime(str, 5, "%d", std::localtime(&currentDate)); //Jour
	returnedString << str << ":";
	strftime(str, 5, "%m", std::localtime(&currentDate)); //Mois
	returnedString << str << ":";
	strftime(str, 5, "%Y", std::localtime(&currentDate)); //Année
	returnedString << str << "_";
	strftime(str, 5, "%H", std::localtime(&currentDate)); //Heures
	returnedString << str << ":";
	strftime(str, 5, "%M", std::localtime(&currentDate)); //Minutes
	returnedString << str << ":";
	strftime(str, 5, "%S", std::localtime(&currentDate)); //Secondes

	returnedString << str;

	return returnedString.str();
}
