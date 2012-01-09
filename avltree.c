#include<stdio.h>

#include"avltree.h"

short Max(short x, short y) 
{
    return x > y ? x : y;
}

short Height(struct tree_node *node) 
{
    if(node == AVL_TREE)  return -1;

    return node->h;
}

void AVLinit() {
    AVL_TREE = (struct tree_node*) malloc(sizeof(struct tree_node));
    root = AVL_TREE->parent = AVL_TREE->leftChild = AVL_TREE->rightChild = AVL_TREE;
}

void AVLFree(struct tree_node *now) {
    if(now == AVL_TREE)    return;
    if(now->leftChild != AVL_TREE)    AVLFree(now->leftChild);
    if(now->rightChild != AVL_TREE)    AVLFree(now->rightChild);
    free(now);
}

void AVLPrint(struct tree_node *now) {
    if(now == AVL_TREE)    return;
    if(now->leftChild != AVL_TREE)    AVLPrint(now->leftChild);
    printf("%d -> ", now->v);
    if(now->rightChild != AVL_TREE)    AVLPrint(now->rightChild);
}

struct tree_node *AVLMax(tree_node *node) 
{
    if(node == AVL_TREE || node->rightChild == AVL_TREE)    
		return node;

    return AVLMax(node->rightChild);

}

struct tree_node *AVLMin( tree_node *node) 
{
    if(node == AVL_TREE || node->leftChild == AVL_TREE)    
		return node;

    return AVLMin(node->leftChild);
}

struct tree_node *AVLLL(tree_node *node) 
{
    
	if(node == AVL_TREE)    return AVL_TREE;
 
	struct tree_node *lc = node->leftChild;

    if(node->parent != AVL_TREE) {
        if(IsLc(node))    
			node->parent->leftChild = lc;
        else    
			node->parent->rightChild = lc;
    }
    node->leftChild = lc->rightChild;
    lc->rightChild = node;
    lc->parent = node->parent;
    node->parent = lc;
    node->leftChild->parent = node;
    node->h = Max(Height(node->leftChild), Height(node->rightChild))+1;
    lc->h = Max(Height(lc->leftChild), Height(lc->rightChild))+1;
  
    if(lc->parent == AVL_TREE) root = lc;
  
    return lc;
}

struct tree_node *AVLRR(tree_node *node) 
{
    if(node == AVL_TREE) return AVL_TREE;
    struct tree_node *rc = node->rightChild;

    if(node->parent != AVL_TREE) {
        if(IsRc(node))    
			node->parent->rightChild = rc;
        else    
			node->parent->leftChild = rc;
    }

    node->rightChild = rc->leftChild;
    rc->leftChild = node;
    rc->parent = node->parent;
    node->parent = rc;
    node->rightChild->parent = node;
    node->h = Max(Height(node->leftChild), Height(node->rightChild))+1;
    rc->h = Max(Height(rc->leftChild), Height(rc->rightChild))+1;
  
    if(rc->parent == AVL_TREE) root = rc;
  
    return rc;
}

struct tree_node *AVLLR(tree_node *node) 
{
    node->leftChild = AVLRR(node->leftChild);

    return AVLLL(node);
}

struct tree_node *AVLRL(tree_node *node) 
{
    node->rightChild = AVLLL(node->rightChild);
    return AVLRR(node);
}

void AVLBalance(struct tree_node *node, tree_node *dest) {
    if(node == dest)    return;

    while(node != dest) 
	{
        if(Height(node->leftChild) - Height(node->rightChild) == 2) {

            if(Height(node->leftChild->leftChild) >= Height(node->leftChild->rightChild))
                node = AVLLL(node);
            else    
				node = AVLLR(node);

            node->h = Max(Height(node->leftChild), Height(node->rightChild))+1;
        }
        else if(Height(node->rightChild) - Height(node->leftChild) == 2) {
            if(Height(node->rightChild->rightChild) >= 
								Height(node->rightChild->leftChild))
                node = AVLRR(node);
            else 
				node = AVLRL(node);

            node->h = Max(Height(node->leftChild), Height(node->rightChild))+1;
        }
        else {
            node->h = Max(Height(node->leftChild), Height(node->rightChild))+1;
            node = node->parent;
        }
    }
}

