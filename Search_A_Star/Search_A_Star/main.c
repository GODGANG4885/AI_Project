#include <stdio.h>
#include "LinkedList.h"

//#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#define MoveRight 0
#define MoveDown 1
#define MoveLeft 2
#define MoveUp 3
#define TILE_SIZE 4
#define TRUE 1
#define FALSE 0


Ty_nodePtr Make_Node();
void Blk_Search(Ty_nodePtr ,int*,int*);
Ty_nodePtr Move(Ty_nodePtr, int , int , int); // 배열 좌표와 방향
int Is_Equal(Ty_nodePtr a, Ty_nodePtr b);
int Is_Exist(linknodePtr plist, Ty_nodePtr c); //Open, Close 리스트에 있는지 확인
int Is_Exist_Node(Ty_nodePtr G, Ty_nodePtr c); //G에 있는지 확인
Ty_nodePtr Compare_fhat(linknodePtr cl, Ty_node* n);
void Compute_hat(Ty_nodePtr* com, int lev, int* goal);
void Copy_Tile(Ty_nodePtr* a, Ty_nodePtr* b);
void Sort_Open(linknodePtr* open);
void Sort_Open(linknodePtr* open);
void Switching_Link(linknodePtr* a, linknodePtr* b);
//void PrintGraph(Ty_nodePtr node_N);
int Is_Goal(Ty_nodePtr ans, int* goal );
int Is_valid_MoveRight(int,int );
int Is_valid_MoveLeft(int, int);
int Is_valid_MoveUp(int, int);
int Is_valid_MoveDown(int, int);
//===============================Declare Data Structure===================================
int start[4][4];
int goal[4][4];
int* goalptr;
// Create New Node : malloc, 초기화
Ty_nodePtr Make_Node() {
	int i = 0, j = 0;

	Ty_nodePtr temp = (Ty_nodePtr)malloc(sizeof(Ty_node));
	//노드 내 타일 초기화.
	for (i = 0; i < TILE_SIZE; i++)
		for (j = 0; j < TILE_SIZE; j++)
			temp->tile[i][j] = 0;
	temp->fhat = 0;
	temp->ghat = 0;
	temp->hhat = 0;
	temp->next = NULL;
	temp->pred = NULL;

	return temp;
}


