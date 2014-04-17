#include <harpe-sort-learning/std.hpp>

namespace std
{
    std::vector<std::string> split(const std::string& string, const std::string& del)
    {
        size_t first = 0,
               second = 0,
               end = string.size(),
               len = del.size();
        std::vector<std::string> tokens;

        while ((second = string.find(del,first)) != (std::string::npos))
        {
            size_t dif = second - first;
            if (dif)
                tokens.push_back(string.substr(first,dif));
            first = second + len;
        }
        if (first != end)
            tokens.push_back(string.substr(first));
        return tokens;
    }

    void replace(string& source, string const& find, string const& replace)
    {
        for(std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
        {
            source.replace(i, find.length(), replace);
            i += replace.length() - find.length() + 1;
        }
    }
}
