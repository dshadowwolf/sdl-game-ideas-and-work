#ifndef __GENERIC_EXCEPTION_HPP__
#include <stdexcept>

class GenericException: public std::runtime_error
{
    public:
        GenericException(std::string const& msg):
            std::runtime_error(msg)
        {}
};

#define __GENERIC_EXCEPTION_HPP__
#endif
