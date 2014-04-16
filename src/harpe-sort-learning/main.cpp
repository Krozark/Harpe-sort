#include <iostream>
#include <fstream>

#include <mgf/Driver.hpp>
#include <harpe-algo/Analyser.hpp>
#include <harpe-algo/Context.hpp>

int main(int argc,char* argv[])
{
    if (not harpe::Context::loadFromLib("./calc_score.so")) //just to be ok, even if not used
        return 1;

    harpe::Context::error=0.05;
    //harpe::Context::finds_max_size=1000; set to inf
    //harpe::Context::finds_max_size_tmp=1000*5; to inf

    harpe::Context::aa_tab.add(0,"A",71.037110);
    harpe::Context::aa_tab.add(1,"C",103.009185);
    harpe::Context::aa_tab.add(2,"D",115.026943);
    harpe::Context::aa_tab.add(3,"E",129.042593);
    harpe::Context::aa_tab.add(4,"F",147.068414);
    harpe::Context::aa_tab.add(5,"G",57.021464);
    harpe::Context::aa_tab.add(6,"H",137.058912);
    harpe::Context::aa_tab.add(7,"I-L",113.084064);
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

    int r=0;
    std::ifstream file(argv[1], std::ifstream::in);

    if (file.good())
    {

        mgf::Driver driver(file);
        mgf::Spectrum* spectrum = nullptr;
        int status = 0;
        while((spectrum = driver.next()) != nullptr)
        {
            spectrum->__print__(std::cout);
            std::vector<harpe::Sequence> res = harpe::Analyser::analyse(*spectrum,status,-1);
            std::cout<<"status: "<<status<<std::endl;
            //harpe::learning::Spectrum parent = harpe::learning::Spectrum::convert(*spectrum);
            //std::vector<harpe::learning::Sequence> learning = harpe::learning::Sequence::convert(parent,res);
            harpe::Analyser::free();
            delete spectrum;
        }
        file.close();

    }
    harpe::Context::closeLib();
    return r;
}
