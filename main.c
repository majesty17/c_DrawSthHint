#include "common.h"

int main(){
	int len;
	char letters[WORD_MAX_LENG];
	struct Node* root;
	root=(struct Node*)malloc(sizeof(struct Node));
	memset(root,0,sizeof(struct Node));
	init(root);
	while(1){
		printf("==============================================================\n");
		printf("Please input the length([0-8],0 to quit):");
		scanf("%d",&len);
		if(!len){
			break;
		}
		if(len<1 || len>8){
			printf("Error input!!!Restart!!!\n");
			continue;
		}
		printf("Please input all the candidate LETTERS(length=12):");
		scanf("%s",letters);
		if(verify(letters)==0){
			printf("Error input!!!Restart!!!\n");
			continue;
		}
		printf("Your input is:<%s>\n",letters);
		printf("Computering...\n");
		int start=clock();
		run(root,len,letters);
		int end=clock();
		printf("Time cost:%6.3f sec\n",(double)(end-start)/CLOCKS_PER_SEC);
	}
	printf("Bye\n");
	del(root);
	return 0;
}
