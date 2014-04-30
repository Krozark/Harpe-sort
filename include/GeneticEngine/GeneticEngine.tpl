#include <utility>

#include <thread>
#include <chrono>

#include <iostream>


template <class T>
template <typename ... Args>
GeneticEngine<T>::GeneticEngine(int nb_threads,float taux_mut,std::string filename,int pop_size,int pop_child,Args&& ... args) : size(nb_threads)
{
    if (size <= 0)
        size = std::thread::hardware_concurrency();
    if (size <= 0)
        size = 1;


    islands = new  GeneticThread<T>*[size];
    for(int i=0;i<size;++i)
        islands[i] = new GeneticThread<T>(taux_mut,filename,pop_size/size,pop_child/size,args ...);

    setTimeout(1000);

};

template<class T>
GeneticEngine<T>::~GeneticEngine()
{
    for(int i=0;i<size;++i)
        if(islands[i])
        {
            delete islands[i];
            islands[i] = 0;
        }
    delete [] islands;
};

template <class T>
//template <typename ... Args>
T* GeneticEngine<T>::run(const int nb_generation/*,Args& ... args*/)
{
    for(int i=0;i<size;++i)
        islands[i]->run(nb_generation/*,args ...*/);
    wait();
    return end();
};

template <class T>
//template <typename ... Args>
T* GeneticEngine<T>::run_while(bool (*f)(const T&,const int)/*,Args& ... args*/)
{
    for(int i=0;i<size;++i)
        islands[i]->run_while(f/*,args ...*/);
    wait();
    return end();
};



template<class T>
void GeneticEngine<T>::wait()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    running = true;
    //sender thread
    void (GeneticEngine<T>::*func)() = &GeneticEngine<T>::send;
    std::thread thread(func,this);
    do
    { 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for(int i=0;i<size and running ;++i)
            running = islands[i]->thread.joinable() and islands[i]->running;
    }
    while(running);
    //stop all islands
    stop();
    //wait the end of the sender thread
    thread.join();

    for(int i=0;i<size;++i)
        islands[i]->thread.join();
};


template<class T>
void GeneticEngine<T>::stop()
{
    running = false;
    for(int i=0;i<size;++i)
    {
        islands[i]->stop();
    }
};

template<class T>
void GeneticEngine<T>::send()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    while(running)
    {
        if (size > 1)
        {
            GeneticThread<T>* dest = islands[random(0,size-1)];
            GeneticThread<T>* src_pt;

            do
            {
                src_pt = islands[random(0,size-1)];
            }
            while(src_pt == dest);

            GeneticThread<T>& src = *src_pt;

            src.mutex.lock();
            T* best = src.get_best()->clone();
            src.mutex.unlock();

            std::cout<<"Sending value from ["<<src.thread.get_id()<<"] to ["<<dest->thread.get_id()<<"] : <"<<*best<<">"<<std::endl;

            send(best,*dest);
        }
        //wait a moment for the other send
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    }
};

template<class T>
void GeneticEngine<T>::send(T* id,GeneticThread<T>& dest)
{
    if(dest.size > 1)
    {
        dest.mutex.lock();
        int i = dest.size -1;
        if(dest.best == dest.individus[i])
            --i;
        delete dest.individus[i];
        dest.individus[i] = id;
        dest.mutex.unlock();
    }
};


template<class T>
T* GeneticEngine<T>::end()
{
    T* best = islands[0]->get_best();
    for(int i=1;i<size;++i)
    {
        T* __best = islands[i]->get_best();
        if(*__best > *best)
            std::swap(best,__best);
        delete __best;
    }
    return best;
};


template<class T>
void GeneticEngine<T>::setCreationMode(CreationMode val)
{
    for(int i=0;i<size;++i)
    {
        switch(val)
        {
            case CreationMode::STUPIDE:
                islands[i]->setCreationMode(GeneticThread<T>::CreationMode::STUPIDE);
            break;
            case CreationMode::TOURNAMENT:
                islands[i]->setCreationMode(GeneticThread<T>::CreationMode::TOURNAMENT);
            break;
        }
    }
};

template<class T>
void GeneticEngine<T>::setReductionMode(ReductionMode val)
{
    for(int i=0;i<size;++i)
    {
        switch(val)
        {
            case ReductionMode::STUPIDE:
                islands[i]->setReductionMode(GeneticThread<T>::ReductionMode::STUPIDE);
            break;
            case ReductionMode::TOURNAMENT:
                islands[i]->setReductionMode(GeneticThread<T>::ReductionMode::TOURNAMENT);
            break;
            }
    }
};


template<class T>
void GeneticEngine<T>::setEvaluateAll(bool v)
{
    for(int i=0;i<size;++i)
    {
        islands[i]->mutex.lock();
        islands[i]->initial_evaluation_req = v;
        islands[i]->mutex.unlock();
    }
};

