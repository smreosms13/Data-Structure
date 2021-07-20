#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert
#include <string.h> // strlen

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *createTree( void)
{
	TREE *pTree = (TREE *)malloc( sizeof(TREE));
	pTree -> root = NULL;
	return pTree;
}

/* Deletes all data in tree and recycles memory
*/
void destroyTree( TREE *pTree);

static void _destroy( NODE *root)
{
	if(root == NULL) return;
	_destroy(root->left);
	_destroy(root->right);
	free(root);
}

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode( char ch)
{
	NODE *newNode = (NODE *)malloc(sizeof(NODE));
	if(newNode != NULL) 
	{	
		newNode -> data = ch;
		newNode -> left = NULL;
		newNode-> right = NULL;
	}
	return newNode;
}
/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree( char *expr, TREE *pTree)
{
	NODE *stackArr[MAX_STACK_SIZE];
	
	int top = -1;
	int lenExp = strlen(expr);
	int i, j, k;
	
	for( i = 0; i < lenExp ; i++)
	{		
		if(isdigit(expr[i])) 
		{
			pTree -> root = _makeNode( expr[i]);	
			stackArr[++top] = pTree->root;
		}
		else 
		{
			if(top <= 0){
				if(top == 0) _destroy(stackArr[top]);
				free(pTree -> root);
			}
			pTree -> root = _makeNode( expr[i]);	
			
			pTree -> root -> right = stackArr[top--];
			pTree -> root -> left = stackArr[top--];
			stackArr[++top] = pTree -> root;
		}
	}
	if (top > 0) 
	{
		for( j = top; j < 0 ; j--)
		{
			_destroy(stackArr[j]);
		}
		free(pTree->root);
		return 0;
	}
	else return 1;		
}	
	
	

/* Print node in tree using inorder traversal
*/
void traverseTree( TREE *pTree);

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse( NODE *root)
{
	if(root != NULL)
	{
		if(isdigit(root -> data)) printf("%c", root -> data); 
		else
		{
			printf("(");
			_traverse(root -> left);
			printf("%c", root -> data);
			_traverse(root->right);
			printf(")");
		}
	}
}
/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);

/* internal traversal function
*/
static void _infix_print( NODE *root, int level)
{
	if(root == NULL) return;

	level++;
	_infix_print(root -> right, level);

	printf("\n");
	for(int i = 1 ; i < level; i++) printf("\t");
	printf("%c\n", root -> data);
	
	_infix_print(root -> left, level);
}		
	

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix( char *expr)
{
	 float stackArr[MAX_STACK_SIZE];
	int top = -1;
	
	int i, oper1, oper2, value;
	int lenExp = strlen(expr);
	
	for( i = 0; i<lenExp; i++)
	{	
		if(isdigit(expr[i]))
		{
			stackArr[++top] =  expr[i] -'0';
		}
		else 
		{
			oper2 = stackArr[top--];
			oper1 = stackArr[top--];
			
			 switch(expr[i])
			{
				case '+': stackArr[++top] = oper1 + oper2; 
							break;
				case '-': stackArr[++top] = oper1 - oper2;
							break;
				case '*': stackArr[++top] = oper1 * oper2; 
							break;
				case '/': stackArr[++top] = oper1 / oper2; 
							break;
			}
		}	
	}
	return stackArr[top];
}
////////////////////////////////////////////////////////////////////////////////
void destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
}

////////////////////////////////////////////////////////////////////////////////
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree( TREE *pTree)
{
	_traverse(pTree->root);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];
	
	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf( stdout, "invalid expression!\n");
			destroyTree( tree);
			fprintf( stdout, "\nInput an expression (postfix): ");
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree(tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}