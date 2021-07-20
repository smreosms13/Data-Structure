#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
TREE *BST_Create( void);
void BST_Destroy( TREE *pTree);
int BST_Insert( TREE *pTree, int data);
int BST_Delete( TREE *pTree, int dltKey);
int *BST_Retrieve( TREE *pTree, int key);
void BST_Traverse( TREE *pTree);
void printTree( TREE *pTree);
//////////////////////////////////////////////////////////////////////////////

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void)
{
	TREE *pTree = (TREE *)malloc( sizeof(TREE));
	pTree -> root = NULL;
	return pTree;
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree)
{
	if(pTree == NULL) return;
	_destroy(pTree->root);
	pTree->root = NULL;
	free(pTree);
}
/* internal function (not mandatory)
*/
static void _destroy( NODE *root)
{
	if(root == NULL) return;
	
	_destroy(root->left);
	_destroy(root->right);
	free(root);
}
/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data)
{
	if(pTree == NULL) return 0;
	NODE *newPtr = _makeNode( data);
	if( newPtr == NULL) return 0;
	
	if(pTree -> root == NULL) pTree -> root = newPtr;
	else _insert( pTree -> root, newPtr);
	return 1;

}
/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr)
{
	if (newPtr -> data == root -> data)
	{
		if(root -> left == NULL) root -> left = newPtr;
		else root-> right = newPtr;
	}	
	else if(newPtr -> data < root -> data)
	{
		if(root -> left == NULL)
			root -> left = newPtr;
		else return _insert(root-> left, newPtr);
	}
	else {
		if(root -> right == NULL)
			root -> right = newPtr;
		else return _insert(root -> right, newPtr);
	}	
 }

NODE *_makeNode( int data)
{
	NODE *newNode = (NODE *)malloc(sizeof(NODE));
	if(newNode != NULL) 
	{	
		newNode -> data = data;
		newNode -> left = NULL;
		newNode-> right = NULL;
	}
	return newNode;
}
/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey)
{
	int success;
	NODE * newRoot;
	newRoot = _delete(pTree->root, dltKey, &success);
	
	if(success) {
		pTree->root = newRoot;
	}

	return success;
}	

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success)
{

	NODE *curPtr;		
	NODE *parentPtr; 
	NODE *exchPtr;
	
	curPtr = root; 
	parentPtr = NULL;
	
	if(!root) 
	{
		*success = 0;
		return NULL;
	}
	
	if( dltKey < root -> data)
		root->left = _delete(root->left, dltKey, success);
	
	else if(dltKey > root->data)
		root->right = _delete(root->right, dltKey, success);
	
	else 
	{
		
		if(!root->left) {

		  parentPtr = curPtr->right;
		  free(curPtr);
		  *success = 1;
		  return parentPtr;
		}
		else {
		  if(!root->right) 
		  {
			parentPtr = root->left;
			free(curPtr);
			*success = 1;
			return parentPtr;
		  }
		  else 
		  {
			exchPtr = root->right;
			
			while(exchPtr->left)
				exchPtr = exchPtr->left;
			
			root->data = exchPtr->data;
			root->right = _delete(root->right, exchPtr->data, success);
		  }
		}
  }
  
  return root;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key)
{
	NODE *pInt;
	pInt = _retrieve(pTree->root, key);
	return &(pInt -> data);
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key)
{
	if(!root) return NULL;
	else {
		if(root -> data < key)		
			return _retrieve(root->right, key);
		else if (root -> data > key)
			return _retrieve(root->left, key);
		else return root;
	}
}	
/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree)
{
	_traverse(pTree->root);
	
	return;
}
static void _traverse( NODE *root)
{
	if(root != NULL)
	{
		_traverse(root -> left);
		printf("%d ", root -> data);
		_traverse(root->right);
	}
}

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}
/* internal traversal function
*/
static void _infix_print( NODE *root, int level)
{
	if(root == NULL) return;

	level++;
	_infix_print(root -> right, level);

	for(int i = 1 ; i < level; i++) printf("\t");
	printf("%d\n", root -> data);
	
	_infix_print(root -> left, level);
}		
/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree)
{
	if (pTree -> root == NULL) return 1;
	else return 0;
}

int main( int argc, char **argv)
{
	TREE *tree;
	int data;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	fprintf( stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf( "%d", &numbers);

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
		
		fprintf( stdout, "%d ", data);
		
		// insert funtion call
		int ret = BST_Insert( tree, data);
		if (!ret) break;
 	}
	fprintf( stdout, "\n");
			
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		int ret = scanf( "%d", &num);
		if (ret != 1) break;
		
		ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		// print tree with right-to-left infix traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}


