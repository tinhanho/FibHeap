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
	int ISTRAVEL;
};

int node_num;

char *output;

struct myList *root; //Root list

struct myList *min; //Pointer points to minimum number

struct myList *myPointerArray[10];

/**********************************************/




/*******************function******************/


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
		
		if(num<min->val) min = newNode; 
	}
	node_num++;
}

void fiboHeap_DecreaseKey(void){
	
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
		node1->child = node2;
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
		node2->child = node1;
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

		if(myPointerArray[node2->degree]==NULL) myPointerArray[node2->degree] = node2;
		else fiboHeap_link(myPointerArray[node2->degree], node2);
	}
}

void Consolidate(void){
	struct myList *trav = root;
	int thebreak=1;

	int loop_num = node_num;

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
			if( (x==NULL || trav->val < x->val) && !trav->ISTRAVEL ){
				x = trav;
			}
			printf("TRAV %d\n", trav->val);
			trav=trav->Rsiblings;	
			if(trav==root) break;			
		}
		
		x->ISTRAVEL=1;
		int degree = x->degree;
		printf("val: %d\n", x->val);
		if(myPointerArray[degree]==NULL){
			myPointerArray[degree] = x;
		}
		else{
			fiboHeap_link(myPointerArray[degree], x);
			//printf("FL: %d , %d", myPointerArray[degree]->val, tmp->val);
		}
		x=NULL;
	}

	//reset
	trav = root->Rsiblings;
	while(trav!=root){
		trav->ISTRAVEL=0;
		trav=trav->Rsiblings;
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
		printf("ROOT: %d\n", root->val);

		if(min->Rsiblings!=min){
			min->Lsiblings->Rsiblings = min->child;
			min->child->Lsiblings->Rsiblings = min->Rsiblings;
			min->Rsiblings->Lsiblings = min->child->Lsiblings;
			min->child->Lsiblings = min->Lsiblings;
		}
	}
	
	min = NULL;

	node_num--;
	
	Consolidate();
}


// void fiboHeap_Delete(int num, int degree){
	
	
// }
/********************************************/


int main(){
	
	char *str = malloc(sizeof(char)*500);
	char *input = str;
	output = malloc(sizeof(char)*500);
	char *ans = output;
	
	root = NULL;
	node_num = 0;
	
	while(1){
		memset(str, '\0', 500);
		input = str;
		fgets(input, 500, stdin);

		int command;		
		if(*input=='i') command=1;
		else if(*input=='e' && *(input+1)=='x' && *(input+2)=='t') command = 2;
		else if(*input =='d') command = 3;
		else if(*input=='e' && *(input+1)=='x' && *(input+2)=='i') command = 4;
		else if(*input=='s') command = 5;
		
		while(*input!=' ' && command!=2 && command!=4 && command!=5) input++;
		input++;
		
		/*
		1: insert
		2: extract-min
		3: delete
		4: exit
		5: show
		*/
		if(command==1){
			fiboHeap_Insert(atoi(input));
		}
		else if(command==2){
			fiboHeap_ExtMin();
		}
		else if(command==4){

			break;
		}
		else if(command==5){
			//FOR DEBUG SECTION
			struct myList *trav = root;
			struct myList *end = root;
			struct myList *tc = NULL;
			int trav_child = 0;
			printf("\nMin: %d\n", min->val);	
			while(1){
				printf("%d ", trav->val);
				// if(trav->child!=NULL){
					// trav_child=1;
					// tc = trav->child;
				// }
				// while(trav_child){
					// struct myList *tmptrav = tc;
					// struct myList *tmpend = tc;
					// printf(" - ");
					// while(1){
						// printf("%d ", tmptrav->val);
						// if(tmptrav->child!=NULL){
							// printf("(%d)", tmptrav->child->val);
						// }
						// tmptrav = tmptrav->Rsiblings;
						// if(tmptrav==tmpend) break;
					// }

					// if(tc->child==NULL) trav_child=0;
					// else{
						// tc = tc->child;
					// }
				// }
				trav = trav->Rsiblings;
				printf(" | ");
				if(trav==end) break;
			}				


		}
	}
	
	// while(*ans!='\0'){
		// printf("%c", *ans);
		// ans++;
	// }
}