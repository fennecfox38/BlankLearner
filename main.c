/*************************************************************************************************************
���� �̸�: main.c
�ֿ� �Լ�: int main(int argc, char *argv[]);
�ֿ� ���: BlankLearner�� ����κ����μ�, BlankGenerator()�� Screen()�� �����Ͽ� �н��⸦ ����. 
ó�� ����: �ո� ��ĭ�� �ҷ��ͼ� screen()���� �н��⸦ �����ϵ�, Menu()�� �پ��� ����� ������ �� �ֵ��� ����. 
������: ���ر�, �ڳ��� 
*************************************************************************************************************/
#include"header.h"

void Information(void); char Menu(unsigned int cnt); // Menu()�� Screen()�� ���� �� � ����� �������� �Է¹��� �Լ�. 
void StarLine(unsigned int i);
void Review(FILE *fp,unsigned int *pcnt,unsigned int *esc_i); // ���� ���� �������ִ� �Լ�. 

extern int Screen(FILE *fp,unsigned int cnt, unsigned int *esc_i,wdif* word_); // �ܼ� â�� ���� �ؽ�Ʈ�� �ҷ��鿩�� ������ִ� �Լ� 
extern int BlankGenerator(FILE *fp,unsigned int* pcnt,wdif** pword); // ���� ������ �о� ��ĭ�� �������ִ� �Լ� 
extern void custom_blank(FILE *fp,unsigned int* pcnt,wdif** pword); // �������� ����ڰ� ��ĭ�� �����ϵ��� ���� �Լ�. 

wdif *word;
unsigned int sco_stat=0; // Blanked word count �ո� ��ĭ ī��Ʈ 

