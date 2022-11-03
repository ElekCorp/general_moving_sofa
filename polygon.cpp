#include "polygon.h"

polygon::polygon()
{
}

polygon::polygon(FP* pt_x, FP* pt_y, int const size)
{
	for (int i = 0; i < size; ++i)
	{
		pont tmp(pt_x[i], pt_y[i]);
		ponts.push_back(tmp);
	}
	pont tmp(pt_x[0], pt_y[0]);
	ponts.push_back(tmp);
}

polygon::polygon(std::vector<FP> pt_x, std::vector<FP> pt_y, int const size)
{
	if (pt_x.size()!=pt_y.size())
	{
		std::cerr << "nemazonos meretu vectorok polygon constructor\n";
	}
	for (int i = 0; i < size; ++i)
	{
		pont tmp(pt_x[i], pt_y[i]);
		ponts.push_back(tmp);
	}
	pont tmp(pt_x[0], pt_y[0]);
	ponts.push_back(tmp);
}

polygon::polygon(const polygon& other)
{
	ponts = other.ponts;
}
