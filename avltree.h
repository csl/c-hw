
#define DAY 0
#define MONTH 1
#define YEAR 2

#define PROGRAM 0
#define INSERT 1
#define DELETE 2
#define SHOW 3
#define QUIT 4

typedef unsigned int DATE[3];

typedef struct tree_node 
{
	DATE expdate;
	int bf;
	int h;
	struct tree_node *parent;
	struct tree_node *leftChild;
	struct tree_node *rightChild;
}tree_node;

struct tree_node *AVL_TREE;
struct tree_node *root;

#define IsLc(X) ((X) == (X)->parent->leftChild)
#define IsRc(X) ((X) == (X)->parent->rightChild)

short Max(short, short);
short Height(struct tree_node*);

struct tree_node *AVLMax(struct tree_node*);
struct tree_node *AVLMin(struct tree_node*);
struct tree_node *AVLFind(int);
struct tree_node *AVLLL(struct tree_node*);
struct tree_node *AVLRR(struct tree_node*);
struct tree_node *AVLLR(struct tree_node*);
struct tree_node *AVLRL(struct tree_node*);

void AVLinit();
void AVLFree(struct tree_node*);
void AVLPrint(struct tree_node*);
void AVLBalance(struct tree_node*, struct tree_node*);
void InsAVL(int);
void DelAVL(int);

