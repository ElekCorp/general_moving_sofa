#pragma once

#include "pont.h"

class halfline
{
private:
	pont x, e;//x a veg pontja e pedig egy vektor
	//e egyseg vektorra lesz normalva
public:
	halfline(pont x, pont e)
	{
		this->x = x;
		if (e.x_out() == 0 && e.y_out() == 0)
		{
			std::cout << "hibas line constructor input\n";
		}
		double r = sqrt(e.x_out() * e.x_out() + e.y_out() * e.y_out());
		pont tmp(e.x_out() / r, e.y_out() / r);

		this->e = tmp;
	}
	halfline(const halfline& other)
	{
		x = other.x;
		e = other.e;
	}
	~halfline()
	{

	}
	halfline operator=(const halfline& z)
	{
		if (this == &z)
		{
			return z;
		}
		x = z.x;
		e = z.e;

		return z;
		
	}
	void renderer(SDL_Renderer* renderer) const
	{
		double step = (real_height + real_wide) / (WIDTH + HEIGHT);

		for (int i = 0; i < 10 * int(1.0 / step); i++)
		{
			pont tmp(x.x_out() + (i * step) * e.x_out(), x.y_out() + (i * step) * e.y_out());
			tmp.renderer(renderer);
		}
		
	}

};

