#include "RandLi.h"


RandLi::RandLi() {
	nProcGen = 0;
}
RandLi::~RandLi() {

}

uint32_t RandLi::rand()
{
	nProcGen += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nProcGen * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

double RandLi::rndDouble(double min, double max)
{
	return ((double)rand() / (double)(0x7FFFFFFF)) * (max - min) + min;
}

int RandLi::rndInt(int min, int max)
{
	return (rand() % (max - min)) + min;
}