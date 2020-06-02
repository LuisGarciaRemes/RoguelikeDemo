#pragma once
#include <Windows.h>

class Timing
{
public:
	Timing();
	~Timing();
	void RunTimer(double& DT);

private:
	double freqPerMS;
	LARGE_INTEGER frequency;
	LARGE_INTEGER countStart;
	LARGE_INTEGER countEnd;
};

