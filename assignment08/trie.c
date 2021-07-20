#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$'	// end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ... 사전의 몇 번째 엔트리에 해당하는지
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;
 
////////////////////////////////////////////////////////////////////////////////
TRIE *trieCreateNode(void);
void trieDestroy( TRIE *root);
void trieDestroy( TRIE *root);
int trieInsert( TRIE *root, char *str, int dic_index);
int trieSearch( TRIE *root, char *str);
void trieList( TRIE *root, char *dic[]);
void triePrefixList( TRIE *root, char *str, char *dic[]);
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void)
{
	TRIE * trie;
	trie = (TRIE *)malloc(sizeof (TRIE));
	trie -> index = -1;
	for(int i = 0; i<MAX_DEGREE; i++)
		trie->subtrees[i] = NULL;
	return trie;
}	

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root)
{
	int i;
	for( i = 0 ; i < MAX_DEGREE - 1 ; ++i)
	{
		if (root -> subtrees[i])
			trieDestroy(root -> subtrees[i]);
	}
	free(root);
}			

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index)
{
	int len = strlen(str);
	int exst = 0, subndx = 0;  

	for(int i = 0; i< len; i++)
	{	
		if(isupper(str[i])) str[i] = tolower(str[i]);
		else if(isalpha(str[i]) == 0 && str[len-1]!= EOW) return 0;
	}
	exst = trieSearch(root, str);
	if(exst!= -1) return 0;
	
	while(*str)
	{
		subndx = getIndex(*str);
		if(!root -> subtrees[subndx])
			root -> subtrees[subndx] = trieCreateNode();
	
		root = root -> subtrees[subndx];
		str++;
	}
	root -> index = dic_index;
	
	
	return 1;
}
		
/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str)
{
	char *tmp;
	TRIE *curr;
	if(!root) return -1;
	
	curr = root;
	for( tmp = str ; *tmp; tmp++)
	{
		if(isupper(*tmp)) *tmp = tolower(*tmp);
		curr = curr -> subtrees[getIndex(*tmp)];
		if(!curr) return -1;
	}
	if(curr -> index != -1) return curr -> index;
	else return -1;
}	
/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]) 
{
	if(!root) return;
	else
	{	
		if(root -> index != -1)
			printf("%s\n", dic[root -> index]);
		
		for(int i = 0; i < MAX_DEGREE-1; i++)
		{ 
			if (root->subtrees[i] != NULL)
				trieList(root -> subtrees[i], dic);
		}
	}
}
/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[])
{
	int subndx;
	if(!root) return;
	while(*str)
	{
		subndx = getIndex(*str);
		root = root -> subtrees[subndx];
		str++;
	}
	trieList(root, dic);
}	
	

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	int index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode();
	
	while (fscanf( fp, "%s", str) != EOF)
	{
		ret = trieInsert( trie, str, index);

		if (ret) 
			dic[index++] = strdup( str);
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search
		if (str[strlen(str)-1] == '*')
		{
			str[strlen(str)-1] = 0;
			triePrefixList( trie, str, dic);
		}
		// keyword search
		else
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		
		printf( "\nQuery: ");
	}
	
	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	
	return 0;
}



/*insert 재귀 - 실패
	if(dic_index < len)
	{
		subndx = getIndex(str[dic_index]);
		if(root -> subtrees[subndx] == NULL)
		{
			root -> subtrees[subndx] = trieCreateNode();
		}
			trieInsert(root->subtrees[subndx], str, dic_index+1);
	}
	else root -> index = dic_index;
	*/
	

	
	/* trieList
	for(int i = 0; i < MAX_DEGREE; i++)
	{
		if(root ->subtrees[i] != NULL)
		{
			if(root -> index != -1)
			printf("%s\n", dic[root -> index]);
		}
		trieList(root -> subtrees[i], root);
	}*/