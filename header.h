/*****************************************************************************************
���� �̸��� header.h 
������Ʈ �̸�: BlankLearner 
������: ���ر�, �ڳ���
���� ����: main.c, blankgenerator.c, screen.c  //���� ��ɿ� ���� ���Ϸ� �и��� ���Ҵ�. 
*****************************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h> //isalpha() isupper()
#include<stdarg.h>
#include<conio.h>
#include<windows.h>

//Ŀ�� ��ǥ �̵� �� �޾ƿ���
#define GotoXY(Pos) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos)
#define GetXY(Pos) {CONSOLE_SCREEN_BUFFER_INFO csbi; GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi); Pos=csbi.dwCursorPosition;}
//���� ���� �� system ��ɾ�
#define PAUSE system("pause");
#define CLEAR system("cls");

#define MARK '|'        // ��ĭ�� �ո� �ܾ��� ������ ǥ��.
#define LOW 20			// Length Of Words �ܾ��� ���̤� (���� ��)

#define rBIT(flag,indicator) (((flag)&(indicator))==(indicator))
#define wBIT1(flag,indicator) ( (flag)|=(indicator) )
#define wBIT0(flag,indicator) ( (flag)^=(indicator) )
enum {HINT=1,WRONG=2,CORRECT=4};

typedef struct{     				// wdif(wordinfo) will contain information about word blanked.
    unsigned int loc,len;
	COORD pos;
    char str[LOW],wrongstr[LOW];  	// ���ڿ��� �����Ѵ�.
    char stat;		// 01 : Space pressed , 02 History of Wrong Answer, 04 Corrected
} wdif;

//getch()�� �ް� �� ����, ����Ű, Ư���ڵ�
enum {BACKSPACE=8,TAB,ENTER=13,ESC=27,SPACE=32,F1=59,LEFT=75,RIGHT=77};

char Ask(int n,...);
void Tutorial(void);
void mvword(int *i,int mov,unsigned int cnt);
