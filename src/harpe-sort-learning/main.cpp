#include <iostream>

#include <vector>
#include <utility>
#include <fstream>
//#include <cmath>
#include <chrono>

#include <mgf/Driver.hpp>
#include <harpe-algo/Analyser.hpp>
#include <harpe-algo/Context.hpp>

#include <harpe-sort-learning/Spectrum.hpp>
#include <harpe-sort-learning/Entity.hpp>

#include <GeneticEngine/random.hpp>
#include <GeneticEngine/GeneticEngine.hpp>

#include <utils/thread.hpp>
#include <utils/log.hpp>
#include <utils/maths.hpp>
#include <utils/sys.hpp>
#include <utils/string.hpp>
#include <utils/plot.hpp>

using namespace std;

#define SHOW_ARGS(x) {cout<<x<<endl\
    <<"\t -h, -help, montre ce message"<<endl\
    <<"\t -f mgf input file (obligatoire)"<<endl\
    <<"\t -t mgf input test file (optional) you can add multiple files names using \",\" or space as separator"<<endl\
    <<"\t -pop-total (defaut = 1000) population"<<endl\
    <<"\t -pop-enf (defaut = 0.8) .population d'enfants. if value is [0~1[,precent of [pop-total], else the number"<<endl\
    <<"\t -mutation (defaut = 1 %) [entre 0 et 100]) taux de mutation"<<endl\
    <<"\t -prefix prefix du nom de fichier de log (default = calc_score)"<<endl\
    <<"\t -create (defaut = tournament) [stupid/tournament] creation mode"<<endl\
    <<"\t -delete (defaut = tournament) [stupid/tournament] delete mode delete mode"<<endl\
    <<"\t -eval (default = 0) [1/0] always eval new"<<endl\
    <<"\t -threads (defaut = -1) [-1 pour le max possible] nombre de thread à utiliser"<<endl\
    <<"\t -max (default = 90) [0~100] score moyen à obtenir (en pourcentage de réussite)"<<endl\
    <<"\t -timeout (default = 300000) in ms timeout pour la diffusion des meilleurs individus"<<endl\
    ;exit(1);\
}

double _max = 90;

utils::plot::Gnuplot graph;
std::vector<std::list<harpe::learning::Spectrum>> learning_spectums_test;
std::vector<std::string> files;
std::mutex learning_mutex;
int nb_threads;


void replot() //<redessine le graph
{
    static std::chrono::system_clock::time_point begin;
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    if(now - begin < std::chrono::seconds(5))
        return;
    begin = now;
    graph.draw();
}

//initialise les datas
bool calc_file(const std::string& filename,const std::string& type,std::list<harpe::learning::Spectrum>& spectums,bool need_discetize)
{
    std::ifstream file(filename, std::ifstream::in);
    if(not file.good())
        return false;

    unsigned int i = 1;
    long unsigned int total=0;
    {
        utils::thread::Pool pool(nb_threads);

        mgf::Driver driver(file);
        mgf::Spectrum* spectrum = nullptr;
        utils::log::ok("Initialisation","données",type);

        while((spectrum = driver.next()) != nullptr)
        {
            utils::log::todo("Learning",i,"Spectre mis en attente");

            pool.push([i,spectrum,&total,&type,&spectums,need_discetize]()->void {
                      int status = 0;
                      std::vector<harpe::SequenceToken*> token_ptr;

                      utils::log::info(type,i,"Debut du traitement du spectre");

                      std::vector<harpe::Sequence> res = harpe::Analyser::analyse(*spectrum,token_ptr,status,-1);

                      if (status == harpe::Analyser::Status::Ok)
                      {
                          //convert for learning
                          size_t size = res.size();
                          while(size > 4638 and need_discetize) /// in wors case, produce a result of 2000
                          {
                              res = utils::maths::discretize(res,[size](double a)->double{ //do not touch the limits
                                 return 1.0/utils::maths::ker::gaussian(a,0.55);
                             });
                              utils::log::ok(type,i,"Discrétisation des données.Passage de",size,"à",res.size(),"propositions");
                              size = res.size();
                          }

                          harpe::learning::Spectrum tmp = harpe::learning::Spectrum::convert(*spectrum,res);
                          learning_mutex.lock();
                          spectums.push_back(std::move(tmp));
                          total += res.size();
                          learning_mutex.unlock();

                          utils::log::ok(type,i,"Ajout du spectre avec",res.size(),"proposition. Status : OK");
                      }
                      else if(status == harpe::Analyser::Status::LearningTooMuchFindsError)
                      {
                          utils::log::warning(type,i,"Trop de propositions sont possible (>",harpe::Context::finds_max_size,"), le spectre n'est donc pas pris en compte pour des raisons de performances");
                      }
                      else
                      {
                          utils::log::error(type,i,"Ajout du spectre status : Erreur <",harpe::Analyser::strErr(status),">. Merci de corriger le fichier d'entrée");
                      }
                      harpe::Analyser::free(token_ptr);
                      delete spectrum;
                      utils::log::info(type,i,"Fin du traitement du spectre");
            });

            ++i;
        }

        pool.wait();
    }// pool destructor here
    utils::log::ok(type,"Fin Initialisation des données d'apprentissage");
    utils::log::info(type,"Total Spectres initiaux",i-1,", propositions:",total);
    utils::log::info(type,"Total Spectres pris en compte",spectums.size()," propositions ratio : ",double(total)/spectums.size());
    
    file.close();
    return true;
}

