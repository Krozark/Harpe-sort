extern std::default_random_engine generator;


#include <utility>
#include <utils/log.hpp>

/* return *x>*y */
template <typename T>
struct gt_ptr : std::binary_function<T,T,bool>
{
    bool operator() (const T* x, const T* y) const {return *x>*y;}
};

template <typename T>
template <typename ... Args>
GeneticThread<T>::GeneticThread(int id,float taux_mut,std::string filename,int pop_size,int pop_child, Args&& ... args) : id(id), size(pop_size), size_child(pop_child), mutation_taux(taux_mut), generation(0), prefix("best/"+filename), running(false)
{
    mutex.lock();
    best = nullptr;
    individus = new T*[size+size_child];
    for(int i=0;i<size;++i)
    {
        individus[i] = new T(std::forward<Args>(args) ...);
    }

    /***** init fonction to use *********/
    initial_evaluation_req = false;
    mutex.unlock();

    setCreationMode(CreationMode::TOURNAMENT);
    setReductionMode(ReductionMode::TOURNAMENT);
};

template <typename T>
GeneticThread<T>::~GeneticThread()
{
    for(int i=0;i<size;++i)
        if(individus[i] and individus[i] != best)
        {
            delete individus[i];
            individus[i] = nullptr;
        }
    delete [] individus;
};

template <typename T>
void GeneticThread<T>::run(const int nb_generation)
{
    //will be execute in thread
    auto lambda = [&](int nb_generation)
    {
        //eval initiale
        this->init();
        //boucle de génération
        while(generation<nb_generation and this->running)
            this->corps();
        this->end();
    };

    //start thread
    thread = std::thread(lambda,nb_generation);
};

template <typename T>
void GeneticThread<T>::run_while(bool (*f)(const T&,int,int id))
{

    //will be execute in thread
    auto lambda = [&](bool (*f)(const T&,int,int id))
    {
        //eval initiale
        this->init();
        do
        {
            this->corps();
        }while ((not f(*this->best,this->generation,this->id)) and this->running);
        this->end();
    };

    //start thread
    thread = std::thread(lambda,f);
};

template <typename T>
void GeneticThread<T>::init()
{
    utils::log::info(id,"Start init");
    mutex.lock();
    running = true;

    best = individus[0];
    individus[0]->eval();

    if(initial_evaluation_req)
    {
        for(int i=1;i<size;++i)
        {
            individus[i]->eval();
            if(*individus[i] > *best)
                best = individus[i];
        }
    }

    mutex.unlock();
    utils::log::info(id,"End init");
};

template <typename T>
void GeneticThread<T>::corps()
{
    mutex.lock();
    // create children
    (this->*createChildFunc)();

    //reduce pop
    (this->*reducePopFunc)();


    #ifdef GENETIQUE_SAVE_RESULTS
        save(std::to_string(best->get_score()));
    #endif
    mutex.unlock();

    utils::log::info(id,"generation #",generation++);

};


template<class T>
T* GeneticThread<T>::makeNew(const T* parent1,const T& parent2)
{
    T* res = parent1->crossOver(parent2);
    if (random(0.f,1.f) < mutation_taux)//Mutation !
    {
        res->mutate();

        for (int j=100;j<res->size();j+=100) //mutation proportianal to his size
            res->mutate();
    }

    if(initial_evaluation_req and res->need_eval())
        res->eval();

    return res;
};

template <typename T>
void GeneticThread<T>::end()
{
    utils::log::info(id,"Start end");
    mutex.lock();
    
    for(int i=0;i<size;++i)
        best = (*individus[i]>*best)?individus[i]:best;

    save("last");
    mutex.unlock();
    running = false;
    utils::log::info(id,"End end");
};

template <typename T>
void GeneticThread<T>::save(const std::string& name)
{
    std::string filename(prefix+"_"+name);
    if(not std::ifstream(filename))
    {
        time_t temps;
        struct tm* datetime;
        char  format[32];
        time(&temps);
        datetime = localtime(&temps);
        strftime(format, 32, "%d-%m-%Y %H:%M", datetime);

        std::ofstream file;
        file.open(filename);
        file<<"/*"
            <<"\n*\tdate: "<<format
            <<"\n*\tscore: "<<best->get_score()
            <<"\n*\tsize: "<<best->size()
            <<"\n*\tgeneration: "<<generation
            <<"\n*/"
            <<"\n"<<*best
            <<"\n";
        file.close();
        utils::log::ok(id,"best (score=",best->get_score(),"): ",*best);
    }
};

