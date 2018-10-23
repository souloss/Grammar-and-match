#include"GramIO.h"

Gram GramIO::Input(istream &in)
{
    system("cls");
     if(typeid(in) == typeid(cin))
        cout<<"请输入文法:"<<endl;
    if(in.peek() == -1)
    {
        throw GramExcept("文件读取失败");
    }
    vector<Rule> rules;
    string buffer;
    string start;
    set<char> NoEndSym;
    set<char> EndSym;
    while(getline(in,buffer))  //循环输入规则，每行一条
    {
        buffer = trim(buffer);
        if(buffer.substr(0,buffer.find(' ')).size() != 1)
            throw GramExcept("推导式左部读取异常!");
        rules.push_back(Rule(buffer.substr(0,buffer.find(' '))[0],buffer.substr(buffer.find(' ')+1,buffer.size())));

        for(char ch : buffer)
        {
            if(Gram::isNoEndSym(ch))
                NoEndSym.insert(ch);
            else if(Gram::isEndSym(ch))
                EndSym.insert(ch);
        }
    }

    start = rules[0].left();  //第一个非终结符为起始符

    if(!Gram::isNoEndSym(rules[0].left()))
        throw GramExcept("文法规则起始符读取异常!");

    Gram gram(NoEndSym,EndSym,rules,start[0]);
    cout<<"读取成功!..按任意键返回"<<endl;
    getch();
    return gram;
}

void GramIO::OutputRule(Gram gram)
{
    for(Rule rule: gram.getRule())
    {
        cout<<rule.left()<<"->"<<rule.right()<<endl;
    }
}

void GramIO::OutputGram(Gram gram)
{
    system("cls");
    if(gram.isempty())
    {
        cout<<"当前文法为空,请输入后再查看!"<<endl;
        getch();
        return;
    }
    cout<<"G["<<gram.getStartSym()<<"] = {"<<endl;
    cout<<"{";
    for(char ch:gram.getNoEndSym())
    {
        cout<<ch<<",";
    }
    if(gram.getNoEndSym().empty())
        cout<<"}"<<endl;
    else
        cout<<"\b}"<<endl;

    for(char ch:gram.getEndSym())
    {
        cout<<ch<<",";
    }
    if(gram.getEndSym().empty())
        cout<<"},"<<endl<<"{"<<endl;
    else
        cout<<"\b},"<<endl<<"{"<<endl;

    OutputRule(gram);
    cout<<"},"<<endl<<gram.getStartSym()<<" }"<<endl;
    getch();
}

void GramIO::OutputGramAttr(Gram gram)
{
    system("cls");
    if(gram.isempty())
    {
        cout<<"当前文法为空,请输入后再查看!"<<endl;
        getch();
        return;
    }
    OutputFirstvec(gram);
    OutputFollowvec(gram);
    OutputSelectvec(gram);
    OutputPriTable(gram);
    OutputPreTable(gram);
    cout<<endl<<"属性关键字:";
    if(GramMatch::LeftRecursion_check(gram))
        cout<<" 左递归";
    if(GramMatch::LL1_check(gram))
        cout<<" LL1文法";
    if(GramMatch::SimPri_check(gram))
        cout<<" 简单优先文法";
    if(GramMatch::OPG_check(gram))
        cout<<" 算符文法";
    cout<<endl;
    cout<<"..按任意键返回"<<endl;
    getch();
}


void GramIO::OutputFirstvec(Gram gram)
{
    for(pair<Rule,set<char>> cur_pair : GramMatch::get_firstvec(gram))
    {
      cout<<"First("<<cur_pair.first.left()<<"->"<<cur_pair.first.right()<<")={";
      for(char ch:cur_pair.second)
      {
          cout<<ch<<",";
      }
      if(cur_pair.second.empty())
        cout<<"}"<<endl;
      else
      cout<<"\b}"<<endl;
    }
}

void GramIO::OutputFollowvec(Gram gram)
{
    for(pair<char,set<char>> cur_pair : GramMatch::get_followvec(gram))
    {
      cout<<"Follow("<<cur_pair.first<<")={";
      for(char ch:cur_pair.second)
      {
          cout<<ch<<",";
      }
      if(cur_pair.second.empty())
        cout<<"}"<<endl;
      else
      cout<<"\b}"<<endl;
    }
}

void GramIO::OutputSelectvec(Gram gram)
{
    for(pair<Rule,set<char>> cur_pair : GramMatch::get_selectvec(gram))
    {
      cout<<"Select("<<cur_pair.first.left()<<"->"<<cur_pair.first.right()<<")={";
      for(char ch:cur_pair.second)
      {
          cout<<ch<<",";
      }
      if(cur_pair.second.empty())
        cout<<"}"<<endl;
      else
      cout<<"\b}"<<endl;
    }
}

void GramIO::OutputPriTable(Gram gram)  //简单优先表
{
    map<pair<char,char>,char> table = GramMatch::Priority_Table(gram);
    cout<<endl<<"简单优先表:"<<endl;
    vector<char> symvec;
    for(char ch : gram.getEndSym())
        symvec.push_back(ch);
    for(char ch : gram.getNoEndSym())
        symvec.push_back(ch);

    for(char rowch : symvec)  //打印首行
        cout<<"\t"<<rowch;
    cout<<endl;

    for(char colch : symvec)
    {
        cout<<colch;
        for(char rowch : symvec)
        {
            pair<char,char> temp_pair(colch,rowch);
            if(table.find(temp_pair) == table.end())
                cout<<"\t ";
            else
                cout<<"\t"<<table[temp_pair];
        }
        cout<<endl;
    }
}

void GramIO::OutputPreTable(Gram gram)  //预测分析表
{
    cout<<endl<<"预测分析表:"<<endl;
    for(char Endch : gram.getEndSym())
        cout<<"\t"<<Endch;
    cout<<endl;
    for(char NoEndch : gram.getNoEndSym())
    {
        cout<<NoEndch;
        for(char Endch : gram.getEndSym())
        {
            Rule rule = GramMatch::Pretable(gram,NoEndch,Endch);
            if(rule.isempty())
                cout<<"\t ";
            else
                cout<<"\t->"<<rule.right();
        }
        cout<<endl;
    }
}

std::string& GramIO::trim(std::string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void GramIO::showstack(stack<char> s)
{
    stack<char> temp;
    while(!s.empty())
    {
        temp.push(s.top());
        s.pop();
    }
    while(!temp.empty())
    {
        cout<<temp.top();
        temp.pop();
    }
}



