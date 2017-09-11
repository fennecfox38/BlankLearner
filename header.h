/*****************************************************************************************
파일 이름은 header.h 
프로젝트 이름: BlankLearner 
제작자: 강준구, 박남규
구성 파일: main.c, blankgenerator.c, screen.c  //각자 기능에 따라 파일로 분리해 놓았다. 
*****************************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h> //isalpha() isupper()
#include<stdarg.h>
#include<conio.h>
#include<windows.h>

//커서 좌표 이동 및 받아오기
#define GotoXY(Pos) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos)
#define GetXY(Pos) {CONSOLE_SCREEN_BUFFER_INFO csbi; GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi); Pos=csbi.dwCursorPosition;}
//자주 쓰게 될 system 명령어
#define PAUSE system("pause");
#define CLEAR system("cls");

#define MARK '|'        // 빈칸이 뚫릴 단어의 시작을 표시.
#define LOW 20			// Length Of Words 단어의 길이ㅡ (글자 수)

#define rBIT(flag,indicator) (((flag)&(indicator))==(indicator))
#define wBIT1(flag,indicator) ( (flag)|=(indicator) )
#define wBIT0(flag,indicator) ( (flag)^=(indicator) )
enum {HINT=1,WRONG=2,CORRECT=4};

typedef struct{     				// wdif(wordinfo) will contain information about word blanked.
    unsigned int loc,len;
	COORD pos;
    char str[LOW],wrongstr[LOW];  	// 문자열을 저장한다.
    char stat;		// 01 : Space pressed , 02 History of Wrong Answer, 04 Corrected
} wdif;

//getch()로 받게 될 문자, 방향키, 특수자들
enum {BACKSPACE=8,TAB,ENTER=13,ESC=27,SPACE=32,F1=59,LEFT=75,RIGHT=77};

char Ask(int n,...);
void Tutorial(void);
void mvword(int *i,int mov,unsigned int cnt);
