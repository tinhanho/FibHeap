#include "stdlib.h"
#include "stdio.h"

/***********Struct, Global variables*************/

struct myList{
	int val;
	int degree;
	struct myList *Lsiblings;
	struct myList *Rsiblings;
	struct myList *child;
	struct myList *parent;
	int ISTRAVEL; // FOR USELESS step but this hw needs it
	int marked;
};

int node_num;

char *output;

struct myList *root; //Root list

struct myList *min; //Pointer points to minimum number

struct myList *myPointerArray[100];

struct myList *Queue[1000];
int Queue_len=0;

struct {
	int val;
	struct myList *node;
}	BIGARRAY[500];
int ARRAY_LEN=0;

/**********************************************/


/*****
??????*/

int WHATISTHIS = 0;

/******/



/*******************function******************/
int compare(const void* a, const void* b) {
   return (((struct myList*)a)->val - ((struct myList*)b)->val);
}

void showCircular(void){
	int QL = Queue_len;
	Queue_len=0;
	
	for(int i=0; i<QL; i++){
		struct myList *trav = Queue[i];
		int end = Queue[i]->val;
		
		ARRAY_LEN=0;
		
		while(1){

			//printf("%d ", trav->val);
			BIGARRAY[ARRAY_LEN].val=trav->val; 
			BIGARRAY[ARRAY_LEN].node=trav; 
			ARRAY_LEN++;
			
			trav = trav->Lsiblings;			

			if(trav->val==end)break;
		}
		qsort(BIGARRAY, ARRAY_LEN, sizeof(BIGARRAY[0]), compare);
		for(int i=0; i<ARRAY_LEN; i++) {
			printf("%d ", BIGARRAY[i].val);
		
			if(BIGARRAY[i].node->child!=NULL){
				Queue[Queue_len]=BIGARRAY[i].node->child;
				Queue_len++;
				//printf("CHILD: %d\n", trav->child->val);
			}	
		}
	}
}

void showTree(struct myList *subTree){
	if(subTree==NULL) return;	
	
	struct myList *trav =subTree;
	Queue_len=0;
	

	if(trav->parent==NULL && WHATISTHIS){
		printf("\n");
	}
	WHATISTHIS = 1;

	printf("%d ", trav->val);
	
	if(trav->child!=NULL){
		Queue[Queue_len]=trav->child;
		Queue_len++;
	}

	while(Queue_len) showCircular();

}

void fiboHeap_Insert(int num){
	if(root==NULL){
		struct myList *newNode = malloc(sizeof(struct myList));
		newNode->val=num;
		newNode->degree=0;
		newNode->Lsiblings=newNode;
		newNode->Rsiblings=newNode;
		newNode->child=NULL;
		newNode->parent=NULL;
		newNode->ISTRAVEL=0;
		newNode->marked=0;		
		root = newNode;
		min = newNode;
	}		
	else{
		struct myList *newNode = malloc(sizeof(struct myList));
		newNode->val = num;
		newNode->degree=0;		
		struct myList *tmp = root->Lsiblings;
		
		newNode->Lsiblings = tmp;
		newNode->Lsiblings->Rsiblings = newNode;
		newNode->Rsiblings = root;
		newNode->Rsiblings->Lsiblings = newNode;
		newNode->child=NULL;
		newNode->parent=NULL;
		newNode->ISTRAVEL=0;
		newNode->marked=0;	
		
		if(num<min->val) min = newNode; 
	}
	node_num++;
}

struct myList *find(int num, struct myList *tree){

	struct myList *trav = tree;
	struct myList *end = tree;

	while(1){
		if(trav->val == num) return trav;
	
	
		if(trav->child!=NULL){
			struct myList *findchild = find(num, trav->child);
			if(findchild!=NULL) return findchild;
		}
		
		trav = trav->Rsiblings;
		
		if(trav==end) break;
	}
	return NULL;
}


void fiboHeap_Cut(struct myList *num_pos){
	if(num_pos->Rsiblings==num_pos) num_pos->parent->child = NULL;
	if(num_pos == num_pos->parent->child) num_pos->parent->child=num_pos->Rsiblings;
	
	num_pos->parent->degree--;
	
	num_pos->Rsiblings->Lsiblings=num_pos->Lsiblings;
	num_pos->Lsiblings->Rsiblings=num_pos->Rsiblings;	

	// To root list
	//printf("ROOT: %d", root->val);
	root->Lsiblings->Rsiblings = num_pos;
	num_pos->Lsiblings = root->Lsiblings;
	root->Lsiblings = num_pos;
	num_pos->Rsiblings = root;

	num_pos->parent = NULL;
	num_pos->marked = 0;
	//printf("ROOT->Rsiblings->Rsiblings: %d", root->Rsiblings->Rsiblings->val);	
}

