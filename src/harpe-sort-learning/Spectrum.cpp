#include <harpe-sort-learning/Spectrum.hpp>
#include <harpe-sort-learning/std.hpp>

#include <algorithm>
#include <utility>


#define MAX(x,y) (((x)>(y))?(x):(y))
#define MIN(x,y) (((x)<(y))?(x):(y))

namespace harpe
{
    namespace learning
    {
        Spectrum Spectrum::convert(const mgf::Spectrum& src,const std::vector<harpe::Sequence>& src_seq)
        {
            return Spectrum(src,src_seq);
        }

        int Spectrum::eval(const Entity& entity)const
        {
            const int _size = propositions.size();
            std::vector<std::pair<const Sequence *,double>> data(_size);

            for(int i=0;i<_size;++i)
            {
                data[i].first = &propositions[i];
                data[i].second = propositions[i].eval(entity);
            }

            std::sort(data.begin(),
                      data.end(),
                      [](const std::pair<const Sequence*,double> _1,const std::pair<const Sequence*,double> _2){
                          return _1.second > _2.second;
                      });

            int res = 0;
            for(int i=0;i<_size and i < 100;++i)
            {
                if(propositions[i].sequence == data[i].first->sequence
                   and std::sign(propositions[i].real_score) == std::sign(data[i].second))
                    res += 1;
                else
                    break;
            }
            return res;//\todo TODO

        }

        int Spectrum::rate(const std::string& seq)const
        {
            const auto tokens = std::split(seq,"-");
            const int _size = tokens.size();

            int res = -_size;

            for(auto real : real_sequences)
            {
                if(real.find(seq) != std::string::npos)//la solution es
                    res = _size;
            }

            if(res <= 0)
            {
                //search for a sub seqence
                for(auto real : real_sequences)
                {
                    for(int i=0;i<_size and res < -i;++i)
                    {
                        for(int j=0;j<_size-i and res < -i -j;++j)
                        {
                            std::vector<std::string>::const_iterator first = tokens.begin() + i;
                            std::vector<std::string>::const_iterator last = tokens.end() - j;
                            std::vector<std::string> tmp(first, last);

                            std::string tok_tmp = std::join("-",tmp);

                            if(real.find(tok_tmp) != std::string::npos)
                            {
                                res = -i -j;
                                break;
                            }
                        }
                    }
                }
            }
            return res;
        }

        Spectrum::Spectrum(const mgf::Spectrum& src,const std::vector<harpe::Sequence>& src_seq)
        {
            for(const std::string& s : src.getHeader().getSeq())
            {
                std::string seq = s;
                std::replace(seq,"I_L","L"); //to be sure
                std::replace(seq,"I","L"); 
                std::replace(seq,"L","I_L");
                real_sequences.push_back(seq);
                seq = join("-",std::split(seq,"-"),true);
                real_sequences.push_back(std::move(seq));
            }

            propositions.reserve(src_seq.size());
            auto& self = *this;
            for(const harpe::Sequence& s : src_seq)
                propositions.emplace_back(Sequence(self,s));

            //sort
            sort();
            //truncate
            //\todo TODO
        }

        std::ostream& operator<<(std::ostream& output,const Spectrum& self)
        {
            output<<"[real_sequences] size : "<<self.real_sequences.size()<<std::endl;
            for(const std::string& s : self.real_sequences)
                output<<s<<std::endl;

            output<<"[proposition] size : "<<self.propositions.size()<<std::endl;
            for(const Sequence& s : self.propositions)
                output<<s<<std::endl;

            return output;
        }

        void Spectrum::sort()
        {
            std::sort(propositions.begin(),
                      propositions.end(),
                      [](const Sequence& _1,const Sequence& _2){
                          return _1.real_score > _2.real_score;
                      });
            
        }
    }
}
