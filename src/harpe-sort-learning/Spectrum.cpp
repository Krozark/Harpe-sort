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
            int res = 0;
            const auto tokens = std::split(seq,"-");
            const int _size = tokens.size();
            if(real_sequences.size() <=0)
            {
                res = -_size;
            }
            else
            {
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
                    for(auto real : real_sequences)
                    {
                        for(int i=1;i<_size;++i)
                        {
                            std::vector<std::string>::const_iterator first = tokens.begin() + i;
                            std::vector<std::string>::const_iterator last = tokens.end();
                            std::vector<std::string> tmp(first, last);

                            std::string seq = std::join("-",tmp);

                            if(real.find(seq) != std::string::npos)
                            {
                                res = MAX(res,i-_size);
                            }
                        }

                        for(int i =_size -1;i>0;--i)
                        {
                            std::vector<std::string>::const_iterator first = tokens.begin() + i;
                            std::vector<std::string>::const_iterator last = tokens.end();
                            std::vector<std::string> tmp(first, last);

                            std::string seq = std::join("-",tmp);

                            if(real.find(seq) != std::string::npos)
                            {
                                res = MAX(res,i-_size);
                            }
                        }
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
