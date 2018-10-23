#include"Grammer.h"

Rule::Rule()
{
    _gram.first = '0';
    _gram.second = "";
}
Rule::Rule(char left,string right)
{
    _gram.first = left;
    _gram.second = right;
}

bool Rule::operator ==(const Rule &r)const
{
    return r.left()==left()?r.right()==right()?true:false:false;
}

bool Rule::operator <(const Rule &r)const
{
    return r.left()>left()?true:false;
}

Rule& Rule::operator =(const Rule &r)
{
    if(this != &r)
    {
        this->setleft(r.left());
        this->setright(r.right());
    }
    return *this;
}

void Rule::setleft(char s)
{
    _gram.first = s;
}

void Rule::setright(string s)
{
    _gram.second = s;
}

char Rule::left()const
{
    return _gram.first;
}

string Rule::right()const
{
    return _gram.second;
}

bool Rule::isempty()const
{
    return _gram.first=='0'?_gram.second==""?true:false:false;
}

/******************

Gram部分

*******************/
Gram::Gram()
{
    _NoEndSym = set<char>{};
    _EndSym = set<char>{};
    _StartSym = '#';
    _Rules = vector<Rule>{};
}

Gram::Gram(set<char> a,set<char> b,vector<Rule> c,char d)
{
    _NoEndSym = a;
    _EndSym = b;
    _Rules = c;
    _StartSym = d;
}

char Gram::getStartSym()const
{
    return this->_StartSym;
}

set<char> Gram::getNoEndSym()const
{
    return this->_NoEndSym;
}

set<char> Gram::getEndSym()const
{
    return this->_EndSym;
}

vector<Rule> Gram::getRule()const
{
    return this->_Rules;
}

//获取某个非终结符的所有规则
vector<Rule> Gram::getRule(char Symbol)const
{
    vector<Rule> rules;
    for(Rule var : _Rules)
    {
        if(var.left() == Symbol)
            rules.push_back(var);
    }
    return rules;
}

bool Gram::isNoEndSym(char ch)    //判断字符是否是非终结符
{
    if(ch >= 'A' && ch <='Z')
        return true;

    return false;
}
bool Gram::isEndSym(char ch)      //判断字符是否是终结符
{
    if(ch >= 'a' && ch <= 'z')
        return true;
    if(ch >= '(' && ch <= '9')
        return true;
    if(ch == '$')
        return true;

    return false;
}

char Gram::getNoEndSym(string right)const
{
    for(Rule rule : _Rules)
    {
        if(rule.right() == right)
            return rule.left();
    }
    return '#';
}

bool Gram::isempty()const
{
    return _StartSym == '#'?true:false;
}
