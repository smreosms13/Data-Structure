#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y)) // node  heignt 계산

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height; // null tree = 0, leaf tree = 1
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
AVL_TREE *AVL_Create( void);
void AVL_Destroy( AVL_TREE *pTree);
int AVL_Insert( AVL_TREE *pTree, char *data);
char *AVL_Retrieve( AVL_TREE *pTree, char *key);
void AVL_Traverse( AVL_TREE *pTree);
void printTree( AVL_TREE *pTree);

static void _destroy( NODE *root);
static NODE *_insert( NODE *root, NODE *newPtr);
static NODE *_makeNode( char *data);
static NODE *_retrieve( NODE *root, char *key);
static void _traverse( NODE *root);
static void _infix_print( NODE *root, int level);
static int getHeight( NODE *root);
static NODE *rotateRight( NODE *root);
static NODE *rotateLeft( NODE *root);
///////////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void)
{
	AVL_TREE *pTree;
	pTree = (AVL_TREE *)malloc( sizeof(AVL_TREE));
	
	if(pTree)
	{
		pTree -> root = NULL;
		pTree-> count = 0;
	}	
	return pTree;
}
/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree)
{	
	if(pTree) 
		_destroy(pTree->root);
	free(pTree);
}	
static void _destroy( NODE *root)
{
	if(root)
	{
		_destroy(root->left);
		free(root->data);
		_destroy(root->right);
		free(root);
	}
	return;
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data)
{	
	
	if(!pTree) return 0; 
	NODE *newPtr = _makeNode( data);
	if(!newPtr) return 0;
	
	if(pTree -> root == NULL) 
		pTree -> root = newPtr;
	else 
		pTree -> root = _insert( pTree -> root, newPtr);	
	
	pTree -> count++;
	return 1;
}
/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr)
{
	if (!root) 
	{	
		root = newPtr;
		return root;
	}
	else
	{
		if(strcmp(root -> data, newPtr->data) > 0)
		{	
			root->left = _insert(root->left, newPtr);
			if(getHeight(root->left) - getHeight(root->right) > 1)
			{	
				if(strcmp(root -> left -> data, newPtr->data) > 0)
					root = rotateRight(root);
				else 
				{
					root -> left =  rotateLeft(root->left);
					root = rotateRight(root);
				}
			}		
		}
		else
		{
			root->right = _insert(root->right, newPtr);
			if(getHeight(root->right) - getHeight(root->left) > 1)	
			{
				if(strcmp(root -> right ->data, newPtr->data) < 0)
					root = rotateLeft(root);
				else 
					{
						root -> right =  rotateRight(root->right);
						root = rotateLeft(root);
					}	
			}
		}	
	}	
	root-> height = 1 + max(getHeight(root->left), getHeight(root->right));
	return root;
}

static NODE *_makeNode( char *data)
{
	NODE *newPtr = (NODE *)malloc(sizeof(NODE));
	if(newPtr)
	{
		newPtr -> left  = NULL;
		newPtr -> right = NULL;
		newPtr -> height = 1;
		newPtr -> data = strdup(data);
	}
	return newPtr;
}
/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key)
{
	NODE * pKey;
	pKey = _retrieve(pTree -> root, key);
	if(pKey)
		return pKey -> data;
	else return NULL;
}
/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key)
{	 
	if(!root) return NULL; 
	else
	{
		if(strcmp(root-> data, key) < 0)		
			return _retrieve(root->right, key);
		else if (strcmp(root-> data, key) > 0)
			return _retrieve(root->left, key);
		else return root;
	}
}
/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree)
{
	_traverse(pTree->root);
	
	return;
}

static void _traverse( NODE *root)
{
	if(root != NULL)
	{
		_traverse(root -> left);
		printf("%s", root -> data);
		_traverse(root->right);
	}
}
/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree)
{
	_infix_print(pTree ->root, 0);	
	return;
}
/* internal traversal function
*/
static void _infix_print( NODE *root, int level)
{
	if(root != NULL)
	{
	_infix_print(root -> right, level+1);
	for(int i = 0 ; i < level ; i++) 
		printf("\t");
	printf("%s\n", root -> data);
	_infix_print(root -> left, level+1);
	}
}		

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root)
{
	if(!root)
		return 0;
	else
		return root -> height;
}	

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root)
{
	NODE * holdPtr;
	
	holdPtr = root -> left;
	root -> left = holdPtr -> right;
	holdPtr ->right = root;
	
	root-> height = 1 + max(getHeight(root->left), getHeight(root->right));
	holdPtr -> height = 1 + max(getHeight(holdPtr->left), getHeight(holdPtr->right));
	
	return holdPtr;
}	

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root)
{
	NODE * holdPtr;
	
	holdPtr = root -> right;
	root -> right = holdPtr -> left;
	holdPtr ->left = root;
	
	root-> height = 1 + max(getHeight(root->left), getHeight(root->right));
	holdPtr -> height = 1 + max(getHeight(holdPtr->left), getHeight(holdPtr->right));
	
	return holdPtr;

}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	//destroy tree
	AVL_Destroy( tree);

	return 0;
}

