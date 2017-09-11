/*************************************************************************************************************
파일 이름: main.c
주요 함수: int main(int argc, char *argv[]);
주요 기능: BlankLearner의 제어부분으로서, BlankGenerator()와 Screen()을 제어하여 학습기를 구현. 
처리 개요: 뚫린 빈칸을 불러와서 screen()으로 학습기를 구현하되, Menu()로 다양한 기능을 실행할 수 있도록 제어. 
제작자: 강준구, 박남규 
*************************************************************************************************************/
#include"header.h"

void Information(void); char Menu(unsigned int cnt); // Menu()는 Screen()을 나온 후 어떤 기능을 실행할지 입력받을 함수. 
void StarLine(unsigned int i);
void Review(FILE *fp,unsigned int *pcnt,unsigned int *esc_i); // 복습 모드로 셋팅해주는 함수. 

extern int Screen(FILE *fp,unsigned int cnt, unsigned int *esc_i,wdif* word_); // 콘솔 창에 본문 텍스트를 불러들여와 출력해주는 함수 
extern int BlankGenerator(FILE *fp,unsigned int* pcnt,wdif** pword); // 본문 파일을 읽어 빈칸을 생성해주는 함수 
extern void custom_blank(FILE *fp,unsigned int* pcnt,wdif** pword); // 수동으로 사용자가 빈칸을 생성하도록 돕는 함수. 

wdif *word;
unsigned int sco_stat=0; // Blanked word count 뚫린 빈칸 카운트 

int main(int argc, char *argv[]){
    FILE *fp,*fp_db;
    unsigned int cnt=0,esc_i=0;
    char c=0;

    Information(); // 안내사항 및 도움말. 

    if((fp=fopen("Temp\\Temp.txt","r+"))==NULL)	{							// 만약 이전에 쓰던 Temp.txt가 없으면  
    	system("mkdir Temp"); system("copy /Y File\\Article.txt Temp\\Temp.txt"); //Article에서 새로 불러오고 
		fp=fopen("Temp\\Temp.txt","r+");
		if(BlankGenerator(fp,&cnt,&word)==-1) return -1;					//빈칸 뚫음.
	}
	else{											// Temp.txt를 성공적으로 불려오면 
		fp_db=fopen("Temp\\wddat.dat","rb");		// wddat.dat으로부터 
		fread(&cnt,sizeof(unsigned int),1,fp_db);	// cnt를 읽어오고, 
		word=(wdif*)calloc(sizeof(wdif),cnt);
		fseek(fp_db,5,SEEK_SET);
		fread(word,sizeof(wdif),cnt,fp_db);			// word[cnt]를 가져온다.
		fclose(fp_db);
	}
	
	if( (c=Screen(fp,cnt,&esc_i,word)) == 1 ) Review(fp,&cnt,&esc_i); 	// 답을 다 맞추면 복습할지 여부를 물어봐서 복습할 거임. 
    else if(c==2) goto END; 								// 단어를 틀리지 않고 모두 맞았거나 복습을 거부했을 땐 그냥 종료시킨다. 
	
	while((c=Menu(cnt))!=ENTER){					//Menu()에서 ENTER가 눌리면 프로그램 종료 
        if(c=='f'||c=='F'){
            CLEAR fputs("빈도 수 입력 (1~5의 정수): ",stdout); // 빈칸의 빈도수를 다시 설정. 
            c=(Ask(5,'1','2','3','4','5')-'0');
            fp_db=fopen("Temp\\userdata.txt","r+");
            fprintf(fp_db,"%d",c);
            fclose(fp_db);
			c='g'; 					//빈도수가 변경되면 다시 빈칸을 뚫어야 적용되므로 빈칸 재생성... 
        }
        if(c=='g'||c=='G'){			//빈칸을 재 생성. 
            fclose(fp);
            system("copy /Y File\\Article.txt Temp\\Temp.txt");  //Article.txt로부터 다시 불러들여와서 
            fp=fopen("Temp\\Temp.txt","r+");
            if(BlankGenerator(fp,&cnt,&word)==-1) return -1; // 빈칸 뚫기 
            esc_i=0; // 커서 위치도 기본 위치로 초기화 
        }
        else if(c=='c'||c=='C'){
        	fclose(fp);
            system("copy /Y File\\Article.txt Temp\\Temp.txt");  //Article.txt로부터 다시 불러들여와서 
            fp=fopen("Temp\\Temp.txt","r+");
            
        	custom_blank(fp,&cnt,&word);
        	esc_i=0;
		}
        else if(c=='p'||c=='P'){ // 뚫린 빈칸이 마음에 들면 텍스트 파일로 출력한다. 
            fclose(fp);
            CLEAR
            system("copy /Y Temp\\Temp.txt File\\Output.txt"); PAUSE
            fp=fopen("File\\Output.txt","r+");
            while((c=fgetc(fp))!=EOF)
                if(c==MARK) {				//Temp.txt에는 단어의 시작점을 알기위해 MARK '|'를 사용했는데 
                    fseek(fp,-1,SEEK_CUR);
                    putc('_',fp);			//출력해줄 땐 보기 흉하고 필요도 없으니, 없애버린다. 
                    fseek(fp,0,SEEK_CUR);
                }
            puts("\nOutput.txt를 확인해주세요.");
            fclose(fp);
            fp=fopen("Temp\\Temp.txt","r+");
            PAUSE
            continue; // 다시 메뉴로 돌아가기. 
        }
        else if(c=='r'||c=='R') Review(fp,&cnt,&esc_i); 							// 복습모드. Review()로 셋팅. 
        else if(c==F1) { Tutorial(); continue; } 									// 도움말 보기. 도움말 본 이후론 다시 메뉴료 돌아간다. 
        if( (c=Screen(fp,cnt,&esc_i,word)) == 1 ) Review(fp,&cnt,&esc_i); 	// 답을 다 맞추면 복습할지 여부를 물어봐서 복습할 거임. 
        else if(c==2) goto END; 								// 단어를 틀리지 않고 모두 맞았거나 복습을 거부했을 땐 그냥 종료시킨다. 
    }
    END:
    free(word); fclose(fp);
    puts("\n프로그램을 종료합니다."); PAUSE
	return 0;
}

