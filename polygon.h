#pragma once
#include <list>
#include <vector>

#include "pont.h"
#include "folyoso.h"

#include "SDL.h"
#include "screen.h"

#define list vector

class polygon
{
private:
    std::list<pont> ponts;//elso es utolso eleme ugyan azon pontot reprezentalja de memoria terulete kulonbozo
    //körüljárási irány határozza meg hogy melyik oldal a "belseje" (cut fv hogyan mûködik)

public:
    polygon();
    polygon(FP* pt_x, FP* pt_y, int const size);
    polygon(std::vector<FP> pt_x, std::vector<FP> pt_y, int const size);
    polygon(const polygon& other);
    ~polygon()//default elég
    {

    }

    void cut(folyoso sarok)//plygon éle és a folyosoval valo metszes eredmenye
    {
        //#pragma omp parallel for
        for (std::list<pont>::iterator it = ponts.begin(); it != ponts.end(); ++it)
        {
            it->move_in(sarok);
        }
    }

    void last_cut(folyoso sarok)//plygon éle és a folyosoval valo metszes eredmenye
    {
        //#pragma omp parallel for
        for (std::list<pont>::iterator it = ponts.begin(); it != ponts.end(); ++it)
        {
            it->last_cut(sarok);
        }
    }

    void first_cut(folyoso sarok)//plygon éle és a folyosoval valo metszes eredmenye
    {
        //#pragma omp parallel for
        for (std::list<pont>::iterator it = ponts.begin(); it != ponts.end(); ++it)
        {
            it->first_cut(sarok);
        }
    }

    FP area(void) const
    {
        FP area = 0;

        std::list<pont>::const_iterator it0 = ponts.begin();
        for (std::list<pont>::const_iterator it = ++(ponts.begin()); it != ponts.end(); ++it)
        {
            FP x1 = (it0->x_out());
            FP x2 = (it->x_out());
            FP y1 = (it0->y_out());
            FP y2 = (it->y_out());
            area += x1 * y2 - x2 * y1;
            ++it0;
        }

        return area/2;
    }

    void renderer(SDL_Renderer* renderer) const
    {
        double step = 1.0 / (WIDTH + HEIGHT);
        std::list<pont>::const_iterator it0 = ponts.begin();
        for (std::list<pont>::const_iterator it = ++(ponts.begin()); it != ponts.end(); ++it)
        {

            pont tmp((*it).x_out(), (*it).y_out());
            tmp.renderer(renderer);//*/

            /*for (int i = 0; i < int(1.0/step); i++)
            {
            	pont tmp((1-i*step)*(*it0).x_out()+(i*step)*(*it).x_out(), (1 - i * step) * (*it0).y_out() + (i * step) * (*it).y_out());
            	tmp.renderer(renderer);
            }//*/
            ++it0;
        }
    }

    void rotation(FP phi)
    {
        for (std::list<pont>::iterator it = ponts.begin(); it != ponts.end(); ++it)
        {
            it->rotation(phi);
        }
    }
    void transport(pont pt)
    {
        for (std::list<pont>::iterator it = ponts.begin(); it != ponts.end(); ++it)
        {
            it->transport(pt);
        }
    }
    void transport(FP x, FP y)
    {
        for (std::list<pont>::iterator it = ponts.begin(); it != ponts.end(); ++it)
        {
            it->transport(x, y);
        }
    }


};



