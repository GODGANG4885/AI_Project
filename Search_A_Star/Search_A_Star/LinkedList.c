#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

void Queue_In(linknodePtr* plist, Ty_nodePtr pnode)
{
	linknodePtr insert = Make_Link();
	insert->nodePtr = pnode;
	linknodePtr temp = *plist;

	if (temp == NULL)		//리스트가 비어있다면
	{
		*plist = insert;
	}
	else
	{
		while (1)
		{
			if (temp->next != NULL)
				temp = temp->next;
			else
			{
				temp->next = insert;
				break;
			}

		}
	}
}

Ty_nodePtr Queue_Out(linknodePtr* plist)
{
	linknodePtr temp = *plist;
	if (temp == NULL)
		return NULL;
	else
	{
		*plist = temp->next;
		return temp->nodePtr;
	}
}


linknodePtr Make_Link()
{
	linknodePtr temp = (linknodePtr)malloc(sizeof(Ty_linklist));
	temp->next = NULL;
	return temp;
}


/*
void ListInit(linknodePtr plist) {

	plist = (linknodePtr)malloc(sizeof(Ty_linklist)); // 리스트 메모리 할당
	plist->nextlink = plist;
}

void LInsert(linknodePtr* plist, Ty_Ty_nodePtr pnode) {
	
	if (*plist == NULL) 
		(*plist)->nodePtr = pnode;
	else
	{
		while (1) {
			if ((*plist)->nodePtr->next == NULL) {
				(*plist)->nodePtr->next = pnode;
				(*plist)->nextlink = pnode;
			}
			else {
				(*plist)->nodePtr = (*plist)->nodePtr->next;
				break;
			}
		}
	}
}
Ty_nodePtr LOut(linknodePtr* plist) {

	if (*plist == NULL)
		return NULL;
	else {
		*plist = (*plist)->nextlink;
		return (*plist)->nodePtr;
	}
}
*/