int main(void) {

	int blk_i = 0, blk_j = 0; 
	int i, j;
	int level = 0;
	
	Ty_nodePtr StartNode;
	linknodePtr Open = NULL;
	linknodePtr Close = NULL;
	linknodePtr List_M = NULL;
	Ty_nodePtr node_N=NULL;
	Ty_nodePtr G = NULL;
	Ty_nodePtr Succ;
	goalptr = goal;
//===========================Start node scnaf, goal node scanf===============================
		printf("시작노드를 입력하시오(1행 4개의 수) : ");
		scanf("%d %d %d %d", &start[0][0], &start[0][1], &start[0][2], &start[0][3]);
		printf("시작노드를 입력하시오(2행 4개의 수) : ");
		scanf("%d %d %d %d", &start[1][0], &start[1][1], &start[1][2], &start[1][3]);
		printf("시작노드를 입력하시오(3행 4개의 수) : ");
		scanf("%d %d %d %d", &start[2][0], &start[2][1], &start[2][2], &start[2][3]);
		printf("시작노드를 입력하시오(4행 4개의 수) : ");
		scanf("%d %d %d %d", &start[3][0], &start[3][1], &start[3][2], &start[3][3]);
		
		//시작노드 예외처리
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k<4; k++) {
					for(int l= 0;l<4;l++){
						if (i == k&&j == l)
							continue;
						if (start[i][j] == start[k][l]) {
							printf("\n시작노드 입력 오류\n");
							return -1;
						}
					}
				}
			}
		}
		printf("\n시작노드 정상 입력\n");
		printf("\n\n");
		printf("목표노드를 입력하시오(4행 4개의 수) : ");
		scanf("%d %d %d %d", &goal[0][0], &goal[0][1], &goal[0][2], &goal[0][3]);
		printf("목표노드를 입력하시오(4행 4개의 수) : ");
		scanf("%d %d %d %d", &goal[1][0], &goal[1][1], &goal[1][2], &goal[1][3]);
		printf("목표노드를 입력하시오(4행 4개의 수) : ");
		scanf("%d %d %d %d", &goal[2][0], &goal[2][1], &goal[2][2], &goal[2][3]);
		printf("목표노드를 입력하시오(4행 4개의 수) : ");
		scanf("%d %d %d %d", &goal[3][0], &goal[3][1], &goal[3][2], &goal[3][3]);
		//목표노드 예외처리
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k<4; k++) {
					for (int l = 0; l<4; l++) {
						if (i == k&&j == l)
							continue;
						if (goal[i][j] == goal[k][l]) {
							printf("\n목표노드 입력 오류\n");
							return -1;
						}
					}
				}
			}
		}
		
		printf("\n시작노드 \n");
		printf("%2d %2d %2d %2d\n", start[0][0], start[0][1], start[0][2], start[0][3]);
		printf("%2d %2d %2d %2d\n", start[1][0], start[1][1], start[1][2], start[1][3]);
		printf("%2d %2d %2d %2d\n", start[2][0], start[2][1], start[2][2], start[2][3]);
		printf("%2d %2d %2d %2d\n", start[3][0], start[3][1], start[3][2], start[3][3]);

		printf("\n목표노드 \n");
		printf("%2d %2d %2d %2d\n", goal[0][0], goal[0][1], goal[0][2], goal[0][3]);
		printf("%2d %2d %2d %2d\n", goal[1][0], goal[1][1], goal[1][2], goal[1][3]);
		printf("%2d %2d %2d %2d\n", goal[2][0], goal[2][1], goal[2][2], goal[2][3]);
		printf("%2d %2d %2d %2d\n", goal[3][0], goal[3][1], goal[3][2], goal[3][3]);

		//시작 노드 데이터 입력
	StartNode = Make_Node();
	for (i = 0; i < TILE_SIZE; i++)
		for (j = 0; j < TILE_SIZE; j++)
			StartNode->tile[i][j] = start[i][j];

	//Open리스트에 시작노드를 넣는다
	Queue_In(&Open, StartNode);
	

	while (1) {
		if (Open == NULL) {
			printf("Search is failed\n");
			break;
		}
		//Open에서 값 꺼냄.
		Queue_In(&Close, Open->nodePtr);
		node_N = Queue_Out(&Open);
		level = node_N->hhat;

		if (Is_Goal(node_N,&goal) == 0)		
		{
			printf("\n\nPrint Path \n\n");
			while (node_N->pred != NULL)
			{
				node_N->pred->next = node_N;
				node_N = node_N->pred;
			}
			while (node_N != NULL)
			{
				printf("l====[%d]=====l\n", node_N->hhat);
				printf("l%2d %2d %2d %2d l\n", node_N->tile[0][0], node_N->tile[0][1], node_N->tile[0][2], node_N->tile[0][3]);
				printf("l%2d %2d %2d %2d l\n", node_N->tile[1][0], node_N->tile[1][1], node_N->tile[1][2], node_N->tile[1][3]);
				printf("l%2d %2d %2d %2d l\n", node_N->tile[2][0], node_N->tile[2][1], node_N->tile[2][2], node_N->tile[2][3]);
				printf("l%2d %2d %2d %2d l\n", node_N->tile[3][0], node_N->tile[3][1], node_N->tile[3][2], node_N->tile[3][3]);
				printf("\n");
				node_N = node_N->next;
			}
			//PrintGraph(node_N);
			break;
		}
		
		Blk_Search(node_N, &blk_i, &blk_j);
		
		if (Is_valid_MoveRight(blk_i, blk_j) == TRUE) {
			Succ=Move(node_N, blk_i,  blk_j, MoveRight);
			if (Is_Exist_Node(G, Succ) == FALSE)		//G에 존재하지 않으면,
			{
				Compute_hat(&Succ, level + 1,goalptr);
				Queue_In(&List_M, Succ);
				//printf("valid RIGHT\n");
			}
		}
		if (Is_valid_MoveLeft(blk_i, blk_j) == TRUE) {
			Succ = Move(node_N, blk_i, blk_j, MoveLeft);
			if (Is_Exist_Node(G, Succ) == FALSE)		//G에 존재하지 않으면,
			{
				Compute_hat(&Succ, level + 1, goalptr);
				Queue_In(&List_M, Succ);
				//printf("valid Left\n");
			}
		}
		if (Is_valid_MoveUp(blk_i, blk_j) == TRUE) {
			Succ = Move(node_N, blk_i, blk_j, MoveUp);
			if (Is_Exist_Node(G, Succ) == FALSE)		//G에 존재하지 않으면,
			{
				Compute_hat(&Succ, level + 1, goalptr);
				Queue_In(&List_M, Succ);
				//printf("valid UP\n");
			}
		}
		if (Is_valid_MoveDown(blk_i, blk_j) == TRUE) {
			Succ = Move(node_N, blk_i, blk_j, MoveDown);
			if (Is_Exist_Node(G, Succ) == FALSE)		//G에 존재하지 않으면,
			{
				Compute_hat(&Succ, level + 1, goalptr);
				Queue_In(&List_M, Succ);
				//printf("valid DOWN\n");

			}
		}
		while (List_M != NULL)
		{
			//open과 close에 없으면 -> 가보지 모한 경로.
			if (Is_Exist(Open, List_M->nodePtr) == FALSE &&Is_Exist(Close, List_M->nodePtr) == FALSE)
			{
				List_M->nodePtr->pred = node_N;
				Queue_In(&Open, List_M->nodePtr);
			}
			//open에 있으면. 
			else if (Is_Exist(Open, List_M->nodePtr) == TRUE)
			{
				G = Compare_fhat(Open, List_M->nodePtr);
			}

			else
			{
				G = Compare_fhat(Close, List_M->nodePtr);
				G->pred = node_N;
			}
			List_M = List_M->next;
		}

		Sort_Open(&Open);
		
	}	
}


