#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int type = -1;
int sort = -1;


typedef struct node{
	char *str;
	void *data;
	struct node *next;
}node_t;

typedef struct list{
	struct node* head;
	int size;
}list_t;

void makeList(list_t **pplist){
	(*pplist) = malloc(sizeof(list_t));
	(*pplist)->head = NULL;
	(*pplist)->size = 0;

}

void makeNode(node_t **ppnode, char *pstr, void *pdata){

	(*ppnode) = malloc(sizeof(node_t));
	//if(pstr == NULL) printf("WRONG!\n");
	//printf("%s = %d is added to new node\n", pstr,*((int*)(pdata)));

	(*ppnode) -> str = pstr;
	(*ppnode) -> data = pdata;
}

void addNode(list_t *plist, node_t *pnode){
	plist->size+=1;
//	printf("%s = %s is added\n", pnode->str,((char*)(pnode->data)));
	if(plist->size == 1){
		plist->head = pnode;
	}
	else{
		node_t *testNode = plist->head;
		while( (testNode->next) != NULL){
			testNode = testNode->next;
		}
		testNode->next = pnode;

	}
}

int search(list_t *plist, void *toSearch){
	node_t *testNode = plist->head;
	int *intSearch;
	double *doubleSearch;
	char *strSearch;

	switch(type){
		case 0:
			intSearch = (int*)toSearch;
			//printf("%d!\n", *intSearch);
			break;
		case 1:
			doubleSearch = (double*)toSearch;
			break;
		case 2:
			strSearch = (char*)toSearch;
			break;
		default:
			printf("Undefined type\n");
			return 0;
	}
	while(testNode !=NULL){
		//printf("%d\n", type);
		if(type == 2){
			if(strcmp(strSearch, (char*)(testNode->data)) == 0){
				printf("String Found!\n");
				return 1;
			}
		}
		else if(type == 0){
			//printf("Test %d and %d\n", *intSearch,*(int*)(testNode->data));
			if(*intSearch == *(int*)(testNode->data)){
				printf("Int Found!\n");
				return 1;
			}
		}
		else if(type == 1){
			if(*doubleSearch == *(double*)(testNode->data)){
				printf("Double Found!\n");
				return 1;
		}
	}
		testNode = testNode->next;

	}
	printf("Not found!\n");
	return 0;
}


void printList(list_t *plist, char* fileName){
	node_t *testNode = plist->head;
	//printf("The file name is%s\n", fileName);
	FILE *output = fopen(fileName,"w");
	if(testNode == NULL) printf("The list is null!\n");
	else{
		while(testNode != NULL){

		if(type == 1) {
			fprintf(output,"%s=%f\n", testNode->str, *((double*)testNode->data));
		}
			else if(type == 0){fprintf(output,"%s=%d\n", testNode->str, *((int*)testNode->data));
		}
			else if(type == 2){fprintf(output,"%s=%s\n", testNode->str, ((char*)testNode->data));
		}

			//fprintf(output,"\n");
		
		/*	if(type == 1){
				printf("%s=%f", testNode->str, *((double*)testNode->data));
		}
			else if(type == 0){printf("%s=%d", testNode->str, *((int*)testNode->data));
		}
			else if(type == 2){printf("%s=%s", testNode->str, ((char*)testNode->data));
		}

			printf("\n");

		*/
			testNode = testNode->next;
		}

	}
	fclose(output);
}

int cmp_inc(void* num1, void* num2){
		if(type == 0){
		if(*((int*)num1) >= *((int*)num2))return 1;
		else return 0;
	}
		else if(type == 1){
		if(*((double*)num1) >= *((double*)num2))return 1;
		else return 0;
		}
		else if(type == 2){
			if(strcmp((char*)num1,(char*)num2) > 0) return 1;
			else return 0;
		}

	return 0;
}

