#include "common.h"


int main(){
	//printf("%d\n",(1<<LETTERS)-(1<<(LETTERS-6)));
//	int i;
//	for(i=0;i<256;i++){
//		int num = 0;
//		int j=i;
//		while(j)
//		{
//			j &= (j-1);
//			num++;
//		}
//		printf("%d,",num);
//		if((i+1)%32==0)
//		printf("\n");
//	}
	//printf("%d\n",(1<<LETTERS)-(1<<(LETTERS-4)));
	char buff[10]="12345";
	while(1){
		int ret=getnext(buff);
		printf("<%s>,",buff);
		if(ret==0)break;
	}
	return 0;
}