int main(int argc, char *argv[]){
    FILE *fp,*fp_db;
    unsigned int cnt=0,esc_i=0;
    char c=0;

    Information(); // �ȳ����� �� ����. 

    if((fp=fopen("Temp\\Temp.txt","r+"))==NULL)	{							// ���� ������ ���� Temp.txt�� ������  
    	system("mkdir Temp"); system("copy /Y File\\Article.txt Temp\\Temp.txt"); //Article���� ���� �ҷ����� 
		fp=fopen("Temp\\Temp.txt","r+");
		if(BlankGenerator(fp,&cnt,&word)==-1) return -1;					//��ĭ ����.
	}
	else{											// Temp.txt�� ���������� �ҷ����� 
		fp_db=fopen("Temp\\wddat.dat","rb");		// wddat.dat���κ��� 
		fread(&cnt,sizeof(unsigned int),1,fp_db);	// cnt�� �о����, 
		word=(wdif*)calloc(sizeof(wdif),cnt);
		fseek(fp_db,5,SEEK_SET);
		fread(word,sizeof(wdif),cnt,fp_db);			// word[cnt]�� �����´�.
		fclose(fp_db);
	}
	
	if( (c=Screen(fp,cnt,&esc_i,word)) == 1 ) Review(fp,&cnt,&esc_i); 	// ���� �� ���߸� �������� ���θ� ������� ������ ����. 
    else if(c==2) goto END; 								// �ܾ Ʋ���� �ʰ� ��� �¾Ұų� ������ �ź����� �� �׳� �����Ų��. 
	
	while((c=Menu(cnt))!=ENTER){					//Menu()���� ENTER�� ������ ���α׷� ���� 
        if(c=='f'||c=='F'){
            CLEAR fputs("�� �� �Է� (1~5�� ����): ",stdout); // ��ĭ�� �󵵼��� �ٽ� ����. 
            c=(Ask(5,'1','2','3','4','5')-'0');
            fp_db=fopen("Temp\\userdata.txt","r+");
            fprintf(fp_db,"%d",c);
            fclose(fp_db);
			c='g'; 					//�󵵼��� ����Ǹ� �ٽ� ��ĭ�� �վ�� ����ǹǷ� ��ĭ �����... 
        }
        if(c=='g'||c=='G'){			//��ĭ�� �� ����. 
            fclose(fp);
            system("copy /Y File\\Article.txt Temp\\Temp.txt");  //Article.txt�κ��� �ٽ� �ҷ��鿩�ͼ� 
            fp=fopen("Temp\\Temp.txt","r+");
            if(BlankGenerator(fp,&cnt,&word)==-1) return -1; // ��ĭ �ձ� 
            esc_i=0; // Ŀ�� ��ġ�� �⺻ ��ġ�� �ʱ�ȭ 
        }
        else if(c=='c'||c=='C'){
        	fclose(fp);
            system("copy /Y File\\Article.txt Temp\\Temp.txt");  //Article.txt�κ��� �ٽ� �ҷ��鿩�ͼ� 
            fp=fopen("Temp\\Temp.txt","r+");
            
        	custom_blank(fp,&cnt,&word);
        	esc_i=0;
		}
        else if(c=='p'||c=='P'){ // �ո� ��ĭ�� ������ ��� �ؽ�Ʈ ���Ϸ� ����Ѵ�. 
            fclose(fp);
            CLEAR
            system("copy /Y Temp\\Temp.txt File\\Output.txt"); PAUSE
            fp=fopen("File\\Output.txt","r+");
            while((c=fgetc(fp))!=EOF)
                if(c==MARK) {				//Temp.txt���� �ܾ��� �������� �˱����� MARK '|'�� ����ߴµ� 
                    fseek(fp,-1,SEEK_CUR);
                    putc('_',fp);			//������� �� ���� ���ϰ� �ʿ䵵 ������, ���ֹ�����. 
                    fseek(fp,0,SEEK_CUR);
                }
            puts("\nOutput.txt�� Ȯ�����ּ���.");
            fclose(fp);
            fp=fopen("Temp\\Temp.txt","r+");
            PAUSE
            continue; // �ٽ� �޴��� ���ư���. 
        }
        else if(c=='r'||c=='R') Review(fp,&cnt,&esc_i); 							// �������. Review()�� ����. 
        else if(c==F1) { Tutorial(); continue; } 									// ���� ����. ���� �� ���ķ� �ٽ� �޴��� ���ư���. 
        if( (c=Screen(fp,cnt,&esc_i,word)) == 1 ) Review(fp,&cnt,&esc_i); 	// ���� �� ���߸� �������� ���θ� ������� ������ ����. 
        else if(c==2) goto END; 								// �ܾ Ʋ���� �ʰ� ��� �¾Ұų� ������ �ź����� �� �׳� �����Ų��. 
    }
    END:
    free(word); fclose(fp);
    puts("\n���α׷��� �����մϴ�."); PAUSE
	return 0;
}

