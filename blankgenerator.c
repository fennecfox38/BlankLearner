/************************************************************************************************************
���� �̸�: blankgenerator.c
�ֿ� �Լ�: int BlankGenerator(FILE *fp,unsigned int* pcnt,wdif** pword);
�ֿ� ���: 1. ������ �ҷ��� ���� ���� ����� �����Ͽ� ��ĭ�� ������ ���� 2. ��ĭ�ձ� 3. cnt, word�� ����. 
��� ����: ���� �ҷ����� �� ��ĭ ���� �� �߻���  ���� �� ���� ���θ�  0�� -1�� ��ȯ. 
�Է� ����: fp�� ���� �޾ƿ���, cnt�� word�� ���� cnt�� word�� �����ͷ� ��ȯ�����ش�. 
������: ���ر�, �ڳ��� 
************************************************************************************************************/
#include"header.h"
#include<time.h>

typedef struct {char **str; unsigned int now;} wdlist;                     
                   
boolean fgets_abc(char *str, unsigned int siz, FILE *fp_db); //�ܾ������κ��� �ܾ���� �ҷ��´�. 
void Register(FILE *fp_db,wdlist *data); // �ؽ�Ʈ�κ��� �ܾ����� �ҷ��� wdlist�� data �� �����Ѵ�. 
int Search(char *str); // �ܾ ���ܸ��, �ʼ� ��Ͽ� �ִ��� �˻�. 
boolean dtblank(unsigned int wdcnt,char freq,char *str); // ��ĭ�� ������ ���� �������ش�. 

wdlist exc,voca;

int BlankGenerator(FILE *fp,unsigned int* pcnt,wdif** pword){
    FILE *fp_db; wdif *word;
    char c,freq;
	unsigned int wdcnt,cnt,len,loc,i;           // len: word length  / loc: location of filepointer indicating position's distance from SEEK_SET

	srand((unsigned)time(NULL));		// srand()�� �̿��� rand()�� seed �ʱ�ȭ 
	exc.now=voca.now=1;
	word=(wdif*)calloc(1,sizeof(wdif));

	//Register()�� ���� �ؽ�Ʈ ���Ͽ� �ִ� �ܾ� ����� wdlist ���� ����ü�� exc,voca�� ������. 
	if((fp_db=fopen("File\\Exception.txt","rt"))==NULL) {fprintf(stderr,"Failed to open Exception.txt\n");return -1;}
	Register(fp_db,&exc);	fclose(fp_db);
    if((fp_db=fopen("File\\VocaList.txt","rt"))==NULL) {fprintf(stderr,"Failed to open VocaList.txt\n");return -1;}
	Register(fp_db,&voca);	fclose(fp_db);
	
	if((fp_db=fopen("Temp\\userdata.txt","rt"))==NULL){ //userdata�� ������ �Ұ����ϸ� 
		fp_db=fopen("Temp\\userdata.txt","wt");
		freq=3;					//userdata�� (freq=3)�� ����. 
	}
	else {												//userdata�� ���� ������ 
		fscanf(fp_db,"%d",&freq); fclose(fp_db);		//freq�� �о�´�. 
		fp_db=fopen("Temp\\userdata.txt","wt");
	}
	
	fseek(fp,0,SEEK_SET);
	fprintf(fp_db,"%d:frequency\n%4s| %s\n",freq,"cnt","string"); //userdata.txt�� �޾��� ���. 
	
	fseek(fp,0,SEEK_SET);
	for(len=wdcnt=cnt=0;(c=fgetc(fp))!=EOF;++len){	// len is used to count length of word, while getting character from file until it reachs End Of File.
		if( isalpha(c) )
            word[cnt].str[len]=c;	                        // ���ĺ��̸� ���ڿ��� ��������. 
		else{												// ���ĺ��ƴϸ� �ܾ� �������ϱ� '\0'�����ϰ� ��ĭ �ձ� ����. 
			word[cnt].str[len]='\0';
			if(len==0);
			else if(len==1) ++wdcnt; 
			else{ 													// ������ ������ �ܾ��϶�, 
				if(dtblank(++wdcnt%10,freq,word[cnt].str)==TRUE){	// dtblank()�� �����Ѵ�. ��ĭ �ձ�� �����ϸ�,
					loc=ftell(fp)-1-len;							// fp�� ����Ű�� �ִ� ��ġ ����. 
					if(c=='\n') --loc;								// '\n'�� 2���� ������ �����ϴ� �Ϳ� ���� �����̴�. 
					word[cnt].loc=loc; word[cnt].len=len;			// word[cnt]�� loc, len ����. 
					fprintf(fp_db,"%04d: %s\n",cnt+1,word[cnt].str);
					word=(wdif*)realloc(word,sizeof(wdif)*(++cnt+1)); // word[]�� �������� �޸� �Ҵ�. 
					word[cnt].stat=word[cnt].str[0]=word[cnt].wrongstr[0]=0;
					fseek(fp,loc,SEEK_SET);
					fputc(MARK,fp);								// ��ĭ�ܾ��� �������� MARK '|'�� üũ 
					for(--len;len>0;--len) fputc('_',fp);		// ��ĭ���� �վ������. 
					fseek(fp,loc+len,SEEK_SET);					// ���������� �д� ��ġ�� ���� �ܾ�� �Űܹ�����. 
				}
			}len=-1;
		}
	}
	// ����ϰ� ���� �޸� �Ҵ� ����. 
	for(i=0;i<exc.now;++i) free(exc.str[i]);
	for(i=0;i<voca.now;++i) free(voca.str[i]);
	free(exc.str); free(voca.str);
	
	fprintf(fp_db,"Total %d of words were successfully replaced as blank.\n",cnt);
	fclose(fp_db); // ���������͵� �� ������ �ݾ��ִ� ����. 
	
	fp_db=fopen("Temp\\wddat.dat","wb"); // ���߿� �ٽ� ��ĭ �� �հ� ��Ȱ�� �Ҷ� ��Ȱ�� �� �� �ֵ���, 
	fseek(fp_db,0,SEEK_SET);
	fwrite(&cnt,sizeof(unsigned int),1,fp_db); //cnt�� 
	fseek(fp_db,5,SEEK_SET);
	fwrite(word,sizeof(wdif),cnt,fp_db);	// word ����ü�迭�� �������ش�. 
	fclose(fp_db);
	
	// ����� ���� cnt�� word�� main()�� cnt�� word�� �����͸� ���� ��ȯ. 
	*pcnt=cnt;
	*pword=word;
	return 0;
}

