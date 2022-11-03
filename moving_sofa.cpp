#include <iostream>

#include "optimizer.h"

#include "SDL.h"


#include "screen.h"

#include <vector>

#include <chrono>

#include <omp.h>

#include "nlopt.hpp"

///TODO: replace point pushing with polygon slicing (find intersections with the wall and create a new polygon with a cut (and so on ...))

void rand_optimizer(optimizer& sofa_opt,const optimizer& start);
int count=0;
double myvfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data)
{
    return sqrt(x[1]*x[1]+x[0]*x[0]);
}

double sofa_area(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);

int main(int argv, char* args[])
{
    //srand(time(NULL));


    //nlopt::opt opt(nlopt::LN_COBYLA, 2);

//std::vector<double> lb(2);
//lb[0] = -HUGE_VAL; lb[1] = 0;
//opt.set_lower_bounds(lb);

    int num_of_ponts=100;
    double fordulasiszog=pi/3;
    double radius=2;
    double delta=0.1;

    double data_in[4];
    data_in[0]=fordulasiszog;
    data_in[1]=num_of_ponts;
    data_in[2]=radius;
    data_in[3]=delta;

    //nlopt::opt opt(nlopt::GN_DIRECT_L_RAND, num_of_ponts*2);
    nlopt::opt opt(nlopt::LN_COBYLA, num_of_ponts*2);
    opt.set_max_objective(sofa_area, (void*)data_in);
    opt.set_xtol_abs(10e-4);


    std::vector<double> x(num_of_ponts*2);
    std::vector<double> lower_b,upper_b;
    for(int i=0; i<x.size(); ++i)
    {
        x[i]=1.0/num_of_ponts;
        lower_b.push_back(-0.1);
        upper_b.push_back(0.1);

    }

     opt.set_lower_bounds(lower_b);
     opt.set_upper_bounds(upper_b);


    double minf;

    try
    {
        nlopt::result result = opt.optimize(x, minf);
        std::cout << minf << "\n iteraciok szama:"<<count<< std::endl;
    }
    catch(std::exception &e)
    {
        std::cout << "nlopt failed: " << e.what() << std::endl;
    }

    #pragma omp parallel
    {
        srand(int(time(NULL)) ^ omp_get_thread_num());
    }

    /* SDL inicializálása és ablak megnyitása */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window* window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (window == NULL)
    {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    /* az elvegzett rajzolasok a kepernyore */
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);



    folyoso sarok(fordulasiszog);
    sarok.renderer(renderer);

    optimizer sofa_start(radius, num_of_ponts, sarok,num_of_ponts/2,delta);
    optimizer sofa_opt(radius, num_of_ponts, sarok,x,delta);



    //rand_optimizer(sofa_opt, sofa_start);

    SDL_RenderPresent(renderer);




    bool quit = false;

    int frame = 0;
    int max_frame = num_of_ponts/2;

    while (!quit)
    {
        frame = frame % max_frame;
        SDL_Event event;
        SDL_PollEvent(&event);



        if (event.type==SDL_QUIT)
        {
            quit = true;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        sarok.renderer(renderer);
        sofa_opt.renderer_frame(renderer, frame);
        SDL_RenderPresent(renderer);

        if (frame % max_frame == 0)
        {
            sofa_opt.area(sofa_start);
            sofa_opt.reverse();
            sofa_opt.area();
            sofa_opt.reverse();
        }
        ++frame;
        //std::cout << frame << "\n";
        SDL_Delay(1000 / max_frame);
    }//*/

    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}

double sofa_area(const std::vector<double> &x, std::vector<double> &grad, void *pt_void)//pt_void double* phi, polyogon pontjai,kezdeti kör sugara,delta
{
    ++count;

    double* pt=(double*)pt_void;
    double phi=pt[0];
    int num_of_points=int(pt[1]);
    double radius=pt[2];
    double delta=pt[3];

    folyoso sarok(phi);

    std::vector<FP> vec_x, vec_y;

    for (int i = 0; i < num_of_points; ++i)
    {
        vec_x.push_back(radius * cos((2 * pi / num_of_points) * i));
        vec_y.push_back(radius * sin((2 * pi / num_of_points) * i));
    }

    polygon sofa(vec_x, vec_y, num_of_points);

    sofa.first_cut(sarok);

    int optimizer_ponts=x.size()/2;
    for (int i = 0; i < optimizer_ponts; ++i)
    {

        double dx=x[i];
        double dy=x[optimizer_ponts+i];

        /*sofa.rotation(phi / optimizer_ponts);
        sofa.transport(dx, dy);
        sofa.cut(sarok);//*/


        if (dx + dy < delta)
        {
            sofa.rotation(phi / optimizer_ponts);
            sofa.transport(dx, dy);
            sofa.cut(sarok);
        }
        else
        {
            int num = int((dx + dy) / delta);
            FP step_x = dx / num;
            FP step_y = dy / num;

            for (int i = 0; i < num; ++i)
            {
                sofa.rotation(phi / (optimizer_ponts*num));
                sofa.transport(step_x, step_y);
                sofa.cut(sarok);
            }

        }//*/
    }

    sofa.last_cut(sarok);

    return sofa.area();

}

void rand_optimizer(optimizer& sofa_opt, const optimizer& sofa_start)
{
    auto start = std::chrono::high_resolution_clock::now();

    optimizer sofa2 = sofa_opt;

    std::cout << sofa_opt.area(sofa_start) << "\n";


    for (int i = 0; i < 100; ++i)
    {
        sofa_opt.optimizer_step(0.01);
        //std::cout << sofa_opt.area(sofa_start) << "\t" << sofa2.area(sofa_start) << "\n";
        if (sofa_opt.area(sofa_start) < sofa2.area(sofa_start))
        {
            sofa_opt = sofa2;
            //std::cout << sofa_opt.area(sofa_start) << "\n";
        }

    }//*/

    sofa_opt.reverse();

    std::cout << sofa_opt.area(sofa_start) << "\n";
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cout << "lefutasi ido: " << time_span.count() << "\n";
}