void fiboHeap_CasCut(struct myList *num_pos){
	struct myList *npparent = num_pos->parent;
	if(npparent!=NULL){
		if(num_pos->marked==0) num_pos->marked=1;
		else{
			fiboHeap_Cut(num_pos);
			fiboHeap_CasCut(npparent);
		}
	}
}

void fiboHeap_DecreaseKey(int num, int decrease_num){
	struct myList *num_pos = find(num, root);
	struct myList *npparent = num_pos->parent;
	//printf("NUMPOS: %d", num_pos->val);
	num_pos->val = num-decrease_num;

	if(npparent!=NULL && num-decrease_num<npparent->val){
		fiboHeap_Cut(num_pos);
		fiboHeap_CasCut(npparent);
	}
	if(num-decrease_num<min->val) min = num_pos;
}


void fiboHeap_link(struct myList *node1, struct myList *node2){
	struct myList *org_child = NULL;
	//printf("\nnode1: %d, node2: %d \n", node1->val, node2->val);
	if(node1->val < node2->val){ //node1 is parent
		root = node1;
		//Remove from root list
		node2->Lsiblings->Rsiblings = node2->Rsiblings;
		node2->Rsiblings->Lsiblings = node2->Lsiblings;
		
		if(node1->child!=NULL) org_child = node1->child;
		node2->parent = node1;

		/*if(node1->child==NULL || node2->val < node1->child->val) */node1->child = node2; /*delete*/		
	
		if(org_child!=NULL){
			node2->Rsiblings = org_child->Rsiblings;
			org_child->Rsiblings->Lsiblings = node2;
			node2->Lsiblings = org_child;
			node2->Lsiblings->Rsiblings = node2;
		}
		else{
			node2->Lsiblings = node2;
			node2->Rsiblings = node2;
		}
		
		myPointerArray[node1->degree]=NULL;
		node1->degree++;
		node1->parent=NULL;
		//if(node1->val==10) printf("10 righit = %d", node1->Rsiblings->val);

		if(myPointerArray[node1->degree]==NULL) myPointerArray[node1->degree] = node1;
		else fiboHeap_link(myPointerArray[node1->degree], node1);
	}
	else{ //node2 is parent
		//Remove from root list
		root = node2;
		node1->Lsiblings->Rsiblings = node1->Rsiblings;
		node1->Rsiblings->Lsiblings = node1->Lsiblings;

		if(node2->child!=NULL) org_child = node2->child;
		node1->parent = node2;
		
		/*if(node2->child==NULL || node1->val < node2->child->val)*/ node2->child = node1; /*delete*/
		
		
		if(org_child!=NULL){
			node1->Rsiblings = org_child->Rsiblings;
			org_child->Rsiblings->Lsiblings = node1;
			node1->Lsiblings = org_child;
			node1->Lsiblings->Rsiblings = node1;
		}
		else{
			node1->Lsiblings = node1;
			node1->Rsiblings = node1;
		}

		
		myPointerArray[node1->degree]=NULL;
		node2->degree++;
		node2->parent=NULL;	
		if(myPointerArray[node2->degree]==NULL) myPointerArray[node2->degree] = node2;
		else fiboHeap_link(myPointerArray[node2->degree], node2);
	}
}

void Consolidate(void){
	struct myList *trav = root;
	int thebreak=1;

	// Init
	for(int i=0; i<10; i++){
		myPointerArray[i]=NULL;
	}

	// Update min
	while(1){
		if(min==NULL || trav->val < min->val) min=trav;
		trav = trav->Rsiblings;
		if(trav==root) break;
	}
	
	struct myList *x = NULL;
	
	//reset
	trav = root;
	while(1){
		trav->ISTRAVEL = 0;
	
		trav = trav->Rsiblings;
		if(trav==root) break;
	}	
	
	while(1){
		trav = root;
		while(1){
			if(trav->ISTRAVEL==0){
				thebreak=0;
				break;
			}
			thebreak=1;
			trav=trav->Rsiblings;
			if(trav==root) break;
		}
		if(thebreak) break;
		
		trav = root;

		while(1){
			//find smallest node in root list
			if(  (x==NULL || trav->val < x->val) && !trav->ISTRAVEL ){
				x = trav;
				//printf("%d \n", x->degree);
			}
			trav=trav->Rsiblings;	
			//printf("%d", x->val);
			if(trav==root && x!=NULL) break;
		}
		
		x->ISTRAVEL=1;
		int degree = x->degree;
		//printf("val: %d\n", x->val);
		if(myPointerArray[degree]==NULL){
			myPointerArray[degree] = x;
		}
		else{
			fiboHeap_link(myPointerArray[degree], x);
		}
		x=NULL;
	}
}

