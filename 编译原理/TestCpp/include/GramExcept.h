#ifndef GRAMEXCEPT_H
#define GRAMEXCEPT_H
#include<exception>
#include<string>

class GramExcept:public std::exception{
private:
    std::string _msg;
public:
    GramExcept(const std::string msg);
    virtual const char* what()const noexcept;
};



#endif
