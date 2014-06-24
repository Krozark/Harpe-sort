#include <harpe-sort-learning/Spectrum.hpp>
#include <utils/string.hpp>
#include <utils/maths.hpp>

#include <algorithm>
#include <utility>



namespace harpe
{
    namespace learning
    {
        Spectrum Spectrum::convert(const mgf::Spectrum& src,const std::vector<harpe::Sequence>& src_seq)
        {
            return Spectrum(src,src_seq);
        }

        int Spectrum::rate(const std::string& seq)const
        {
            const auto tokens = utils::string::split(seq,"-");
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

                            std::string tok_tmp = utils::string::join("-",tmp);

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
                utils::string::replace(seq,"I_L","L"); //to be sure
                utils::string::replace(seq,"I","L"); 
                utils::string::replace(seq,"L","I_L");
                real_sequences.push_back(seq);
                seq = utils::string::join("-",utils::string::split(seq,"-"),true);
                real_sequences.push_back(std::move(seq));
            }

            propositions.reserve(src_seq.size());
            auto& self = *this;
            for(const harpe::Sequence& s : src_seq)
                propositions.emplace_back(Sequence(self,s));

            //sort
            sort();
            //truncate (take head and tail, delete middle)
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