void Review(FILE *fp,unsigned int* pcnt,unsigned int* esc_i){
    unsigned int i,count;
    wdif *nword; //new word info
    sco_stat=*esc_i=0;
    nword=(wdif*)calloc(sizeof(wdif),1);
    for(i=count=0;i<*pcnt;++i){
        if( rBIT(word[i].stat,CORRECT) && !rBIT(word[i].stat,WRONG) ){					// 한번에 맞은 단어들은 빈칸을 뚫어 다시 문제낼 필요가 없어서 
            fseek(fp,word[i].loc,SEEK_SET); // 그냥 Temp.txt의 빈칸을 채워넣는다. 
            fputs(word[i].str,fp);
        }
        else{								// 한번이라도 틀렸거나 힌트를 본 단어라면, 
            nword[count].loc=word[i].loc;	// word[cnt]의 정보를 복사한다. 
            nword[count].len=word[i].len;
            nword[count].pos=word[i].pos;
            strcpy(nword[count].str,word[i].str);
            nword[count].stat=nword[count].wrongstr[0]=0;
            nword=(wdif*)realloc(nword,sizeof(wdif)*(++count+1));
        }
    }
    *pcnt=count;
    free(word); 									// 기존의 word는 소멸시키고 nword의 값을 word에. 
    word=nword; CLEAR
	printf("복습할 %d개의 단어들:\n",count); 		//복습할 단어들도 표시하고 
    for(i=0;i<count;++i)
        printf("%s\n",word[i].str);
    printf(":복습할 %d개의 단어들\n",count);
    
    FILE *fp_db=fopen("Temp\\wddat.dat","wb"); 		// wddat에 word 구조체의 정보도 넣는다. 
	fseek(fp_db,0,SEEK_SET);
	fwrite(&count,sizeof(unsigned int),1,fp_db);
	fseek(fp_db,5,SEEK_SET);
	fwrite(word,sizeof(wdif),count,fp_db);
	fclose(fp_db);
    
    PAUSE return;
}

char Menu(unsigned int cnt){
    unsigned int i,cor,fix;
    if(sco_stat==2) return ENTER;
	CLEAR
	puts("[메뉴(현재 창)에서 창 크기 조절이 가능합니다.]\n[창 크기 조절 중 글자 배열이 이상해질 수 있습니다만 신경쓰시지 마시고 계속해주세요.]");
	puts("본문 파일이 바뀌게 되면 반드시 다시 빈칸 재생성(G)을 먼저 해야 합니다.\n");
	for(i=cor=fix=0;i<cnt;++i){
        if( rBIT(word[i].stat,WRONG) ) ++fix;
        else if( rBIT(word[i].stat,CORRECT) ) ++cor;
    }
    printf("맞은 갯수 %d개, 수정한 갯수 %d개, 틀린 단어 또는 미제출 갯수 %d개\n",cor,fix,cnt-cor-fix);

    fputs("\n메뉴(ESC: 학습기, ENTER: 종료, F1: 도움말, C: 사용자 지정 빈칸 F: 빈도 설정, G: 빈칸 재생성, P: 텍스트 파일 출력, R: 복습): ",stdout);
    return Ask(13,ESC,ENTER,F1,'c','C','f','F','g','G','p','P','r','R');
}

void Tutorial(void) {
    CLEAR puts("[학습창에서는 창 크기 조절이 불가능합니다.]");
	printf("%7s : %15s\n%7s : %15s\n", "←", "왼쪽 단어로 이동","TAB, →", "오른쪽 단어로 이동");
    printf("%7s : %15s\n%7s : %15s\n%7s : %15s\n", "ENTER", "채점", "SPACE", "힌트 On/Off", "ESC", "메뉴"); PAUSE }

void Information(void){
	CLEAR StarLine(80);
    puts("Blank Learner 1.0\n\n\t제작 : 강준구, 박남규\n\n<사용법> BlankLearner.exe와 같은 경로에 File폴더를 만든 뒤 다음 파일들을 폴더 안에 저장해 주세요.");
    puts("Article.txt에 본문을 붙여넣으세요.\nException.txt에 절대 빈칸화 시키지 않을 단어를 입력하세요.\nVocaList.txt에 무조건 빈칸화 시킬 단어를 입력하세요.(대소문자 구분)");
    StarLine(80); PAUSE Tutorial();}

void StarLine(unsigned int i){ for(; i!=0; --i) putchar('*');	putchar('\n');} // i개의 별을 한 줄에  출력한다. 

char Ask(int n,...){					// 가변 인자함수이며, 변수 n은 뒤의 가변인자의 갯수를 받는다. 
    char c; unsigned int i;
    va_list ap;
    va_start(ap,n);
    while(1){							//getch()로 키보드로부터 매개변수로 입력받은 문자가 입력될 때까지 계속 입력받음. 
        va_start(ap,n);
        c=getch();
        if(c<=0) c=getch();
        for(i=n;i!=0;--i)
            if(c==(char)va_arg(ap,int)){
                va_end(ap);
                putchar(c);
                return c;				// 매개변수로 입력받은 문자가 걸리면 그 문자를 반환. 
            }
        va_end(ap);
    }
}
