#include <harpe-sort-learning/Spectrum.hpp>
#include <harpe-sort-learning/std.hpp>

#define MAX(x,y) (x>y?x:y)

namespace harpe
{
    namespace learning
    {
        Spectrum Spectrum::convert(const mgf::Spectrum& src,std::vector<harpe::Sequence>& src_seq)
        {
            return Spectrum(src,src_seq);
        }

        int Spectrum::rate(const std::string& seq)const
        {
            const auto tokens = std::split(seq,"-");
            const int _size = tokens.size();

            int res = -_size;

            for(auto real : real_sequences)
            {
                if(real.find(seq) != std::string::npos)//la solution est valide
                {
                    int tmp = _size;//nombre de AA
                    res=MAX(res,tmp);
                }
            }

            if(res == 0)
            {
                //search for a sub seqence
                for(auto real : real_sequences)
                {
                    for(int i=1;i<_size;++i)
                    {
                        std::vector<std::string>::const_iterator first = tokens.begin() + i;
                        std::vector<std::string>::const_iterator last = tokens.end();
                        std::vector<std::string> tmp(first, last);

                        std::string tok_tmp = std::join("-",tmp);

                        if(real.find(tok_tmp) != std::string::npos)
                            res = MAX(res,-i);
                    }

                    for(int i =_size -1;i>0;--i)
                    {
                        std::vector<std::string>::const_iterator first = tokens.begin() + i;
                        std::vector<std::string>::const_iterator last = tokens.end();
                        std::vector<std::string> tmp(first, last);

                        std::string tok_tmp = std::join("-",tmp);

                        if(real.find(tok_tmp) != std::string::npos)
                            res = MAX(res,i-_size);
                    }
                }
            }

            return res;
        }

        Spectrum::Spectrum(const mgf::Spectrum& src,std::vector<harpe::Sequence>& src_seq)
        {
            for(const std::string& s : src.getHeader().getSeq())
                real_sequences.push_back(s);

            auto& self = *this;
            for(const harpe::Sequence& s : src_seq)
                propositions.emplace_back(Sequence(self,s));
            
        }
    }
}
