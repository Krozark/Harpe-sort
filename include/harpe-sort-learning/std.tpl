#include <sstream>

namespace std
{
    template<typename T>
    std::string join(const std::string& del,const std::vector<T>& datas)
    {
        std::stringstream ss;
        const unsigned int _size = datas.size();
        if(_size > 0)
            ss<<datas[0];
        for(unsigned int i=1;i<_size;++i)
            ss<<del<<datas[i];
        return ss.str();
    }
}