int cmp_dec(void* num1, void* num2){
		if(type == 0){
		if(*((int*)num1) >= *((int*)num2))return 0;
		else return 1;
	}
		else if(type == 1){
		if(*((double*)num1) >= *((double*)num2))return 0;
		else return 1;
		}
		else if(type == 2){
			if(strcmp((char*)num1,(char*)num2) > 0) return 0;
			else return 1;
		}

	return 0;
}

int cmp_rand(void* num1, void* num2){
	int result = rand()%2;
	//printf("%d\n", result);
	return result;
}


void sortList(list_t *plist, int sortType){
	int (*cmp)(void*,void*);

	if(sortType == 0){
		cmp = cmp_inc;
	}
	else if(sortType == 1){
		cmp = cmp_dec;
	}
	else if(sortType == 2){
		srand(time(NULL));

		cmp = cmp_rand;
	}
		for(node_t *temp = plist->head; temp!= NULL; temp = temp->next ){
			for(node_t *p = temp ->next ; p != NULL; p = p->next){
				if(cmp(temp->data, p->data)){
					void* victim = temp->data;
					temp->data = p->data;
					p->data = victim;
				}
			}
		}
	


}

int main(int argc, char *argv[]){
	if(argc < 2) return 1;

	char line[1024];
	FILE *fp;
	printf("reading %s\n",argv[1]);

	if((fp = fopen(argv[1],"r")) == NULL){
		printf("File not opened!\n");
		return 0;

	}

	char *fileName= argv[1];
	char *sortType = argv[2];
	if(strcmp(sortType, "inc") == 0) sort = 0;
	else if(strcmp(sortType, "dec") == 0) sort = 1;
	else if(strcmp(sortType, "rand") == 0) sort = 2;

	list_t *plist;
	makeList(&plist);
	//printf("size of the list is %d\n", (*plist).size);
	char *p;
	char *rest;
	p = strtok(fileName,"_");
	rest = strtok(NULL, "_");

	p = strtok(rest,".");

	if(strcmp(p,"int") == 0) type = 0;
	else if(strcmp(p, "double") == 0) type = 1;
	else if(strcmp(p, "char") == 0) type = 2;

	node_t *pnode[1024];
	int cnt = 0;
	char* name;
	int intData;

	char* cData;
	while(!feof(fp)){
		fgets(line, 1024,fp);
		char *pos = strchr(line,'=');
		if (!pos) continue;
		int length = pos - line;
		//printf("length = %d\n", length);
		*pos = '\0';
		char *str = (char*)malloc(sizeof(char) * (length+1));
		strcpy(str,line);
		void *data;
		//printf("%s\n", str);
			switch(type){
				case 0:
					data = malloc(sizeof(int));
					*((int*)data) = strtol(pos+1,NULL,10);
					break;
				case 1:
					data = malloc(sizeof(double));
					*((double*)data) = strtod(pos+1,NULL);
					break;
				case 2:
					length = strlen(pos + 1);
					data = malloc(sizeof(char) * (length + 1));
					strcpy(data,pos+1);
					//printf("%s=%s\n",str,data);
					pos = data + strlen(data) - 1;
					while(*pos == '\n') *(pos--)='\0';
					printf("%s=%s\n",str,data);

					break;
			}

		makeNode(&pnode[cnt],str,data);
		addNode(plist,pnode[cnt++]);
	}
	//printf("Reading complete\n");
	fclose(fp);

	printf("sorting elements\n");
	sortList(plist,sort);
	//printList(plist);

	char *sortName[] = {"inc","dec","rand"};
	char *varType[] = {"int","double","char"};
	char buffer[1024] = {};
	strcpy(buffer,sortName[sort]);
	int l = strlen(buffer);
	buffer[l] = '_';
	strcpy(buffer + l + 1, varType[type]);
	strcat(buffer,".txt");

	printf("writing %s\n", buffer);
	
	printList(plist,buffer);

	return 0;
}