Ty_nodePtr Move(Ty_nodePtr Ty_nod, int blk_i, int blk_j, int k) {

	Ty_nodePtr cur = Make_Node();
	Copy_Tile(&cur, &Ty_nod);
	int temp;

	switch (k) {
	case MoveRight:  
		temp = cur->tile[blk_i][blk_j];
		cur->tile[blk_i][blk_j] = cur->tile[blk_i][blk_j + 1];
		cur->tile[blk_i][blk_j + 1] = temp;
		return cur;
		break;
	
	case MoveDown:	 
		temp = cur->tile[blk_i][blk_j];
		cur->tile[blk_i][blk_j] = cur->tile[blk_i + 1][blk_j];
		cur->tile[blk_i + 1][blk_j] = temp;
		return cur;
		break;
	
	case MoveLeft:	 
		temp = cur->tile[blk_i][blk_j];
		cur->tile[blk_i][blk_j] = cur->tile[blk_i][blk_j - 1];
		cur->tile[blk_i][blk_j - 1] = temp;
		return cur;
		break;
	
	case MoveUp: 	 
		temp = cur->tile[blk_i][blk_j];
		cur->tile[blk_i][blk_j] = cur->tile[blk_i - 1][blk_j];
		cur->tile[blk_i - 1][blk_j] = temp;
		return cur;
		break;
	
	}
}



void Blk_Search(Ty_nodePtr pt,int* blk_i, int* blk_j) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (pt->tile[i][j] == -1) {
				*blk_i = i;
				*blk_j = j;
				break;
			}
		}
	}

}
//움직일 수 있는지 체크하는 함수
int Is_valid_MoveRight(int i, int j) {
	if (j !=3)
		return 1;
	else
		return 0;
}
int Is_valid_MoveLeft(int i, int j) {
	if (j !=0)
		return 1;
	else
		return 0;
}
int Is_valid_MoveUp(int i, int j) {
	if (i !=0)
		return 1;
	else
		return 0;
}
int Is_valid_MoveDown(int i, int j) {
	if (i !=3 )
		return 1;
	else
		return 0;
}
//목표 타일과 불일치 타일 갯수 카운터 리턴
int Is_Goal(Ty_nodePtr ans, int* goalptr){
	int i = 0, j = 0;
	int count = 0;
	
	for (i = 0; i < TILE_SIZE; i++)
	{
		for (j = 0; j < TILE_SIZE; j++)
		{
			if (ans->tile[i][j] != goal[i][j])
				count++;
		}
	}

	return count;
}
//타일 복사
void Copy_Tile(Ty_nodePtr* a, Ty_nodePtr* b)
{
	Ty_nodePtr temp_a = *a;
	Ty_nodePtr temp_b = *b;
	int i = 0, j = 0;

	for (i = 0; i < TILE_SIZE; i++)
		for (j = 0; j < TILE_SIZE; j++)
			temp_a->tile[i][j] = temp_b->tile[i][j];
}
//타일 비교 같으면 TRUE 리턴
int Is_Equal(Ty_nodePtr a, Ty_nodePtr b)
{
	int i = 0, j = 0;
	int state = TRUE;
	for (i = 0; i < TILE_SIZE; i++)
	{
		for (j = 0; j < TILE_SIZE; j++)
		{
			if (a->tile[i][j] != b->tile[i][j])
			{
				state = FALSE;
				break;
			}
		}
	}

	return state;
}
//리스트에 있는지 확인
int Is_Exist(linknodePtr plist, Ty_nodePtr c)
{
	linknodePtr cursor = plist;
	int state = FALSE;
	if (cursor == NULL)
	{
		return FALSE;
	}
	else
	{
		while (cursor->next != NULL)
		{
			if (Is_Equal(cursor->nodePtr, c) == FALSE)
				cursor = cursor->next;
			else {
				state = TRUE;
				break;
			}


		}
	}
	return state;
}
//hat 계산
void Compute_hat(Ty_nodePtr* com, int lev,int* goalptr)
{
	Ty_nodePtr temp = *com;
	if (temp != NULL)
	{
		temp->ghat = Is_Goal(temp, goalptr);
		temp->hhat = lev;

		temp->fhat = temp->ghat + temp->hhat;
	}

}

