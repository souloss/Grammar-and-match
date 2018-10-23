#ifndef GRAMMER_H
#define GRAMMER_H

#include"GramExcept.h"
#include<iostream>
#include<set>
#include<vector>
using namespace std;


/******
推导式类
    使用char存储左部,使用string存储右部，构造该类
    的对象后可以直接提取推导式的内容,一般作为Gram
    类的一部分使用使用
    GramIO辅助类还可以以特别的的方式输出该文法
******/
class Rule{
private:
    /*
    存储原始推导式
    由于使用pair<char,string>，所以只能存储二型，三型文法推导式
    */
    pair<char,string> _gram;
public:
    /*
    默认构造函数，用于构造空推导式
    默认空推导式存储规则为 0->""
    */
    Rule();
    /*
    构造函数，用于构造推导式
    入参：推导式左部，推导式右部
    */
    Rule(char left,string right);

    //用于判断两个推导式是否相等
    bool operator ==(const Rule &r)const;

    //用于判断推导式左部ascii符大小
    bool operator <(const Rule &r)const;

    Rule& operator =(const Rule &r);

    // 设置推导式左部
    void setleft(char s);
    // 设置推导式右部
    void setright(string s);
    // 返回推导式左部
    char left()const;
    // 返回推导式右部
    string right()const;
    // 判断推导式是否为'空'
    bool isempty()const;
};

/******
文法类
    使用char存储非终结符和终结符,构造该类的对象后
    可以直接判断该文法的属性,提取文法的内容.使用
    GramIO辅助类还可以以特别的的方式输出该文法
******/
class Gram{
private:
    set<char> _NoEndSym;   //非终结符集合
    set<char> _EndSym;     //终结符集合
    vector<Rule> _Rules;   //推导式集合
    char _StartSym;        //起始符

public:
    Gram();
    Gram(set<char> a,set<char> b,vector<Rule> c,char d); //Gram构造函数

    char getStartSym()const;                //获取文法的起始符
    set<char> getNoEndSym()const;           //获取文法的非终结符集合
    set<char> getEndSym()const;             //获取文法的终结符集合
    vector<Rule> getRule()const;            //获取文法的全部推导式
    vector<Rule> getRule(char Symbol)const; //获取某个非终结符的所有推导式

    static bool isNoEndSym(char ch);    //判断字符是否是非终结符,这里只做范围判定
    static bool isEndSym(char ch);      //判断字符是否是终结符,这里只做范围判定

    char getNoEndSym(string right)const;    //根据推导式的右部获取其左部,如果该右部没有左部则返回 #
    bool isempty()const;                    //判断文法逻辑上是否为空
};


#endif
