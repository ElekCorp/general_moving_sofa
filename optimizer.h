#pragma once
#include "folyoso.h"
#include "polygon.h"



class optimizer
{
private:
    polygon sofa;
    folyoso sarok;
    FP fordulasiszog;

    int optimizer_ponts;
    FP delta;//max eltolas nagysag x,y iranyokban
    std::vector<FP> vec_x;
    std::vector<FP> vec_y;
public:
    optimizer() {}
    optimizer(FP radius, int num_of_points, folyoso sarok, FP fordulasiszog, int optimizer_ponts = 100, FP delta = 0.1)
    {
        std::vector<FP> x, y;

        for (int i = 0; i < num_of_points; ++i)
        {
            x.push_back(radius * cos((2 * pi / num_of_points) * i));
            y.push_back(radius * sin((2 * pi / num_of_points) * i));
        }
        polygon tmp(x, y, num_of_points);
        sofa = tmp;

        for (int i = 0; i < optimizer_ponts; ++i)
        {
            vec_x.push_back(0);
            vec_y.push_back(0);
        }

        //polygon teszt(x, y, num_of_points);
        this->sarok = sarok;
        this->fordulasiszog = fordulasiszog;
        this->optimizer_ponts = optimizer_ponts;
        this->delta = delta;
    }
    optimizer(FP radius, int num_of_points, folyoso sarok, int optimizer_ponts = 100, FP delta = 0.1)
    {
        std::vector<FP> x, y;

        for (int i = 0; i < num_of_points; ++i)
        {
            x.push_back(radius * cos((2 * pi / num_of_points) * i));
            y.push_back(radius * sin((2 * pi / num_of_points) * i));
        }
        polygon tmp(x, y, num_of_points);
        sofa = tmp;

        for (int i = 0; i < optimizer_ponts; ++i)
        {
            vec_x.push_back(0);
            vec_y.push_back(0);
        }


        //polygon teszt(x, y, num_of_points);
        this->sarok = sarok;
        this->fordulasiszog = sarok.phi_out();
        this->optimizer_ponts = optimizer_ponts;
        this->delta = delta;
    }
    optimizer(FP radius,int num_of_points,folyoso sarok,std::vector<FP> vec_xy, FP delta = 0.1)
    {
        std::vector<FP> x, y;

        for (int i = 0; i < num_of_points; ++i)
        {
            x.push_back(radius * cos((2 * pi / num_of_points) * i));
            y.push_back(radius * sin((2 * pi / num_of_points) * i));
        }
        polygon tmp(x, y, num_of_points);
        sofa = tmp;

        int optimizer_ponts=vec_xy.size()/2;
        //std::cout<<optimizer_ponts<<"\n";
        for (int i = 0; i < optimizer_ponts; ++i)
        {
            double dx=vec_xy[i];
            double dy=vec_xy[optimizer_ponts+i];

            vec_x.push_back(dx);
            vec_y.push_back(dy);

        }

        //polygon teszt(x, y, num_of_points);
        this->sarok = sarok;
        this->fordulasiszog = sarok.phi_out();
        this->optimizer_ponts = optimizer_ponts;
        this->delta = delta;


    }
    //optimizer(optimizer& other) //default cpyctor elég
    //~optimizer() //default dtor elég
    optimizer operator=(optimizer const& other)
    {
        if (this == &other)
        {
            return *this;
        }

        sofa = other.sofa;
        sarok = other.sarok;
        fordulasiszog = other.fordulasiszog;

        optimizer_ponts = other.optimizer_ponts;
        delta = other.delta;
        vec_x = other.vec_x;
        vec_y = other.vec_y;

        return *this;
    }

    FP area(void)
    {
        sofa.first_cut(sarok);

        for (int i = 0; i < optimizer_ponts; ++i)
        {

            if (vec_x[i] + vec_y[i] < delta)
            {
                sofa.rotation(fordulasiszog / optimizer_ponts);
                sofa.transport(vec_x[i], vec_y[i]);
                sofa.cut(sarok);
            }
            else
            {
                int num = int((vec_x[i] + vec_y[i]) / delta);
                FP step_x = vec_x[i] / num;
                FP step_y = vec_y[i] / num;

                for (int i = 0; i < num; ++i)
                {
                    sofa.rotation(fordulasiszog / (optimizer_ponts*num));
                    sofa.transport(step_x, step_y);
                    sofa.cut(sarok);
                }

            }
        }

        sofa.last_cut(sarok);

        return sofa.area();
    }

    FP area(const optimizer& start)
    {
        this->start_position(start);

        //sofa.first_cut(sarok);

        for (int i = 0; i < optimizer_ponts; ++i)
        {

            if (vec_x[i] + vec_y[i] < delta)
            {
                sofa.rotation(fordulasiszog / optimizer_ponts);
                sofa.transport(vec_x[i], vec_y[i]);
                sofa.cut(sarok);
            }
            else
            {
                int num = int((vec_x[i] + vec_y[i]) / delta);
                FP step_x = vec_x[i] / num;
                FP step_y = vec_y[i] / num;

                for (int i = 0; i < num; ++i)
                {
                    sofa.rotation(fordulasiszog / (optimizer_ponts * num));
                    sofa.transport(step_x, step_y);
                    sofa.cut(sarok);
                }

            }
        }

        //sofa.last_cut(sarok);

        return sofa.area();
    }

    void renderer(SDL_Renderer* renderer) const
    {
        sofa.renderer(renderer);
    }

    void optimizer_step(FP epsilon)
    {
        for (int i = 0; i < optimizer_ponts; ++i)
        {
            vec_x[i] += (epsilon * double(rand()) / RAND_MAX) - epsilon / 2;
            vec_y[i] += (epsilon * double(rand()) / RAND_MAX) - epsilon / 2;
        }
    }

    void start_position(const optimizer& start)
    {
        sofa = start.sofa;
        sarok = start.sarok;
        fordulasiszog = start.fordulasiszog;

        optimizer_ponts = start.optimizer_ponts;
        delta = start.optimizer_ponts;
    }

    void reverse(void)
    {
        for (int i = optimizer_ponts - 1; i >= 0; --i)
        {
            sofa.transport(-vec_x[i], -vec_y[i]);
            sofa.rotation(-fordulasiszog / optimizer_ponts);
        }

    }

    void renderer_frame(SDL_Renderer* renderer, int frame)
    {
        if (frame >= optimizer_ponts)
        {
            frame = optimizer_ponts - 1;
        }
        sofa.rotation(fordulasiszog / optimizer_ponts);
        sofa.transport(vec_x[frame], vec_y[frame]);
        sofa.renderer(renderer);

    }//*/

};

