#include "PudEntity/PudEntityController.h"
#include <windows.h>

PudEntityController::PudEntityController()
{}

PudEntityController::~PudEntityController()
{}

int PudEntityController::getDesiredXDelta() const
{
	//Determine delta
	int iDelta = 0;
	if(GetAsyncKeyState('J') & 0x8000) iDelta -= 1;
	if(GetAsyncKeyState('L') & 0x8000) iDelta += 1;
	
	//Return successfully
	return iDelta;
}

int PudEntityController::getDesiredZDelta() const
{
	//Determine delta
	int iDelta = 0;
	if(GetAsyncKeyState('K') & 0x8000) iDelta -= 1;
	if(GetAsyncKeyState('I') & 0x8000) iDelta += 1;

	//Return successfully
	return iDelta;
}



