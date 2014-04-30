#ifndef GENETICENGINE_HPP
#define GENETICENGINE_HPP

#include <GeneticEngine/GeneticThread.hpp>

/* T is Individu Class  */
template <class T>
class GeneticEngine
{
    public:
        template <typename ... Args>
        GeneticEngine(int nb_threads,float taux_mut,std::string filename,int pop_size,int pop_child,Args&& ... args);

        ~GeneticEngine();

        //template <typename ... Args>
        T* run(const int nb_generation/*,Args& ... args*/);

        //template <typename ... Args>
        T* run_while(bool (*f)(const T&,const int)/*,Args& ... args*/);

        void stop();
        
        enum class CreationMode {STUPIDE=0,TOURNAMENT};
        enum class ReductionMode {STUPIDE=0,TOURNAMENT};

        void setCreationMode(CreationMode val);
        void setReductionMode(ReductionMode val);

        void setEvaluateAll(bool v);

        void setTimeout(unsigned int t){timeout = t;};
        
    private:
        GeneticThread<T>** islands;
        int size;
        volatile bool running;

        unsigned int timeout;

        void send();
        void send(T* id,GeneticThread<T>& dest);


        void wait();


        T* end();
};

#include <GeneticEngine/GeneticEngine.tpl>

#endif
