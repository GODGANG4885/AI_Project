#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef int TileBlk[4][4]; //�� ���� -1
typedef struct anode* Ty_nodePtr;
typedef struct anode {
	TileBlk tile;
	int fhat, ghat, hhat;
	Ty_nodePtr next; //����Ʈ ������� ������
	Ty_nodePtr pred; //�θ��� ������ 
}Ty_node;
typedef struct linknode* linknodePtr;
typedef struct linknode {
	Ty_nodePtr nodePtr;
	Ty_nodePtr next;
}Ty_linklist;

linknodePtr Make_Link();
Ty_nodePtr Queue_Out(linknodePtr* plist);
void Queue_In(linknodePtr* plist, Ty_nodePtr pnode);

#endif