/******************** CUSTOM FONCTION **********************/
template<class T>
void GeneticThread<T>::setCreationMode(CreationMode val)
{
    mutex.lock();
    switch(val)
    {
        case CreationMode::STUPIDE:
            createChildFunc = &GeneticThread<T>::stupideCreation;
            initial_evaluation_req = true;
        break;

        case CreationMode::TOURNAMENT:
            createChildFunc = &GeneticThread<T>::tournamentCreation;
        break;
        
    }
    mutex.unlock();
};

template<class T>
void GeneticThread<T>::setReductionMode(ReductionMode val)
{
    mutex.lock();
    switch(val)
    {
        case ReductionMode::STUPIDE:
            reducePopFunc = &GeneticThread<T>::stupidReduction;
            initial_evaluation_req = true;
        break;

        case ReductionMode::TOURNAMENT:
            reducePopFunc = &GeneticThread<T>::tournamentReduction;
        break;
    }
    mutex.unlock();
};

//creation
template<class T>
void GeneticThread<T>::stupideCreation()
{
    //en tri les size - size_child
    std::partial_sort(individus,individus+size_child,individus+size,gt_ptr<T>());
    best = individus[0];
    //creation des enfants
    for(int i=0;i<size_child;++i)
    {
        int c = size+i;
        //on prend que les meilleurs, mais avec random
        individus[c] = makeNew(individus[i%size],*individus[random(0,size-1)]);

    }
};

template<class T>
void GeneticThread<T>::tournamentCreation()
{
    for (int i=size;i<int(size+size_child);++i)
    {
        //random individus
        T* id_rand[7];
        for(int j=0;j<7;++j)
        {
            id_rand[j] = individus[random(0,size-1)];

            if(id_rand[j]->need_eval())
                id_rand[j]->eval();
        }

        T* tmp_best[2];

        tmp_best[0] = (*id_rand[0]>*id_rand[1])?id_rand[0]:id_rand[1];
        tmp_best[1] = (*id_rand[2]>*id_rand[3])?id_rand[2]:id_rand[3];
        tmp_best[0] = (*tmp_best[0]>*tmp_best[1])?tmp_best[0]:tmp_best[1];

        tmp_best[1] = (*id_rand[4]>*id_rand[5])?id_rand[4]:id_rand[5];
        tmp_best[1] = (*id_rand[6]>*tmp_best[1])?id_rand[6]:tmp_best[1];
        //create child
        individus[i] = makeNew(tmp_best[1],*tmp_best[0]);

        best = (*tmp_best[0]>*best)?tmp_best[0]:(*tmp_best[1]>*best)?tmp_best[1]:best;

    }
};


//reduction
template<class T>
void GeneticThread<T>::stupidReduction()
{
    std::partial_sort(individus,individus+size,individus+size+size_child,gt_ptr<T>());
    for(int i=0;i<size_child;++i)
    {
        int c = size+i;
        delete individus[c];
        individus[c] = 0;
    }
    best = individus[0];
};

template <class T>
void GeneticThread<T>::tournamentReduction()
{
    for(int i=size+size_child-1;i>=size;--i)
    {
        //random individus
        std::pair<int,T*> id_rand[7]; //i,Individu*
        for(int j=0;j<7;++j)
        {
            int r;
            do{
               r = random(0,i);
            }while (individus[r] == best);
            id_rand[j] = std::make_pair(r,individus[r]);

            if(individus[r]->need_eval())
                individus[r]->eval();
        }

        std::pair<int,T*> worst[3];
        
        //calc the worst
        worst[0] = (*id_rand[0].second>*id_rand[1].second)?id_rand[1]:id_rand[0];
        worst[1] = (*id_rand[2].second>*id_rand[3].second)?id_rand[3]:id_rand[2];
        worst[0] = (*worst[0].second>*worst[1].second)?worst[1]:worst[0];

        worst[1] = (*id_rand[4].second>*id_rand[5].second)?id_rand[5]:id_rand[4];
        worst[1] = (*id_rand[6].second>*worst[1].second)?worst[1]:id_rand[6];
    
        worst[2] = (*worst[0].second>*worst[1].second)?worst[1]:worst[0];

        //delete worst
        delete worst[2].second;

        //move last at new free place
        individus[worst[2].first] = individus[i];
        individus[i] = 0;

        /*if(best == worst[2].second)
        {
            if(worst[2].second == worst[1].second)
                best = worst[0].second;
            else // worst[2] = worst[0]
                best = worst[1].second;
        }*/
    }
};

