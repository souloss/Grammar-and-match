#ifndef GRAMIO_H
#define GRAMIO_H
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<typeinfo>
#include"GramExcept.h"
#include"Grammer.h"
#include"GramMatch.h"
#include<stdlib.h>
#include<conio.h>
#include<stack>

class GramIO{
public:
    //Input
    /*
    Gram输入
    入参:输入流
    使用传入的输入流按行读取推导式，每行形如:
    A aA
    左边为推导式左部,右边为推导式右部
    若是控制台输入,则以Ctrl+Z模拟文件结束结束输入
    */

    static Gram Input(istream &in);


    //Output
    static void OutputRule(Gram gram);      //控制台格式化输出文法的推导式
    static void OutputGram(Gram gram);      //控制台格式化输出文法
    static void OutputGramAttr(Gram gram);  //控制台格式化输出文法属性(包括First集,Follow集,Select集,文法类别等)
    static void OutputFirstvec(Gram gram);  //控制台格式化输出文法的First集列表
    static void OutputFollowvec(Gram gram); //控制台格式化输出文法的Follow集列表
    static void OutputSelectvec(Gram gram); //控制台格式化输出文法的Select集列表

    static void OutputPriTable(Gram gram);  //简单优先表
    static void OutputPreTable(Gram gram);  //预测分析表



    static std::string& trim(std::string &s);   //去字符串首尾空格

    static void showstack(stack<char> s);   //顺序显示栈内内容
};

#endif


