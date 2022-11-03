#pragma once
#include "FP.h"
#include <cmath>

#include "screen.h"
#include "SDL.h"
#include <iostream>

class folyoso;//csak deklaracio hogy a pont tudja hogy van ilyen

class pont
{
private:
	FP x, y;

public:
	pont(FP x = 0, FP y = 0)
	{
		this->x = x;
		this->y = y;
	}
	pont(const pont& other)
	{
		x = other.x;
		y = other.y;
	}
	~pont()
	{

	}

	pont operator=(const pont& z)
	{
		if (this==&z)
		{
			return z;
		}

		this->x = z.x;
		this->y = z.y;

		return z;
	}
	void print(void) const
	{
		std::cout << x << "\t" << y << "\n";
	}

	pont pont_to_pixel(void) const
	{
		pont tmp(int((x + real_wide / 2) * WIDTH / real_wide), int((-y + real_height / 2) * HEIGHT / real_height));
		return tmp;

	}
	void renderer(SDL_Renderer* renderer) const
	{
		pont tmp = this->pont_to_pixel();
		SDL_RenderDrawPoint(renderer, tmp.x, tmp.y);
	}

	void rotation(FP phi)
	{
		pont tmp = (*this);
		x = tmp.x * cos(phi) - tmp.y * sin(phi);
		y = tmp.x * sin(phi) + tmp.y * cos(phi);
	}
	void transport(pont pt)
	{
		x += pt.x;
		y += pt.y;
	}
	void transport(FP x, FP y)
	{
		this->x += x;
		this->y += y;
	}

	FP x_out(void) const
	{
		return x;
	}
	FP y_out(void) const
	{
		return y;
	}


	void move_in(folyoso sarok);
	bool move_in_print(folyoso sarok) const;

	void first_cut(folyoso sarok);
	void last_cut(folyoso sarok);

};