Ty_nodePtr Compare_fhat(linknodePtr cl, Ty_node* n)
{
	linknodePtr temp = cl;
	Ty_nodePtr ret = n;

	while (temp == NULL)
	{
		if (Is_Equal(cl->nodePtr, n) == TRUE)
		{
			if (temp->nodePtr->fhat < ret->fhat)
				ret = cl->nodePtr;
		}

		temp = temp->next;
	}

	return ret;
}

int Is_Exist_Node(Ty_nodePtr G, Ty_nodePtr c)
{
	Ty_nodePtr temp = G;
	int state = FALSE;

	while (temp != NULL)
	{
		if (Is_Equal(temp, c) == TRUE)
		{
			state = TRUE;
			break;
		}
		else
			temp = temp->next;
	}
	return state;
}
void Sort_Open(linknodePtr* open)
{

	linknodePtr temp = *open;
	linknodePtr cursor = *open;

	while (cursor != NULL)
	{
		temp = cursor;
		while (temp != NULL)
		{
			if (cursor->nodePtr->fhat > temp->nodePtr->fhat)
			{
				Switching_Link(&cursor, &temp);

			}
			temp = temp->next;
		}
		cursor = cursor->next;
	}
}
void Switching_Link(linknodePtr* a, linknodePtr* b)
{
	linknodePtr aa = *a;
	linknodePtr bb = *b;
	linknodePtr temp = NULL;
	temp = aa;
	aa = bb;
	bb = temp;
}
/*void PrintGraph(Ty_nodePtr node_N) {
	Ty_nodePtr NextNode1, NextNode2, NextNode3;
	

	printf("\n\nFind Path \n\n");
	while (node_N->pred != NULL)
	{
		node_N->pred->next = node_N;
		node_N = node_N->pred;
	}
	NextNode1 = node_N->next;
	NextNode2 = NextNode1->next;
	NextNode3 = NextNode2->next;
	while (node_N != NULL && NextNode1 != NULL && NextNode2 != NULL && NextNode3 != NULL)
	{
		NextNode1 = node_N->next;
		NextNode2 = NextNode1->next;
		NextNode3 = NextNode2->next;

			printf("====[%d]===== l ====[%d]===== l ====[%d]===== l ====[%d]===== l\n ", node_N->hhat, NextNode1->hhat, NextNode2->hhat, NextNode3->hhat);
			printf("%2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l\n", 
				node_N->tile[0][0], node_N->tile[0][1], node_N->tile[0][2], node_N->tile[0][3],
				NextNode1->tile[0][0], NextNode1->tile[0][1], NextNode1->tile[0][2], NextNode1->tile[0][3],
				NextNode2->tile[0][0], NextNode2->tile[0][1], NextNode2->tile[0][2], NextNode2->tile[0][3],
				NextNode3->tile[0][0], NextNode3->tile[0][1], NextNode3->tile[0][2], NextNode3->tile[0][3]);
			printf("%2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l\n", 
				node_N->tile[1][0], node_N->tile[1][1], node_N->tile[1][2], node_N->tile[1][3],
				NextNode1->tile[1][0], NextNode1->tile[1][1], NextNode1->tile[1][2], NextNode1->tile[1][3],
				NextNode2->tile[1][0], NextNode2->tile[1][1], NextNode2->tile[1][2], NextNode2->tile[1][3],
				NextNode3->tile[1][0], NextNode3->tile[1][1], NextNode3->tile[1][2], NextNode3->tile[1][3]);
			printf("%2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l\n", 
				node_N->tile[2][0], node_N->tile[2][1], node_N->tile[2][2], node_N->tile[2][3],
				NextNode1->tile[2][0], NextNode1->tile[2][1], NextNode1->tile[2][2], NextNode1->tile[2][3],
				NextNode2->tile[2][0], NextNode2->tile[2][1], NextNode2->tile[2][2], NextNode2->tile[2][3],
				NextNode3->tile[2][0], NextNode3->tile[2][1], NextNode3->tile[2][2], NextNode3->tile[2][3]);
			printf("%2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l %2d %2d %2d %2d l\n", 
				node_N->tile[3][0], node_N->tile[3][1], node_N->tile[3][2], node_N->tile[3][3],
				NextNode1->tile[3][0], NextNode1->tile[3][1], NextNode1->tile[3][2], NextNode1->tile[3][3],
				NextNode2->tile[3][0], NextNode2->tile[3][1], NextNode2->tile[3][2], NextNode2->tile[3][3],
				NextNode3->tile[3][0], NextNode3->tile[3][1], NextNode3->tile[3][2], NextNode3->tile[3][3]);
			node_N = NextNode3->next;
		
	}


}*/