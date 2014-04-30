#ifndef GENETICTHREAD_HPP
#define GENETICTHREAD_HPP

#include <utility>
#include <functional>
#include <algorithm>
#include <fstream>
//rondom
#include <ctime>
#include <random>
#include <GeneticEngine/random.hpp>
//multi threading
#include <mutex>
#include <thread>
#include <iostream>

#define GENETIQUE_SAVE_RESULTS

template <class T> class GeneticEngine;

/* T = class Individu */
template <class T>
class GeneticThread
{
    public:
    /* taux_mut = mutation rate [0 .. 1]
    *  filename = where the output have to be save
    *  pop_size = size of the population
    *  size_child = number of child
    *  Args ... = argument to T(Args& ... args ) constructor */
        template <typename ... Args>
        GeneticThread(float taux_mut,std::string filename,int pop_size,int pop_child,Args&& ... args);

        ~GeneticThread();

        /*return the best (greater score)
        * you have to delete it yourself
        * run for nb_generation
        */
        void run(const int nb_generation);
        
        /* return the best
        * you have to delete it youself
        * run until f return true
        */
        
        void run_while(bool (*f)(const T&,const int));

        T* get_best()const {return best;};

        inline void stop(){running=false;}

        inline int get_size()const {return size;};

        /****** FONCTIONEMENT MODE ************/



    private:
        friend class GeneticEngine<T>;
        T** individus;
        const int size;
        const int size_child;
        const float mutation_taux;
        int generation;
        const std::string prefix;
        bool initial_evaluation_req;
        T* best;
        /* Fonction to call to create children */
        void (GeneticThread<T>::*creatChildFunc)();
        /* Fonction to call to  reduce pop */
        void (GeneticThread<T>::*reducePopFunc)();


        enum class CreationMode {STUPIDE=0,TOURNAMENT};
        enum class ReductionMode {STUPIDE=0,TOURNAMENT};

        void setCreationMode(CreationMode val);
        void setReductionMode(ReductionMode val);



        /*************** FONCTIONS *******************/
        
        /* eval all the population */
        void init();

        std::mutex mutex;
        std::thread thread;
        volatile bool running;

        /* sort the pop
        *  make children using T.crossOver(const T& other) + childre.mutate()
        *  remove worst and replace them with childrens
        */
        void corps();

        /* create a new T using parent1 & 2
         * It use crossover, and mutate internatly
         */
        T* makeNew(const T* parent1,const T& parent2);

        /* At the end:
        *  save it in last.res file
        */
        void end();

        /* save current best in file using << operator */
        void save(const std::string& name);


        /********** CREATIONS *****************/
        void stupideCreation();
        void tournamentCreation();

        /********** REDUCTION *******************/
        void stupidReduction();
        void tournamentReduction();


};

#include <GeneticEngine/GeneticThread.tpl>
#endif