void fiboHeap_ExtMin(void){
	struct myList *trav = root;
	
	// if min has no child
	if(min->child==NULL){
		if(root==min) root = min->Rsiblings;
		min->Lsiblings->Rsiblings = min->Rsiblings;
		min->Rsiblings->Lsiblings = min->Lsiblings;
	}
	else{ // min has child
		if(root==min) root = min->child;
		//printf("ROOT: %d\n", root->val);

		if(min->Rsiblings!=min){
			// printf("TEST: %d\n", min->child->val);
			// printf("TEST2: %d\n", min->child->Rsiblings->val);
			// printf("TEST3: %d\n", min->Rsiblings->val);		
			if(min->child->Rsiblings!=min->child){
				min->Lsiblings->Rsiblings = min->child->Rsiblings;
				min->child->Rsiblings->Lsiblings = min->Lsiblings;
			}
			else{
				min->child->Lsiblings = min->Lsiblings;
				min->child->Lsiblings->Rsiblings = min->child;
			}
			min->child->Rsiblings = min->Rsiblings;
			min->Rsiblings->Lsiblings = min->child;
			// printf("TEST4: %d\n", min->child->Rsiblings->val);
		}
		min->child->parent=NULL;
	}
	
	min = NULL;

	node_num--;
	Consolidate();
	if(node_num==0) root=NULL;
}


void fiboHeap_Delete(int num){	
	fiboHeap_DecreaseKey(num, 10000);
	fiboHeap_ExtMin();
}
/********************************************/


int main(){
	
	char *str = malloc(sizeof(char)*500);
	char *input = str;
	output = malloc(sizeof(char)*500);
	//char *ans = output;
	
	root = NULL;
	node_num = 0;
	
	while(1){
		memset(str, '\0', 500);
		input = str;
		fgets(input, 500, stdin);

		int command;		
		if(*input=='i') command=1;
		else if(*input=='e' && *(input+1)=='x' && *(input+2)=='t') command = 2;
		else if(*input =='d' && *(input+2)=='l') command = 3;
		else if(*input == 'd' && *(input+2)=='c') command = 4;
		else if(*input=='e' && *(input+1)=='x' && *(input+2)=='i') command = 5;
		else if(*input=='s') command = 6;
		
		while(*input!=' ' && command!=2 && command!=5 && command!=6) input++;
		input++;
		
		/*
		1: insert
		2: extract-min
		3: delete
		4: decrease
		5: exit
		6: show
		*/
		if(command==1){
			fiboHeap_Insert(atoi(input));
		}
		else if(command==2){
			fiboHeap_ExtMin();
		}
		else if(command==3){
			fiboHeap_Delete(atoi(input));
		}
		else if(command==4){
			int num = atoi(input);
			while(*input!=' ') input++;
			input++;
			int decrease_num = atoi(input);
			fiboHeap_DecreaseKey(num, decrease_num);
		}
		else if(command==5){
			struct myList *trav = root;
			
			for(int i=0; i<10; i++){
				myPointerArray[i]=NULL;
			}		
			
			while(1){
				myPointerArray[trav->degree] = trav;
				//printf("trav deg:%d val:%d \n", trav->degree, trav->val);
				trav = trav->Rsiblings;
				
				if(trav==root) break;
			}
	

			for(int i=0; i<10; i++){
				if(myPointerArray[i]!=NULL){
					//printf("val:%d, degree:%d", myPointerArray[i]->val, myPointerArray[i]->degree);
					showTree(myPointerArray[i]);
				}
			}
			break;
		}
		else if(command==6){
			//FOR DEBUG SECTION
			// struct myList *trav = root;
			// struct myList *end = root;
			// struct myList *tc = NULL;
			// int trav_child = 0;
			// printf("\nMin: %d\n", min->val);	
			// while(1){
				// printf("%d ", trav->val);
				// trav = trav->Rsiblings;
				// printf(" | ");
				// if(trav==end) break;
			// }				
			// struct myList *trav = tree;
			// struct myList *end = tree;

			// while(1){
				// if(trav->val == num) return trav;
				
				
				// trav = trav->Rsiblings;
				
				// if(trav==end) break;
			// }
			struct myList *trav = root;
			
			for(int i=0; i<10; i++){
				myPointerArray[i]=NULL;
			}		
			
			while(1){
				
				if(myPointerArray[trav->degree]==NULL) myPointerArray[trav->degree] = trav;
				else printf("%d\n", trav->val);
				//if(trav->val==3) printf("deg3: %d", trav->degree);
				//printf("trav deg:%d val:%d \n", trav->degree, trav->val);
				trav = trav->Rsiblings;
				
				if(trav==root) break;
			}
	

			for(int i=0; i<10; i++){
				if(myPointerArray[i]!=NULL){
					//printf("val:%d, degree:%d", myPointerArray[i]->val, myPointerArray[i]->degree);
					showTree(myPointerArray[i]);
				}
			}
		}
	}
	
	// while(*ans!='\0'){
		// printf("%c", *ans);
		// ans++;
	// }
}