#include "Timing.h"
#include "ConsolePrint.h"
using namespace std;

//initialize timer;
Timing::Timing()
{
	QueryPerformanceFrequency(&frequency);
	freqPerMS = (double)(frequency.QuadPart);
	countStart.QuadPart = 0;
	DEBUG_PRINT("Initated Timer");
}

Timing::~Timing()
{
}

//run the clock
void Timing::RunTimer(double& DT)
{
	QueryPerformanceCounter(&countEnd);
	DT = (countEnd.QuadPart - countStart.QuadPart) / freqPerMS;
	countStart.QuadPart = countEnd.QuadPart;
}



