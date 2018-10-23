#ifndef GRAMUI_H
#define GRAMUI_H

#define whiteFont_redBck  BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|BACKGROUND_INTENSITY|FOREGROUND_INTENSITY  //对标志位进行组合设置成红字白底
#define whiteFont FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY  //对标志位进行组合设置成白字红底

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<conio.h>
#include <iostream>
#include"Grammer.h"
#include"GramIO.h"
#include"GramMatch.h"
#include<string>
#include<vector>

using namespace std;
// 未完成
class MenuItem
{
public:
    MenuItem(string title,void*(*fun)(...));
    string getTitle();
};


class GramUI
{
public:
    GramUI();
    int UIMain();

private:
    int Menu();
    int MenuCall();
    void show();
    void help();

    bool issucesskey(char buffer);

    static void gotoxy(int y,int x);
    static void Hidecursor();
    static void Showcursor();

    Gram _gram;
    vector<string> menu_str;
    unsigned choose;
};

#endif // GRAMUI_H
