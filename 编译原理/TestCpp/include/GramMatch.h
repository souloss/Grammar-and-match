#ifndef MATCHGRAM_H
#define MATCHGRAM_H
#include"Grammer.h"
#include"GramExcept.h"
#include"GramIO.h"
#include<vector>
#include<stack>
#include<map>
#include<cstring>
#include<stdlib.h>
#include <algorithm>

using namespace std;
class GramMatch
{
private:
    //内部函数,自顶向下-朴素匹配法.
    //传入参数:初始产生式的规则集合，需要匹配的字符串
    static bool TD_match(Gram gram,string match,const char *str);

    //内部函数,自顶向下-预测表匹配法
    static bool Pretable_match(Gram gram,const char *str);

    //内部函数,自底向上简单优先法匹配.
    static bool DT_match(Gram gram,const char *str);


    static char Priority(Gram gram,char A,char B);   //辅助函数,优先级比较,比较文法中任意两个符号的优先级

    static set<char> SymHead(Gram gram,char start);  //用于辅助自底向上匹配,获取某符号的推导式头部符号集合,如果符号是终结符,则返回自身的集合
    static set<char> Symtail(Gram gram,char start);  //用于辅助自底向上匹配,获取某符号的推导式尾部符号集合,如果符号是终结符,则返回自身的集合

    static set<char> get_first(Gram gram,Rule rule);     //辅助函数,获取文法中某个推导式的First集
    static set<char> get_follow(Gram gram,char left);    //辅助函数,获取文法中某个非终结符的Follow集

    static bool match_precheck(const char *str);  //匹配前检查
public:
    static bool LL1_check(Gram gram);           //LL1文法检查
    static bool LeftRecursion_check(Gram gram); //左递归检查
    static bool SimPri_check(Gram gram);        //简单优先文法检查
    static bool OPG_check(Gram gram);  //算符文法检查

    static vector<pair<Rule,set<char>>> get_firstvec(Gram gram);  // 每条推导式的first集的vector
    static vector<pair<char,set<char>>> get_followvec(Gram gram); // 每个非终结符的follow集的vector
    static vector<pair<Rule,set<char>>> get_selectvec(Gram gram); // 每条推导式的select集的vector
    static map<pair<char,char>,char> Priority_Table(Gram gram);   //遍历文法,求优先级表,并保存在map中
    static Rule Pretable(Gram gram,char NoEndSym,char EndSym);    //寻预测分析表

    static void topTodown(Gram gram,const char *str,int type);  //自顶向下匹配
    static void downTotop(Gram gram,const char *str);           //自底向上匹配
};

#endif