int main(int argc,char* argv[])
{
    if (not harpe::Context::loadFromLib("./libempty_sort.so")) //just to be ok, even if not used
        return 1;

    int pop_size = 1000;
    float pop_child = 0.8;
    float mutation_taux = 1;
    std::string filename= "calc_sort.cpp";
    nb_threads = std::thread::hardware_concurrency();
    string creation = "tournament";
    string del = "tournament";
    bool eval = false;
    std::string mgf;
    std::string mgf_test;
    int timeout = 30000;

    {
        int i=1;
        while(i<argc)
        {
            string arg = string(argv[i]);
            if(arg == "-f")
            {
                if(++i < argc)
                {
                    mgf = argv[i];
                }
                else
                    SHOW_ARGS("Pas de fichier spécifié")
            }
            else if(arg == "-t")
            {
                if(++i < argc)
                {
                    mgf_test = argv[i];
                    while(i+1 < argc and argv[i+1][0]!='-')
                    {
                        mgf_test+=",";
                        mgf_test+=+argv[++i];
                    }
                }
                else
                    SHOW_ARGS("Pas de fichier spécifié")
            }
            else if (arg =="-pop-total")
            {
                if(++i <argc)
                {
                    pop_size = atoi(argv[i]);
                    if(pop_size <0)
                        SHOW_ARGS("Pas de population négative possible")
                }
                else
                    SHOW_ARGS("Pas de population de précisée")
            }
            else if (arg == "-pop-enf")
            {
                if(++i <argc)
                {
                    pop_child = atof(argv[i]);
                    if(pop_child <0)
                        SHOW_ARGS("Pas de population négative possible")
                }
                else
                    SHOW_ARGS("Pas de population de précisée")
            }
            else if(arg == "-mutation")
            {
                if(++i <argc)
                {
                    mutation_taux = atoi(argv[i]);
                    if(mutation_taux <0 or mutation_taux > 100)
                        SHOW_ARGS("Taux de mutation mauvais")

                }
                else
                    SHOW_ARGS("Pas de mutation de précisée")
            }
            else if (arg == "-prefix")
            {
                if(++i <argc)
                {
                    filename = argv[i];
                }
                else
                    SHOW_ARGS("Pas de préfix de précisé")
            }
            else if(arg == "-threads")
            {
                if(++i < argc)
                {
                    nb_threads = atoi(argv[i]);
                }
                else
                    SHOW_ARGS("Pas de nombre de précisé");
            }
            else if (arg =="-h" or arg=="-help")
                SHOW_ARGS("Aide")            
            else if(arg == "-create")
            {
                if(++i < argc)
                {
                    creation = argv[i];
                }
                else
                    SHOW_ARGS("Pas de mode de précisé");
            }
            else if(arg == "-delete")
            {
               if(++i < argc)
               {
                   del = argv[i];
               }
               else
                   SHOW_ARGS("Pas de mode de précisé");
            }
            else if (arg == "-eval")
            {
                if(++i < argc)
                {
                    eval = (atoi(argv[i])==1);
                }
                else
                    SHOW_ARGS("Pas de nombre de précisé");
            }
            else if(arg == "-max")
            {
                if(++i < argc)
                {
                    _max = atoi(argv[i]);
                }
                else
                    SHOW_ARGS("Pas de nombre précisé")
            }
            else if(arg == "-timeout")
            {
                if(++i < argc)
                {
                    timeout = atoi(argv[i]);
                }
                else
                    SHOW_ARGS("Pas de nombre précisé")
            }
            else
                SHOW_ARGS(string(argv[i])+": Mauvais argument");
            ++i;
        }

        mutation_taux /=100;
        _max /=100;
        if (pop_child < 1)
            pop_child = pop_child*pop_size;

        if (mgf == "")
            SHOW_ARGS("Pas de fichier mgf spécifié")
    }

    files = utils::string::split(mgf_test,",");
    learning_spectums_test.resize(files.size());

    cout<<"Aguments: "
    <<"\n learning: "<<mgf
    <<"\n verification: "<<mgf_test
    <<"\n pop-size: "<<pop_size
    <<"\n pop-enf: "<< (int)pop_child
    <<"\n mutation: "<<mutation_taux*100
    <<"\n prefix: "<<filename
    <<"\n create: "<<creation
    <<"\n delelte: "<<del
    <<"\n eval: "<<eval
    <<"\n threads: "<<nb_threads
    <<"\n max: "<<_max*100
    <<"\n timeout: "<<timeout
    <<endl;


    harpe::Context::error=0.05;
    harpe::Context::finds_max_size=100000;
    harpe::Context::mod = harpe::Context::MOD::LEARNING;
    harpe::Context::finds_max_size_tmp=harpe::Context::finds_max_size*5;

    harpe::Context::aa_tab.add(0,"A",71.037110);
    harpe::Context::aa_tab.add(1,"C",103.009185);
    harpe::Context::aa_tab.add(2,"D",115.026943);
    harpe::Context::aa_tab.add(3,"E",129.042593);
    harpe::Context::aa_tab.add(4,"F",147.068414);
    harpe::Context::aa_tab.add(5,"G",57.021464);
    harpe::Context::aa_tab.add(6,"H",137.058912);
    harpe::Context::aa_tab.add(7,"I_L",113.084064);
    harpe::Context::aa_tab.add(8,"K",128.094963);
    harpe::Context::aa_tab.add(9,"M",131.040485);
    harpe::Context::aa_tab.add(10,"N",114.042927);
    harpe::Context::aa_tab.add(11,"P",97.052764);
    harpe::Context::aa_tab.add(12,"Q",128.058578);
    harpe::Context::aa_tab.add(13,"R",156.101111);
    harpe::Context::aa_tab.add(14,"S",87.032028);
    harpe::Context::aa_tab.add(15,"T",101.047679);
    harpe::Context::aa_tab.add(16,"V",99.068414);
    harpe::Context::aa_tab.add(17,"W",186.079313);
    harpe::Context::aa_tab.add(18,"Y",163.063320);


    harpe::Context::aa_tab.sort();

    if(calc_file(mgf,"Learning",harpe::learning::Entity::learning_spectums,true))
    {
        const unsigned int _size = files.size();
        for(unsigned int f=0;f<_size;++f)
        calc_file(files[f],"Validation "+std::to_string(f+1)+"/"+std::to_string(f+1)+" : "+files[f],learning_spectums_test[f],true);

        utils::sys::dir::create("plot");
        for(int i=0; i<nb_threads;++i)
        {
            graph.add("island "+std::to_string(i));
            graph[i].add("Learning-"+mgf);
            graph[i][0].addPoint(0,0);
            graph[i][0].close();

            const unsigned int _size = files.size();
            for(unsigned int f=0;f<_size;++f)
            {
                graph[i].add("Validation-"+files[f]);
                graph[i][f+1].addPoint(0,0);
                graph[i][f+1].close();
            }
        }

        rand_init();
        harpe::learning::Entity::Node::max_indice = harpe::Sequence::Stats::SIZE;

        utils::sys::dir::create("best");

        GeneticEngine<harpe::learning::Entity> engine(nb_threads,mutation_taux,filename,pop_size,pop_child,TREE_INIT_PROONDEUR);
        engine.setTimeout(timeout);
        engine.setEvaluateAll(eval);



        bool (*stop)(const harpe::learning::Entity&,int,int id) = [](const harpe::learning::Entity& best,int generation,int id) -> bool
        {
            bool res = best.get_score() > _max; //tant qu'on a pas _max% de réussite

            graph[id][0].open();
            graph[id][0].addPoint(generation,best.get_score());
            graph[id][0].close();

            const unsigned int _size = learning_spectums_test.size();
            for(unsigned int f=0;f<_size;++f)
            {
                unsigned int _size = learning_spectums_test[f].size();

                if(_size>0)// tests
                {
                    double sum = 0;
                    for(harpe::learning::Spectrum& s : learning_spectums_test[f])
                    {
                        sum += s.eval(best);
                    }
                    sum/=_size;
                    utils::log::info("Validation","Validation de",best.get_score(),"donne",sum,"de réusite sur",files[f]);

                    graph[id][f+1].open();
                    graph[id][f+1].addPoint(generation,sum);
                    graph[id][f+1].close();
                }
            }
            replot();
            return res;
        };

        if (creation == "tournament")
            engine.setCreationMode(GeneticEngine<harpe::learning::Entity>::CreationMode::TOURNAMENT);
        else
            engine.setCreationMode(GeneticEngine<harpe::learning::Entity>::CreationMode::STUPIDE);

        if(del == "tournament")
            engine.setReductionMode(GeneticEngine<harpe::learning::Entity>::ReductionMode::TOURNAMENT);
        else
            engine.setReductionMode(GeneticEngine<harpe::learning::Entity>::ReductionMode::STUPIDE);

        utils::log::info("Initialisation","moteur génétique");
        harpe::learning::Entity* best = engine.run_while(stop);

        utils::log::info("best",*best);
        delete best;
    }
    else
    {
        utils::log::error("Learning","input file not valid");
    }


    harpe::Context::closeLib();
    return 0;
}
