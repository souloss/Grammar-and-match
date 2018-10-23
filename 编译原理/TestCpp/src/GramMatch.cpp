#include"GramMatch.h"

//传入参数:初始产生式的规则集合，需要匹配的字符串
//流程:
//传入 文法，规则串，待匹配串
//如果待匹配串匹配到规则串的大写字母则将大写字母正确展开
//如果待匹配串匹配到规则串的小写字母则成功就步进，失败则返回false
bool GramMatch::TD_match(Gram gram,string match,const char *str)
{
    cout<<match<<"\t"<<str<<endl;
    if((match.size()==0 || match == "$") && *str == '\0')
        return true;
    for(char ch : match)
    {
        if(Gram::isNoEndSym(ch))
        {
            bool hasnull = false;
            for(Rule r : gram.getRule(ch))  //展开规则串,寻找正确的路径
            {
               for(char r_ch : get_first(gram,r))
               {
                   if(r_ch == *str) // 重匹配展开后的规则串
                   {
                       match.erase(0,1);
                       match.insert(0,r.right());
                       cout<<"展开推导式:"<<r.left()<<"->"<<r.right()<<endl;
                       return TD_match(gram,match,str);
                   }
               }
               set<char> temp = get_first(gram,r);
               if(temp.find('$') != temp.end())
                    hasnull = true;
            }
            if(hasnull)     //以空代替非终结符
            {
                match.erase(0,1);
                match.insert(0,"$");
                cout<<"展开推导式"<<ch<<"->$"<<endl;
                return TD_match(gram,match,str);
            }
            else
                break;  //所有展开式都不满足，退出
        }
        else if(Gram::isEndSym(ch))
        {
            if(*str == ch)
            {
                match.erase(0,1);
                str++;
                cout<<"步进"<<endl;
                return TD_match(gram,match,str);
            }
            else
                return false;
        }
        else if(ch == '$')
        {
            if(*str=='\0')
            {
                return true;
            }
            else
                continue;
        }
    }

    return false;
}

//内部辅助函数,用于寻预测分析表
Rule GramMatch::Pretable(Gram gram,char NoEndSym,char EndSym)
{
    const vector<pair<Rule,set<char>>> selectvec = get_selectvec(gram);
    for(pair<Rule,set<char>> select : selectvec)
    {
        if(select.first.left() == NoEndSym)
        {
            if(select.second.find(EndSym) != select.second.end()) //该集中有该符号
            {
                return select.first;
            }
        }
    }

    return Rule();
}
//内部函数,自顶向下-预测表分析法
bool GramMatch::Pretable_match(Gram gram,const char *str)
{
    char *temp = (char*)malloc(strlen(str)+2);
    strcpy(temp,str);
    str = strcat(temp,"#"); // 将原始字符串后面自动接上 #

    stack<char> mstack;  //匹配栈
    mstack.push('#');
    mstack.push(gram.getStartSym());

    GramIO::showstack(mstack);
    cout<<"\t匹配\t";
    cout<<"\t"<<str<<endl;

    while(!mstack.empty())
    {
        if(Gram::isNoEndSym(mstack.top()))
        {
            Rule rule = Pretable(gram,mstack.top(),*str);
            if(rule.isempty())
            {
                return false;
            }

            mstack.pop();
            for(int i = rule.right().size()-1; i >= 0 ;i--)
            {
                if(rule.right()[i] == '$')
                    continue;
                mstack.push(rule.right()[i]);
            }

                GramIO::showstack(mstack);
                cout<<"\t预测"<<rule.left()<<"->"<<rule.right()<<"\t";
                cout<<"\t"<<str<<endl;
        }
        else if(Gram::isEndSym(mstack.top()))
        {
            if(*str == mstack.top())
            {
                mstack.pop();
                str++;

                GramIO::showstack(mstack);
                cout<<"\t步进\t";
                cout<<"\t"<<str<<endl;

            }
            else
            {
                return false;
            }
        }
        else if(mstack.top() == '#')
        {
            if(*str == mstack.top())
            {
                mstack.pop();
                str++;

                GramIO::showstack(mstack);
                cout<<"\t"<<str<<endl;

            }
            else
                return false;
        }
    }

    return true;
}

