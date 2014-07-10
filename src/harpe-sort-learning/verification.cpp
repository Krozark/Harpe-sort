#include <iostream>
#include <fstream>

#include <mgf/Driver.hpp>
#include <harpe-algo/Analyser.hpp>
#include <harpe-algo/Context.hpp>

#include <harpe-sort-learning/Spectrum.hpp>
#include <harpe-sort-learning/Entity.hpp>

#include <utils/thread.hpp>
#include <utils/log.hpp>
#include <utils/sys.hpp>

using namespace std;

#define SHOW_ARGS(x) {utils::log::info("Help",x\
    ,"\n\t -h, -help, montre ce message"\
    ,"\n\t -f mgf input file (obligatoire)"\
    ,"\n\t -l calc_score source file (obligatoire)"\
    );exit(1);\
}

int main(int argc,char* argv[])
{

    std::string mgf;
    std::string lib;

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
            else if(arg == "-l")
            {
                if(++i < argc)
                {
                    lib = argv[i];
                }
                else
                    SHOW_ARGS("Pas de fichier spécifié")
            }
            else if (arg =="-h" or arg=="-help")
                SHOW_ARGS("Aide")            
            else
                SHOW_ARGS(string(argv[i])+": Mauvais argument");
            ++i;
        }

        if (mgf == "" or lib == "")
            SHOW_ARGS("Pas de fichier mgf spécifié")
    }

    utils::sys::Compiler comp;
    comp.input(lib)
        .output("calc_score-verif")
        .flags("-o3","-Wall","-std=c++0x")
        .link("utils")
        .get();

    if (not harpe::Context::loadFromLib("./calc_score-verif.so"))
    {
        utils::log::error("Verification","loadFromLib failed");
        return 1;
    }

    //harpe::Context::error=0.05;
    //harpe::Context::finds_max_size=1000;
    //harpe::Context::finds_max_size_tmp=harpe::Context::finds_max_size*5;
    //harpe::Context::mod = harpe::Context::MOD::NORMAL;

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

    std::ifstream file(mgf, std::ifstream::in);


    if (file.good())
    {
        std::atomic<unsigned int> total(0);
        std::atomic<double> total_res(0.0);
        {
            utils::thread::Pool pool(std::thread::hardware_concurrency());

            mgf::Driver driver(file);
            mgf::Spectrum* spectrum = nullptr;
            utils::log::info("Initialisation","données d'apprentissage");
            
            unsigned int i = 0;

            while((spectrum = driver.next()) != nullptr)
            {
                utils::log::todo(i,"Spectre mis en attente");

                pool.push([i,spectrum,&total,&total_res]()->void {
                    int status = 0;
                    std::vector<harpe::SequenceToken*> token_ptr;

                    std::vector<harpe::Sequence> res = harpe::Analyser::analyse(*spectrum,token_ptr,status,-1);

                    if (status == harpe::Analyser::Status::Ok)
                    {
                        harpe::learning::Spectrum s = harpe::learning::Spectrum::convert(*spectrum,res);
                        double d = s.eval(harpe::Context::f());

                        utils::log::ok(i,"Ajout du spectre avec",d,"de réussite (sur",res.size()," spectre). Status : OK");

                        double current;
                        do{
                         current = total_res;
                        }while(not total_res.compare_exchange_weak(current, current + d));
                        total += 1;
                    }
                    else
                    {
                        utils::log::error(i,"Ajout du spectre status : Erreur <",harpe::Analyser::strErr(status),">. Merci de corriger le fichier d'entrée");
                    }
                    harpe::Analyser::free(token_ptr);
                    delete spectrum;
                    utils::log::info(i,"Fin du traitement du spectre");
                });

                ++i;
            }
            pool.wait();
        }
        double d = total_res/ total;
        utils::log::ok("Validation :",d);
    }
    else
    {
        utils::log::error("Verification","input file not valid");
    }
    file.close();


    harpe::Context::closeLib();
    return 0;
}
