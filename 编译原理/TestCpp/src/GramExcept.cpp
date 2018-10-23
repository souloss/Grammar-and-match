#include"GramExcept.h"

GramExcept::GramExcept(const std::string msg):exception()
{
    _msg = msg;
}

const char* GramExcept::what()const noexcept
{
    return _msg.c_str();
}