// 自底向上的匹配方法
bool GramMatch::DT_match(Gram gram,const char *str)
{
    char *temp = (char*)malloc(strlen(str)+2);
    strcpy(temp,str);
    str = strcat(temp,"#"); // 将原始字符串后面自动接上 #

    stack<char> mstack;
    mstack.push('#');

    GramIO::showstack(mstack);
    cout<<"\t匹配\t";
    cout<<"\t"<<str<<endl;

    while(true)
    {
        if(Priority(gram,mstack.top(),*str) == '>')  //规约
        {
            if(Priority(gram,mstack.top(),'$') == '<')
            {
                mstack.push(gram.getNoEndSym("$"));
                GramIO::showstack(mstack);
                cout<<"\t规约:"<<"$"<<"->"<<gram.getNoEndSym("$");
                cout<<"\t"<<str<<endl;
                continue;
            }
            string cur_str = "";         // 用来存储当前规约串
            char temp;                  // 临时字符
            temp = mstack.top();

            cur_str.insert(0,1,temp);
            mstack.pop();
            while(Priority(gram,mstack.top(),cur_str[0]) != '<')
            {
                cur_str.insert(0,1,mstack.top());
                mstack.pop();
            }
            temp = gram.getNoEndSym(cur_str);  //获取左部

            if(Gram::isNoEndSym(temp))
            {
                mstack.push(temp);

                GramIO::showstack(mstack);
                cout<<"\t规约:"<<cur_str<<"->"<<temp;
                cout<<"\t"<<str<<endl;

                if(*str == '#' && mstack.size() == 2 && mstack.top() == gram.getStartSym())
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            mstack.push(*str);  //移进
            str++;

            GramIO::showstack(mstack);
            cout<<"\t移进";
            cout<<"\t"<<str<<endl;
        }
    }
}


//优先级比较,比较文法中任意两个符号的优先级
char GramMatch::Priority(Gram gram,char A,char B)
{
    map<pair<char,char>,char> PTable = Priority_Table(gram);
    if(A == '#' && B!='#')
        return '<';
    else if(B == '#' && A!='#')
        return '>';
    else if(A == '#' && B == '#')
        return '=';

    pair<char,char> ret(A,B);
    if(PTable.find(ret) == PTable.end())
        return ' ';
    else
        return PTable[ret];
}

//用于自底向上匹配，寻找文法中非终结符推导式可能出现的头部符号，如果传入的是终结符则返回自身的集合。
set<char> GramMatch::SymHead(Gram gram,char start)
{
    set<char> ret;
    if(Gram::isNoEndSym(start))
    {
        stack<char> left_stack;
        left_stack.push(start);
        while(!left_stack.empty())
        {
            char temp = left_stack.top();
            left_stack.pop();
            for(Rule rule : gram.getRule(temp))
            {
                if(Gram::isNoEndSym(rule.right()[0]) && ret.find(rule.right()[0]) == ret.end())
                    left_stack.push(rule.right()[0]);

                ret.insert(rule.right()[0]);
            }
        }
    }
    else if(Gram::isEndSym(start))
    {
        ret.insert(start);
    }

    return ret;
}

//用于自底向上匹配，寻找文法中非终结符的推导式的可能出现的最后一个符号，如果传入的是终结符则返回自身的集合
set<char> GramMatch::Symtail(Gram gram,char start)
{
    set<char> ret;
    if(Gram::isNoEndSym(start))
    {
        stack<char> left_stack;
        left_stack.push(start);
        while(!left_stack.empty())
        {
            char temp = left_stack.top();
            left_stack.pop();
            for(Rule rule : gram.getRule(temp))
            {
                const unsigned lastindex = rule.right().size()-1;
                if(Gram::isNoEndSym(rule.right()[lastindex]) && ret.find(rule.right()[lastindex]) == ret.end())
                {
                    left_stack.push(rule.right()[lastindex]);
                }
                ret.insert(rule.right()[lastindex]);
            }
        }
    }
    else if(Gram::isEndSym(start))
    {
        ret.insert(start);
    }

    return ret;
}

// 获取单条规则的First集
set<char> GramMatch::get_first(Gram gram,Rule rule)   // wait....
{
  //  cout<<"求"<<rule.left()<<"->"<<rule.right()<<endl;
    set<char> ret;
    if(Gram::isEndSym(rule.right()[0]))
        ret.insert(rule.right()[0]);
    else if(Gram::isNoEndSym(rule.right()[0]))
    {
        unsigned int index = 0;
        for(char ch : rule.right())
        {
            if(Gram::isNoEndSym(ch))
            {
                if(ch == rule.left())   //左递归
                    continue;
                for(Rule rule : gram.getRule(ch))
                {
                    set<char> temp = get_first(gram,rule);
                    ret.insert(temp.begin(),temp.end());
                }
                if(ret.find('$') == ret.end()) //若非终结符的First集中没有 空，则该规则的First集就是当前的ret
                {
                    break;
                }
                else if(index != rule.right().size()-1) //若,非终结符不是最后一个并且有 空 则删除该 空，并且遍历该规则下一个字符
                {
                    ret.erase(ret.find('$'));
                }
            }
            else if(Gram::isEndSym(ch))
            {
                ret.insert(ch);
                break;
            }
            index++;
        }
    }
    else if(rule.right()=="$")
    {
        ret.insert('$');
    }

    return ret;
}

// 获取单个非终结符的Follow集

//这里要重写
//流程：
// 如果入参是开始产生式 则 set 中只包含 $
// 否则，遍历所有产生式
// 若有 A -> αBβ , 并且First(β)中没有{$}则First(β)属于Follow(B)，否则First(β)和Follow(A)都属于Follow(B)
// 若有 A ->αB 则 Follow(A) 属于 Follow(B)
set<char> GramMatch::get_follow(Gram gram,char left)
{
    set<char> ret;
    if(left == gram.getStartSym())
        ret.insert('#');

    for(Rule rule : gram.getRule())
    {
        unsigned int index = 0;
        for(char ch : rule.right())
        {
//       cout<<"当前求"<<left<<"遍历到"<<rule.right()<<","<<rule.right().size()<<","<<index<<endl;
            if(ch == left && index != rule.right().size()-1)
            {
                if(Gram::isEndSym(rule.right()[index+1]))
                {
                    ret.insert(rule.right()[index+1]);
                    break;
                }
                else if(Gram::isNoEndSym(rule.right()[index+1]))
                {
                    for(Rule frule : gram.getRule(rule.right()[index+1]))
                    {
                        set<char> temp = get_first(gram,frule);
                        ret.insert(temp.begin(),temp.end());
                    }
                    if(ret.find('$') != ret.end())
                    {
                        ret.erase('$');
                        if(index+1 == rule.right().size()-1) //最后一个非终结符
                        {
                            set<char> temp = get_follow(gram,rule.left());
                            ret.insert(temp.begin(),temp.end());
                        }
                    }
                    else
                        break;
                }
            }
            else if(ch == left && index == rule.right().size()-1)
            {
                if(rule.left() == left)   //防止右递归
                  continue;
                set<char> temp = get_follow(gram,rule.left());
                ret.insert(temp.begin(),temp.end());
            }
            index++;
        }
    }

    return ret;
}

bool GramMatch::match_precheck(const char *str)
{
    while(*str != '\0')
    {
        if(Gram::isEndSym(*str))
        {
            str++;
        }
        else
            return false;
    }
    return true;
}
/**********  以上是private部分,以下是public部分**************/
/*
将该文法中每条规则的First集放入vector中
*/
vector<pair<Rule,set<char>>> GramMatch::get_firstvec(Gram gram)
{
    vector<pair<Rule,set<char>>> ret;
    for(Rule rule : gram.getRule())
    {
        pair<Rule,set<char>> cur;
        set<char> cur_set;
        cur.first = rule;
        cur_set = get_first(gram,rule);
        cur.second = cur_set;
        ret.push_back(cur);
    }
    return ret;
}

vector<pair<char,set<char>>> GramMatch::get_followvec(Gram gram)
{
    vector<pair<char,set<char>>> ret;
    for(char ch: gram.getNoEndSym())
    {
//        cout<<"进行到"<<ch<<endl;
        pair<char,set<char>> cur;
        set<char> cur_set;
        cur.first = ch;
        cur_set = get_follow(gram,ch);
        cur.second = cur_set;
        ret.push_back(cur);
    }
    return ret;
}

vector<pair<Rule,set<char>>> GramMatch::get_selectvec(Gram gram)
{
//  cout<<"in get_selectvec()"<<endl;
    vector<pair<Rule,set<char>>> ret;
    for(Rule rule : gram.getRule())
    {
//      cout<<"get_select:"<<rule.left()<<"->"<<rule.right()<<endl;
        pair<Rule,set<char>> cur_pair;
        cur_pair.first = rule;

        set<char> first = get_first(gram,rule);
        if(first.find('$') != first.end())  //First集存在空
        {
            set<char> follow = get_follow(gram,rule.left());
            cur_pair.second = first;
            cur_pair.second.erase('$');
            cur_pair.second.insert(follow.begin(),follow.end());
        }
        else    //First集不存在空
        {
            cur_pair.second = first;
        }

        ret.push_back(cur_pair);
    }
    return ret;
}

//判断是否符合LL1
bool GramMatch::LL1_check(Gram gram)
{
    if(LeftRecursion_check(gram))  //左递归不是LL1
        return false;
//    cout<<"in LL1_check()"<<endl;
    vector<pair<Rule,set<char>>> select = get_selectvec(gram);
    for(char NoEndSym : gram.getNoEndSym())
    {
//        cout<<"LL1_check:"<<NoEndSym<<endl;
        unsigned int count = 0;
        set<char> Nset;
        for(pair<Rule,set<char>> p : select)
        {
            if(p.first.left() == NoEndSym)
            {
                count += p.second.size();
                Nset.insert(p.second.begin(),p.second.end());
            }
        }
        if(count != Nset.size())
            return false;
    }

    return true;
}

// 判断是否符合左递归
bool GramMatch::LeftRecursion_check(Gram gram)
{
    for(Rule rule : gram.getRule())
    {
        for(char ch : rule.right())
        {
            if(Gram::isNoEndSym(ch))
            {
                if(ch == rule.left())
                    return true;   //是左递归
                set<char> temp;
                for(Rule Irule : gram.getRule(ch))
                {
                    set<char> Itemp = get_first(gram,Irule);
                    temp.insert(Itemp.begin(),Itemp.end());
                }
                if(temp.find('$') == temp.end())
                    break;
                else
                    continue;
            }
            else if(Gram::isEndSym(ch))
            {
                break;
            }
        }
    }

    return false;
}


// 判断是否符合简单优先规则
bool GramMatch::SimPri_check(Gram gram)
{
    try{
        Priority_Table(gram);
        unsigned count = 0;
        set<string> right;
        for(Rule rule : gram.getRule())
        {
            count++;
            right.insert(rule.right());
        }
        if(count != right.size())
            return false;
        else
            return true;
    }catch(GramExcept ex)
    {
        return false;
    }
}


//判断是否符合算符规则
bool GramMatch::OPG_check(Gram gram)
{
    return false;
}

// 用于自底向上匹配，根据文法推导式求优先级关系
map<pair<char,char>,char> GramMatch::Priority_Table(Gram gram)
{
    map<pair<char,char>,char> ret;
    for(Rule rule : gram.getRule())  // 从每个推导式中寻找优先关系
    {
//        cout<<"遍历到"<<rule.left()<<"->"<<rule.right()<<endl;
        if(rule.right().size() < 2)
            continue;

        for(unsigned i = 0; i <= rule.right().size()-2; i++) //每次遍历比较一次
        {
            /*******  等于关系 ***************/
            pair<char,char> eq_pair(rule.right()[i],rule.right()[i+1]);
            if(ret.find(eq_pair) == ret.end())  //第一次添加该文法的优先级关系
                ret[eq_pair] = '=';
            else
            {
                if(ret[eq_pair] != '=')  //如果出现两种优先级关系则抛出异常
                    throw GramExcept("该文法不是简单优先文法");
            }
          //cout<<"等于关系"<<eq_pair.first<<"="<<eq_pair.second<<endl;

            /******* 小于关系 **************/
            if(Gram::isNoEndSym(rule.right()[i+1]))
            {
               set<char> lt_set = SymHead(gram,rule.right()[i+1]);
               for(char ch : lt_set)
               {
                    pair<char,char> lt_pair(rule.right()[i],ch);
                    if(ret.find(lt_pair) == ret.end())
                        ret[lt_pair] = '<';
                    else
                    {
                        if(ret[lt_pair] != '<')
                            throw GramExcept("该文法不是简单优先文法");
                    }
                //cout<<"小于关系"<<lt_pair.first<<"<"<<lt_pair.second<<endl;
               }
            }

            /******* 大于关系 ***********/
            if(Gram::isNoEndSym(rule.right()[i]))
            {
                set<char> gt_left_set = Symtail(gram,rule.right()[i]);
                set<char> gt_right_set = SymHead(gram,rule.right()[i+1]);
                for(char ch : gt_left_set)
                {
                    for(char inch : gt_right_set)
                    {
                        pair<char,char> gt_pair(ch,inch);
                        if(ret.find(gt_pair) == ret.end())
                            ret[gt_pair] = '>';
                        else
                        {
                            if(ret[gt_pair] != '>')
                                throw GramExcept("该文法不是简单优先文法");
                        }
                       //cout<<"大于关系"<<gt_pair.first<<">"<<gt_pair.second<<endl;
                    }
                }
            }
        }
    }

    return ret;
}

/****
  自顶向下文法匹配，要求传入文法以及匹配串
  文法必须是LL1文法
****/
void GramMatch::topTodown(Gram gram,const char *str,int type)
{
    if(LL1_check(gram) && match_precheck(str))
    {
        bool sucess = false;
        if(type == 0)
        {
            char temp[2] = {gram.getStartSym(),0};
            if(TD_match(gram,string(temp),str))
                sucess = true;
        }
        else if(type == 1)
        {
            if(Pretable_match(gram,str))
                sucess = true;
        }
        if(sucess)
            cout<<"匹配成功"<<endl;
        else
            cout<<"匹配失败"<<endl;
    }
    else
    {
        throw GramExcept("该文发不是LL1文法或匹配串输入有误,无法使用该方法匹配");
    }
}

/****
  自底向上文法匹配，要求传入文法以及匹配串
  文法必须是 简单优先文法
****/
void GramMatch::downTotop(Gram gram,const char *str)
{
    if(SimPri_check(gram) && match_precheck(str))
    {
        if(DT_match(gram,str))
            cout<<"匹配成功"<<endl;
        else
            cout<<"匹配失败"<<endl;
    }
    else
    {
        throw GramExcept("该文发不是简单优先文法或匹配串输入有误,无法使用该方法匹配");
    }
}