void Review(FILE *fp,unsigned int* pcnt,unsigned int* esc_i){
    unsigned int i,count;
    wdif *nword; //new word info
    sco_stat=*esc_i=0;
    nword=(wdif*)calloc(sizeof(wdif),1);
    for(i=count=0;i<*pcnt;++i){
        if( rBIT(word[i].stat,CORRECT) && !rBIT(word[i].stat,WRONG) ){					// �ѹ��� ���� �ܾ���� ��ĭ�� �վ� �ٽ� ������ �ʿ䰡 ��� 
            fseek(fp,word[i].loc,SEEK_SET); // �׳� Temp.txt�� ��ĭ�� ä���ִ´�. 
            fputs(word[i].str,fp);
        }
        else{								// �ѹ��̶� Ʋ�Ȱų� ��Ʈ�� �� �ܾ���, 
            nword[count].loc=word[i].loc;	// word[cnt]�� ������ �����Ѵ�. 
            nword[count].len=word[i].len;
            nword[count].pos=word[i].pos;
            strcpy(nword[count].str,word[i].str);
            nword[count].stat=nword[count].wrongstr[0]=0;
            nword=(wdif*)realloc(nword,sizeof(wdif)*(++count+1));
        }
    }
    *pcnt=count;
    free(word); 									// ������ word�� �Ҹ��Ű�� nword�� ���� word��. 
    word=nword; CLEAR
	printf("������ %d���� �ܾ��:\n",count); 		//������ �ܾ�鵵 ǥ���ϰ� 
    for(i=0;i<count;++i)
        printf("%s\n",word[i].str);
    printf(":������ %d���� �ܾ��\n",count);
    
    FILE *fp_db=fopen("Temp\\wddat.dat","wb"); 		// wddat�� word ����ü�� ������ �ִ´�. 
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
	puts("[�޴�(���� â)���� â ũ�� ������ �����մϴ�.]\n[â ũ�� ���� �� ���� �迭�� �̻����� �� �ֽ��ϴٸ� �Ű澲���� ���ð� ������ּ���.]");
	puts("���� ������ �ٲ�� �Ǹ� �ݵ�� �ٽ� ��ĭ �����(G)�� ���� �ؾ� �մϴ�.\n");
	for(i=cor=fix=0;i<cnt;++i){
        if( rBIT(word[i].stat,WRONG) ) ++fix;
        else if( rBIT(word[i].stat,CORRECT) ) ++cor;
    }
    printf("���� ���� %d��, ������ ���� %d��, Ʋ�� �ܾ� �Ǵ� ������ ���� %d��\n",cor,fix,cnt-cor-fix);

    fputs("\n�޴�(ESC: �н���, ENTER: ����, F1: ����, C: ����� ���� ��ĭ F: �� ����, G: ��ĭ �����, P: �ؽ�Ʈ ���� ���, R: ����): ",stdout);
    return Ask(13,ESC,ENTER,F1,'c','C','f','F','g','G','p','P','r','R');
}

void Tutorial(void) {
    CLEAR puts("[�н�â������ â ũ�� ������ �Ұ����մϴ�.]");
	printf("%7s : %15s\n%7s : %15s\n", "��", "���� �ܾ�� �̵�","TAB, ��", "������ �ܾ�� �̵�");
    printf("%7s : %15s\n%7s : %15s\n%7s : %15s\n", "ENTER", "ä��", "SPACE", "��Ʈ On/Off", "ESC", "�޴�"); PAUSE }

void Information(void){
	CLEAR StarLine(80);
    puts("Blank Learner 1.0\n\n\t���� : ���ر�, �ڳ���\n\n<����> BlankLearner.exe�� ���� ��ο� File������ ���� �� ���� ���ϵ��� ���� �ȿ� ������ �ּ���.");
    puts("Article.txt�� ������ �ٿ���������.\nException.txt�� ���� ��ĭȭ ��Ű�� ���� �ܾ �Է��ϼ���.\nVocaList.txt�� ������ ��ĭȭ ��ų �ܾ �Է��ϼ���.(��ҹ��� ����)");
    StarLine(80); PAUSE Tutorial();}

void StarLine(unsigned int i){ for(; i!=0; --i) putchar('*');	putchar('\n');} // i���� ���� �� �ٿ�  ����Ѵ�. 

char Ask(int n,...){					// ���� �����Լ��̸�, ���� n�� ���� ���������� ������ �޴´�. 
    char c; unsigned int i;
    va_list ap;
    va_start(ap,n);
    while(1){							//getch()�� Ű����κ��� �Ű������� �Է¹��� ���ڰ� �Էµ� ������ ��� �Է¹���. 
        va_start(ap,n);
        c=getch();
        if(c<=0) c=getch();
        for(i=n;i!=0;--i)
            if(c==(char)va_arg(ap,int)){
                va_end(ap);
                putchar(c);
                return c;				// �Ű������� �Է¹��� ���ڰ� �ɸ��� �� ���ڸ� ��ȯ. 
            }
        va_end(ap);
    }
}
