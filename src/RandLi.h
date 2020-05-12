#pragma once
#include "Base.h"

class RandLi
{
public:
	RandLi();
	~RandLi();

	uint32_t rand();
	double rndDouble(double min, double max);
	int rndInt(int min, int max);

public:

	uint32_t nProcGen = 0;
};

