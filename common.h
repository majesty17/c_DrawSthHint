#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define LETTERS			12
#define WORD_MAX_LENG	30
#define WORDLIST		"C:\\words.txt"
///给getones函数用的数组
char countTable[256]={
0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
};
///树节点定义
struct Node{
	char isword;
	struct Node* next[26];
};
///插入word到root中
void add(struct Node* root,char *word){
	int len=strlen(word);
	int i;
	for(i=0;i<len;i++){
		int pos=word[i]-'a';
		if(root->next[pos]==NULL){
			root->next[pos]=(struct Node*)malloc(sizeof(struct Node));
			memset(root->next[pos],0,sizeof(struct Node));
		}
		root=root->next[pos];
		if(i==len-1){
			root->isword=1;
		}
	}
}
///建树
void init(struct Node* root){
	FILE *fd=NULL;
	char word[WORD_MAX_LENG];
	fd=fopen(WORDLIST,"r");
	if(fd==NULL){
		printf("File not found!\n");
		printf("Plz let me see %s!\n",WORDLIST);
		exit(0);
	}
	int count=0;
	printf("Initialing...\n");
	int start=clock();
	while(fgets(word,WORD_MAX_LENG,fd) !=NULL){
		word[strlen(word)-1]='\0';
		strlwr(word);
		add(root,word);
		count++;
	}
	int end=clock();
	printf("%d words loaded.\n",count);
	printf("Time cost:%6.3f sec\n",(double)(end-start)/CLOCKS_PER_SEC);
	fclose(fd);
}
///毁树
void del(struct Node* root){
	///to be
	int i;
	for(i=0;i<26;i++){
		if(root->next[i]!=NULL ){
			del(root->next[i]);
		}
	}
	free(root);
}
///检验用户输入
int verify(char * str){
	if(strlen(str)!=LETTERS)return 0;
	int i;
	for(i=0;i<LETTERS;i++){
		if(isalpha(str[i])){
			str[i]=tolower(str[i]);
		}
		else{
			return 0;
		}
	}
	return 1;
}
///从root树里查找str
int isword(struct Node* root,char *str){
	int len=strlen(str);
	int pos=str[0]-'a';
	if(len==1){
		if(root==NULL || root->next[pos]==NULL){
			return 0;
		}
		else if(root->next[pos]->isword==1){

			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		if(root==NULL){
			return 0;
		}
		else{
			return isword(root->next[pos],str+1);
		}
	}
}
///获得二进制的num中1的个数
int getones(int num){
	return countTable[num/256]+countTable[num%256];
	return 0;
}
///用来给qsort升序排列用的回调函数
int cmp(const void *a,const void *b){
	return *(char *)a - *(char *)b;
}
///获得str按照字典序的下一个排列
int getnext(char* str){
	int len=strlen(str);
	int i,j;
	///找到相邻两个，左边的小于右边
	for(i=len-2;i>=0;i--){
		if(str[i]<str[i+1]){
			break;
		}
	}
	///没找到，说明没有正序对
	if(i==-1){
		return 0;
	}
	for(j=len-1;j>=0;j--){
		if(str[j]>str[i]){
			break;
		}
	}
	///交换j,i
	char tmp=str[i];
	str[i]=str[j];
	str[j]=tmp;
	///从i+1开始，后面的反转
	strrev(str+i+1);
	return 1;
}
///开始搜索
void run(struct Node* root,int len,char *letters){
	int i,j,k,count=0;
	char buff[LETTERS]={0,};//=(char*)calloc(LETTERS,sizeof(char));
	char temp[LETTERS]={0,};
	char *outputs[512]={0,};
	qsort(letters,LETTERS,sizeof(char),cmp);
	int from=(1<<len)-1;
	int to=(1<<LETTERS)-(1<<(LETTERS-len));
	///e.g.: len=5,from 11111(2) to 111110000000(2)
	for(i=from;i<=to;i++){
		if(getones(i)!=len){
			continue;
		}
		memset(buff,0,LETTERS*sizeof(char));
		for(j=0,k=0;j<LETTERS;j++){
			if((i & (1<<j) )>0){
				buff[k++]=letters[j];
			}
		}
		///跟上次一样，则继续
		if(strcmp(buff,temp)==0){
			continue;
		}
		strcpy(temp,buff);
		///用buff里面的字母进行全排列
		while(1){
			if(isword(root,buff)){
				///查中！输出
				//printf("%s\n",buff);
				outputs[count++]=strdup(buff);
			}
			int ret=getnext(buff);
			if(ret==0){
				break;
			}
		}
	}
	///排序
	for(i=1;i<count;i++){
		for(j=0;j<i;j++){
			if(strcmp(outputs[j],outputs[i])>0){
				char *tmp=outputs[i];
				outputs[i]=outputs[j];
				outputs[j]=tmp;
			}
		}
	}
	///无重复输出
	for(i=0,j=0;i<count;i++){
		if(i>0 && strcmp(outputs[i],outputs[i-1])==0){
			continue;
		}
		printf("%03d\t%s\n",++j,outputs[i]);
	}
	///释放
	for(i=0;i<count;i++){
		free(outputs[i]);
	}
}




