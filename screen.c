/************************************************************************************************************
파일 이름: screen.c
주요 함수: int Screen(FILE *fp, unsigned int cnt, unsigned int *esc_i,wdif* word_);
주요 기능: 학습기에서 실제 영어 본문을 콘솔에 띄워주고 정오답을 알려주는 부분.
출력 인자: sco_stat에 저장될 수. (빈칸을 모두 맞추었는지 main() 제어부에서 확인하는 용도.) 
처리 개요: fp에 있는 정보를 받아온 cnt와 word를 통해서 분석하여 표시/ esc_i는 커서 위치 저장 및 불러오기 용도 
제작자: 강준구, 박남규 
************************************************************************************************************/
#include"header.h"

// Text Color Set 텍스트 색상 설정
#define RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000c);
#define BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0009);
#define GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000a);
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0007);

void InitBlank(unsigned int n); // 빈칸에 남아있는 오답의 흔적들을 제거한다. 
void Adjust(unsigned int n);	// 나중에 Screen()을 다시 불러왔을때, 이전에 입력했던 정답 오답 상황을 표시해준다. 
void mvword(int *i,int mov,unsigned int cnt); // 커서를 단어단위로 이동할 때. 
void Backspace(char* str, unsigned int* i); //Scanner에서 backspace가 눌렸을때, 커서를 이동시킴과 동시에 한글자씩 지우는 역할. 
char Scanner(char *str, unsigned int siz,int n); //빈칸에 단어를 받는 역할. 

wdif *word;
unsigned int width; //Console width 콘솔 창 너비