boolean dtblank(unsigned int wdcnt,char freq,char *str){			// Function that determine whether replace as blank or not.
    int i,sum=0;
    static boolean dc[10]={0,1,0,1,0,1,0,1,0,1};	// dc[] �� ��ĭ�� ���� �� ������ ������ ���(��,����)�� ��� ��.
    
	if((i=Search(str))==0) return dc[wdcnt]=FALSE;			// �ش� �ܾ Exception.txt�� ������ ��ĭ�� ���� �ʴ´�. 
	else if(i==2) return dc[wdcnt]=TRUE;					// �ش� �ܾ VocaList.txt�� ������ �� �մ´�. 
    else if(dc[(wdcnt+9)%10]==TRUE) return dc[wdcnt]=FALSE;		// ���� �ܾ �շ� ������ ���� ��ĭ�� ������Ű�� ���� ���� �ʵ��� �Ѵ�. 
    else if(isupper(str[0])) return dc[wdcnt]=FALSE;		// �빮�ڷ� �����ϸ� ��ĭ�� ���� �ʴ´�. (�����̳� ��������� ��츦 ����) 
    
	else{											// �̷��� ���������� ��� �����ϸ�, ��ĭ ������ �������� ��������. 
		if((i=rand()%3)!=2) return dc[wdcnt]=i;
 
		for(i=9;i>=0;--i)
			sum+=dc[i];								// �ٹ� 10���� ��ĭ�� ��� �ִ��� Ȯ�� 
        if(sum>freq) dc[wdcnt]=FALSE;				// ����ġ���� ������ �� ����. 
        else if(sum<freq) dc[wdcnt]=TRUE;			// ����ġ���� ������ �մ´�. 
        else dc[wdcnt]=rand()&&1;					// ���ġ�� ����. 
        return dc[wdcnt];							// ��� ��ȯ, dc[j]�� �����ؼ� �ΰ�ΰ� 10�� ����Ա�. 
    }
}

int Search(char *str){
	unsigned int i;
	for(i=0;i<exc.now;++i)
		if(strcmp(str,exc.str[i])==0) return 0;					// If str[] and exc_str[i][] containing same word, (if str[] contains exception word,) return 0.
	for(i=0;i<voca.now;++i)
		if(strcmp(str,voca.str[i])==0) return 2;
	return 1;													// If str[] does not contain any of exception word, return 1.
}

void Register(FILE *fp_db,wdlist *data){
	(*data).str=(char**) calloc( sizeof(char*), (*data).now );								// Exception Word list which is including the words that user doesn't want to replace as blank.
    (*data).str[0]=(char*) calloc( sizeof(char), LOW);
    while((fgets_abc((*data).str[(*data).now-1],LOW,fp_db))!=0){				//Exception registration to exc_str[][]
		(*data).str=(char**)realloc((*data).str, sizeof(char*) * ++(*data).now );				//Reallocation for exc_str, as needed.
		(*data).str[(*data).now-1]=(char*)calloc(sizeof(char),LOW); 						//New allocation for exc_str[].
	}
}

boolean fgets_abc(char *str, unsigned int siz, FILE *fp_db){	//fgets_abc�� ���Ͻ�Ʈ�����κ��� ���ĺ����� �̷���� �ܾ �����´�. 
	unsigned int i; char c;
	for(i=0;isalpha((c=fgetc(fp_db)))&&i<siz;str[i++]=c);
	str[i]='\0';												//���ڿ� ���� '\0' NULL ���� ���� 
	return (c!=EOF ? TRUE : FALSE);								//������ �������� �Ǻ�. 
}
