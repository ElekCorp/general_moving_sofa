#pragma once
#include "FP.h"
#include <cmath>

#include "SDL.h"
#include "halfline.h"


class folyoso
{
private:
	FP phi;
	FP tgphi;
	FP x0;
public:
	folyoso(FP phi = 0)
	{
		this->phi = phi;
		this->tgphi = tan(phi);
		this->x0 = 1/tan(asin(1) - phi / 2);
	}
	folyoso(folyoso& other)
	{
		phi = other.phi;
		tgphi = other.tgphi;
		x0 = other.x0;
	}
	//default dtor is elég

	FP phi_out(void) const
	{
		return phi;
	}
	FP tgphi_out(void) const
	{
		return tgphi;
	}
	FP x0_out(void) const
	{
		return x0;
	}

	void renderer(SDL_Renderer* renderer) const
	{
		pont vec_be(-1, 0);
		pont sarok(x0, 0);
		halfline also_line(sarok, vec_be);
		also_line.renderer(renderer);

		pont felso(0, 1);
		halfline felso_line(felso, vec_be);
		felso_line.renderer(renderer);

		pont vec_ki(cos(phi), sin(phi));
		halfline ki_also_line(sarok, vec_ki);
		ki_also_line.renderer(renderer);

		halfline felso_ki_line(felso, vec_ki);
		felso_ki_line.renderer(renderer);



	}

};