int Screen(FILE *fp, unsigned int cnt, unsigned int *esc_i,wdif* word_){
	char c,get[LOW];
	unsigned int tot=0;
    int i,mov=1;
    COORD Pos,set={0,0};

    CONSOLE_SCREEN_BUFFER_INFO csbi; GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	width=csbi.dwSize.X; // Backspace에서 쓸 콘솔창 너비를 받아둔다.(Backspace에서 밖에 안 쓰지만 한 글자 지우려고 함수 불러올때마다 가져오긴 부담스러움.) 
	
	word=word_; //word를 변조할 일은 없고 읽기만 하니 그냥 주소값 받아와서 잘 쓴다. 
    CLEAR
    fseek(fp,0,SEEK_SET);
   	for(i=0;(c=fgetc(fp))!=EOF;putc(c,stdout)){ // Temp에서 빈칸 뚫린 본문은 다 가져온다. 
		if(c==MARK){
			GetXY(word[i++].pos);
			c='_';
		}
	}
	GetXY(Pos); // 나중을 위해 제일 하단 좌표 기억. 
	
	for(i=cnt-1;i>=0;--i) Adjust(i); //나중에 Screen()을 다시 불러왔을때, 이전에 입력했던 정답 오답 상황을 표시해준다.
	GotoXY(set);
	//Hell Gate Opened.
    for(i=*esc_i;c!=ESC;){ // 시작 단어를 마지막으로 끝냈던 (esc_i번째) 위치로 간다.  /ESC 눌릴때까지 학습기 화면에 머무른다. 
        if( rBIT(word[i].stat,CORRECT) ) {mvword(&i,mov,cnt); continue; } // 이미 맞은 단어들은 넘어가기.  
		GotoXY(word[i].pos);				// i번째 단어 입력할꺼 맞으면 좌표 옮겨야지. 
		
        c=Scanner(get,word[i].len,i); // i번째에 뭐 입력할 건지 불러온다. 입력이 끝나고 ESC,ENTER,화살표 등등 뭘 받았는지 반환됨. 

        if(c==LEFT||c==RIGHT||c==TAB) { 			// 커서 이동 키를 받았으면, 
        	if( rBIT(word[i].stat,HINT) ) wBIT0(word[i].stat,HINT); // 힌트 봤었으면 없애고 간다. 메롱 
			Adjust(i);					// 깔끔하게 정리하고 떠난다. 
			mov= c==LEFT ? -1 : 1;		// 왼쪽 오른쪽 구분해주고
			mvword(&i,mov,cnt);			// mov에 따라서 mvword로 이동 시켜준다. 
			continue;					// 계속 해야지? 
		}
		else if(c==ENTER) {
			GotoXY(word[i].pos); // 미리 다시 쓸려고 처음위치로 가 있는다. 
        	if(strcmp(word[i].str,get)==0) { // 맞았으면 
            	GREEN puts(get); WHITE		// 초록색으로 
            	wBIT1(word[i].stat,CORRECT);
	            Adjust(i); 
				mvword(&i,(mov=1),cnt); // 다시 정리해주고 다음칸으로 이동해준다. 
	            ++tot;
			}
			else{	//틀렸으면 
				InitBlank(i); // 한번 청소하고 
    	      	RED puts(get); WHITE // 빨간색으로 틀린단어 써주기. 
        	  	strcpy(word[i].wrongstr,get); // wrongstr에 복사해주고 두고두고 기억하기 
          		wBIT1(word[i].stat,WRONG);
			}
			if(tot==cnt){ //다 초록색 됐으면, 
                ++Pos.Y;  // (그 다음줄 의미) 
                GotoXY(Pos); // 끝으로 가고 
                for(i=tot=0;i<cnt;++i) if( rBIT(word[i].stat,CORRECT) ) ++tot; // 한번에 맞춘단어 몇갠지 확인해서 
                if(tot==cnt) {puts("모두 맞히셨습니다!"); return 2;} //천재임이 증명되면 축하해주고 
                puts("정답을 모두 입력하셨습니다. 복습하시겠습니까? (y/n): "); // 그건 아니면 그냥 복습확인 
                return ((c=Ask(4,'y','Y','n','N'))=='y'||c=='Y') ? 1 : 2;  //sco_stat에 저장될 결과 반환 
            }					//sco_stat==1은 한번이상 틀린 단어가 존재해서 복습할 때, sco_stat==2는 천재이거나 복습하기 싫을때. 
		}
		else if(c==SPACE) {
            if(!rBIT(word[i].stat,WRONG)) wBIT1(word[i].stat,WRONG); // space 한번 누르면 1점은 까이고 들어간다. / res>0일때 space를 누를순 없고, res<0일땐 굳이 의미 없으니... 
            GotoXY(word[i].pos);				// 단어 시작위치로 가서 쓸 준비 
			if(word[i].stat & HINT){ //이미 힌트 활성화 되어 있는데 space 눌리면
                wBIT0(word[i].stat,HINT);	// 되돌려주고 
                InitBlank(i);	// 힌트 다시 없애고 
                RED puts(word[i].wrongstr); WHITE // 과거의 치욕을 되살려 준다. 
            }
			else{				 			// state 0: Not yet.
                wBIT1(word[i].stat,HINT); // state 1: Hint enabled.
                BLUE puts(word[i].str); WHITE // 파란색으로 정답 보여주기 
            }
            continue; //단어는 계속 입력해야지? 
		}
	}
	*esc_i=i; // 마지막에 몇번째 (i번째) 단어였는지를 (esc_i에)저장해둔다. 나중에 다시 올때 불러낸다. 
    return 0; // 다 안 맞췄으면 그냥 sco_stat=0. 
}
char Scanner(char *str, unsigned int siz,int n){
	unsigned int i=0; char c; COORD Pos;
	while(1) {
		GetXY(Pos); // 시작 위치 기억한다. 
		c=getch();	// 버퍼 없이 문자 받는다. 
		if(c<=0) return getch(); // RIGHT, LEFT 키는 2바이트 차지이고 첫 바이트는 음수라 그냥 버리고 두번째 바이트를 가지고 판별한다. 
		else if(c==BACKSPACE) Backspace(str,&i); // Backspace 구현은 조금 길어서 함수로 뺌. 
		else if(c==ESC||c==SPACE||c==TAB||c==ENTER) {str[i]='\0'; return c;} // ESC,SPACE,TAB,ENTER는 Screen()가서 처리. 문자열 끝에 '\0' 
		else if(isalpha(c)){
			if(word[n].pos.X==Pos.X && word[n].pos.Y==Pos.Y) InitBlank(n); //입력 시작했으니까 깨끗히 해주자. 
			if(0<=i&&i<siz){ // 허용 범위 내이면, 
				str[i++]=c;	// str에도 넣고 
				putchar(c);	// 출력도 한다.
			}
		}
	}
}
void Backspace(char* str, unsigned int* i){  // Backspace Function in Scanner(). Scanner함수에서 Backspace의 역할을 담당하는 함수.
	COORD Pos;
	GetXY(Pos);
	if(*i==0) return;               // 지울 칸이 없으면 그대로 둔다.
	else if(Pos.X>0) Pos.X--;       // 커서를 왼쪽으로 이동.
	else {                          // 콘솔의 왼쪽 끝에 도달하면 그 윗줄 오른쪽 끝으로 이동한다.
		if(Pos.Y<1)return;
		Pos.X=width-1;
		Pos.Y--;
	}                   
	str[--*i]='\0';			// 백스페이스로 글자길이가 한자씩 준다. / 단어의 끝은 여기가 됨.  
	GotoXY(Pos);
	putchar('_');                   // 삭제된 공간에 언더바를 삽입.
	GotoXY(Pos);                    // 커서는 원위치. 
	return;
}

void InitBlank(unsigned int n){	//Initialize Blank space in console.
    int i;
    GotoXY(word[n].pos);
    for(i=word[n].len;i!=0;--i) putc('_',stdout);
    GotoXY(word[n].pos);
}

void Adjust(unsigned int n){
	InitBlank(n);
    if( rBIT(word[n].stat,CORRECT) ) {GREEN puts(word[n].str); WHITE}
    else if ( rBIT(word[n].stat,HINT) ) {BLUE puts(word[n].str); WHITE}
    else if( rBIT(word[n].stat,WRONG) ) {RED puts(word[n].wrongstr); WHITE}
}

void mvword(int *i,int mov,unsigned int cnt){ //move location between word and other word.
    *i+=mov;						//mov에 저장된 값에 따라 i를 옮겨주는데,
    if(*i<0) *i+=cnt;				//i가 0보단 작을 수 없으니 마지막 단어로 옮긴다. 
    else if(*i>=cnt) *i-=cnt;		//마지막 단어 다음 단어는 없으니 처음으로 간다.
}//Screen()에서 굳이 mov 변수를 도입해준 것은 화살표의 방향을 기억해 주기 때문이다. (이미 정답을 맞춰 두 세단어를 건너 뛰어도 그 화살표 방향을 유지하도록.)