void InsAVL(int key) 
{
    struct tree_node *curr = root, *prev = AVL_TREE;
    while(curr != AVL_TREE) {
        prev = curr;
        if(curr->v > key)    
			curr = curr->leftChild;
        else if(curr->v < key)    
			curr = curr->rightChild;
        else return;
    }

    curr = (struct tree_node*) malloc(sizeof(struct tree_node));
    curr->v = key, curr->parent = prev, curr->h = 0;
    curr->leftChild = curr->rightChild = AVL_TREE;
  
    if(prev == AVL_TREE)    
		root = curr;
    else if(prev->v > key)    
		prev->leftChild = curr;
    else    
		prev->rightChild = curr;

    AVLBalance(curr, AVL_TREE);
}

void DelAVL(int key) 
{
    struct tree_node *node = AVLFind(key);
    
	if(node == AVL_TREE)  return;
    
	struct tree_node *temp = AVLMin(node->rightChild);
    
	if(temp == AVL_TREE) 
	{
        struct tree_node *tmpp = node->parent;
        if(node->parent == AVL_TREE) root = node->leftChild;
        if(IsLc(node))    
			node->parent->leftChild = node->leftChild;
        else    
			node->parent->rightChild = node->leftChild;

        node->parent->h = Max(Height(node->parent->leftChild), 
						  Height(node->parent->rightChild))+1;
        node->leftChild->parent = node->parent;
        AVLBalance(tmpp, AVL_TREE);
    }
    else {
        struct tree_node *tmpp = temp->parent;       

        if(node->parent == AVL_TREE) root = temp;

        if(IsLc(node))    
			node->parent->leftChild = temp;
        else    
			node->parent->rightChild = temp;

        if(temp != node->rightChild) {
            temp->rightChild->parent = temp->parent;
            temp->parent->leftChild = temp->rightChild;
            temp->rightChild = node->rightChild;
            node->rightChild->parent = temp;
        }

        node->leftChild->parent = temp;

        temp->leftChild = node->leftChild, temp->parent = node->parent;
        if(tmpp != node)    
			AVLBalance(tmpp, AVL_TREE);
    }
    free(node);
}

struct tree_node *AVLFind(int key) 
{
    struct tree_node *curr = root;

    while(curr != AVL_TREE) {
        if(curr->v > key)    
			curr = curr->leftChild;
        else if(curr->v < key)    
			curr = curr->rightChild;
        else return curr;
    }

    return AVL_TREE;
}

int main(void)
{
    char cmdline[255];
    char op[255], cvalue[255];
    char *pch;
    char symbol_table[255][255];
    int csymbol=0;
    int n_v = 0;
    int count=0;
    int code=1;

    while (1)
    {
        printf("> ");
        gets(cmdline);
        pch = strtok(cmdline, " ");
        count=0;
        code=1;
        while (pch != NULL)
        {
            if (count == 0)
            {
                if (!strcmp("avl",pch))
                    code = PROGRAM;
                else if (!strcmp("I",pch))
                    code = INSERT;
                else if (!strcmp("D",pch))
                    code = DELETE;
                else if (!strcmp("T",pch))
                    code = SHOW;
                else if (!strcmp("q",pch))
                    code = QUIT;
            	else
            	{
                	printf("no command\n");
                	break;
            	}

            }
            else if (count == 1)
            {
           		switch (code)
            	{
            		case INSERT:
            		case DELETE:
                		strcpy(op, pch);
                		break;
            		case SHOW:
            		case QUIT:
            		default:
                		printf("no command\n");
                	break;

            	}
            }
        	else
        	{
                printf("no command\n");
                break;
			}
        
     	pch = strtok (NULL, " ");
     	count++;
    	}

    	switch (code)
    	{
        	case PROGRAM:
              	if (count != 1) break;
                printf("Welcome to AVL!\n");
                break;
        	case INSERT:
                if (count != 2) break;
                break;
        	case DELETE:
                if (count != 2) break;
                break;
        	case SHOW:
                if (count != 1) break;
                break;
        	case QUIT:
                if (count != 1) break;
				exit(1);
    }
  }

  return 0;
}
