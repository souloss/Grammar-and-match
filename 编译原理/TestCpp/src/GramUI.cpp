#include "GramUI.h"

GramUI::GramUI()
{
    choose = 1;
    menu_str =
    {
        "1.从控制台输入文法",
        "2.从文件输入文法",
        "3.显示当前文法定义",
        "4.显示当前文法属性",
        "5.自顶向下-朴素法匹配字符串",
        "6.自顶向下-预测表法匹配字符串",
        "7.自底向上-简单优先匹配字符串",
        "8.帮助",
        "9.退出"
    };
}

int GramUI::UIMain()
{
    Menu();
    return 0;
}

int GramUI::Menu()
{
    while(true)
    {
        show();
        char buffer = getch();
        if(!issucesskey(buffer))
        {
            buffer = getch();
        }
        switch(buffer)
        {
        case 'w':
        case 'W':
        case 72:
        {
            if(choose > 1)
                choose--;
            else
                choose = menu_str.size();
            break;
        }
        case 's':
        case 'S':
        case 80:
        {
            if(choose < menu_str.size())
                choose++;
            else
                choose = 1;
            break;
        }
        case 13:  //回车
        {
            try{
                MenuCall();
            }catch(GramExcept e)
            {
                cout<<e.what();
                getch();
            }
        }
        default:
            cin.clear();
            break;
        }
    }
}


/***
        "1.从控制台输入文法",
        "2.从文件输入文法",
        "3.显示当前文法定义",
        "4.显示当前文法属性",
        "5.自顶向下-朴素法匹配字符串",
        "6.自顶向下-预测表法匹配字符串",
        "7.自底向上-简单优先匹配字符串",
        "8.帮助",
        "9.退出"
***/
int GramUI::MenuCall()
{
    Showcursor();
    switch(choose)
    {
        case 1:
        {
            _gram = GramIO::Input(cin);
            break;
        }
        case 2:
        {
            ifstream in;		  //输入流指针in
            in.open("file.txt");  //以输出的方式打开文件 file.txt
            _gram = GramIO::Input(in);
            break;
        }
        case 3:
        {
            GramIO::OutputGram(_gram);
            break;
        }
        case 4:
        {
            GramIO::OutputGramAttr(_gram);
            break;
        }
        case 5:
        {
            system("cls");
            cout<<"请输入字符串:";
            char *temp = (char*)malloc(1024);
            cin>>temp;
            cout<<"进入自顶向下-朴素法匹配过程:"<<endl;
            GramMatch::topTodown(_gram,temp,0);
            getch();
            free(temp);
            break;
        }
         case 6:
        {
            system("cls");
            cout<<"请输入字符串:";
            char *temp = (char*)malloc(1024);
            cin>>temp;
            GramMatch::topTodown(_gram,temp,1);
            getch();
            free(temp);
            break;
        }
         case 7:
        {
            system("cls");
            cout<<"请输入字符串:";
            char *temp = (char*)malloc(1024);
            cin>>temp;
            GramMatch::downTotop(_gram,temp);
            getch();
            free(temp);
            break;
        }
         case 8:
        {
            help();
            break;
        }
        case 9:
        {
            exit(0);
            break;
        }
        default:
            cin.clear();
    }
    return 0;
}

void GramUI::show()
{
    Hidecursor();
    system("cls");
    unsigned row = 3;
    unsigned col = 15;
    unsigned i = 1;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),whiteFont);//设置白字黑背景

    for(string cur_str : menu_str)
    {
        gotoxy(row,col);
        if(i == choose)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),whiteFont_redBck);//设置白字红背景
        cout<<cur_str<<endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),whiteFont);//设置白字黑背景
        i++;
        row+=2;
    }
}

void GramUI::help()
{
    int row = 5;
    vector<string> help_str =
    {
        "1.推导式中的空由'$'替代",
        "2.每行输入一条推导式,左部和右部由空格隔开",
        "3.由文件尾字符决定输入的结束(文件尾字符，在命令行输入中由Ctrl+Z模拟该字符且该字符应该输入在列头)",
        "4.以第一个规则的左部作为起始产生式.",
        "5.非终结符只能为大写字母，终结符只能为小写字母和算术符号以及数字",
        "6.菜单由上下键或者W/S键控制功能选择，回车键进入功能",
        "7.从文件输入的作用功能默认从项目/程序同目录文件'file'.txt中读取"
    };
    system("cls");
    gotoxy(3,20);
    cout<<"帮      助"<<endl;
    for(string str : help_str)
    {
        gotoxy(row,10);
        cout<<str;
        row++;
    }

    getch();
}

bool GramUI::issucesskey(char buffer)
{
    string SucessKey = "WwSsPH\r\340";
    for(char ch : SucessKey)
    {
        if(buffer == ch)
            return true;
    }

    return false;
}


void GramUI::gotoxy(int y,int x)
{
    COORD pos;
    pos.X=x;
    pos.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void GramUI::Hidecursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = false; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}

void GramUI::Showcursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = true;
    SetConsoleCursorInfo(handle, &CursorInfo);
}
