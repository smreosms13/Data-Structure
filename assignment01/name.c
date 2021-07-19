#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간
#define LINEAR_SEARCH 0
#define BINARY_SEARCH 1

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

////////////////////////////////////////////////////////////////////////////////
// 함수 원형 선언(declaration)

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 2배씩 증가

// 선형탐색(linear search) 버전
void load_names_lsearch( FILE *fp, int year_index, tNames *names);

// 이진탐색(binary search) 버전
void load_names_bsearch( FILE *fp, int year_index, tNames *names);

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year);

// qsort를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare( const void *n1, const void *n2);

																															 
////////////////////////////////////////////////////////////////////////////////
// 함수 정의 (definition)

// 이름 구조체를 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc( sizeof(tNames));
	
	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}

//선형탐색(linear search) 버전
void load_names_lsearch( FILE *fp, int year_index, tNames *names)
{	
	int i,overlap;
	char *c;
	char tmp[100];
	
	char n[20];
	char s;
	int f;
	
	
	while(!feof(fp))
	{	
		fgets(tmp, sizeof(tmp), fp);
		for(c = tmp; ; c++)
		{
			if(!*c) 
				break;
			if(*c == ',') 
				*c = ' ';
		}
		sscanf(tmp, "%s %c %d", n, &s, &f);
		
		
		if(year_index > 0)
		{
			for (i=0; i< names->len; i++)
			{
				if (strcmp(n, names->data[i].name)==0 && (s == names->data[i].sex))
				{
					names -> data[i].freq[year_index] = f;
					overlap = 1;
					break;
				}
			}
		}	
		if(names->capacity == names->len)
		{
			names->capacity *= 2;
			names->data = (tName*)realloc(names->data, names->capacity * sizeof(tName));
			
			assert(names->data != NULL);
			if(!names->data)
				{
					fprintf(stderr, "Memory capacity warning!");
					return;
				}
		}
		if(year_index ==0 || !overlap) {
			strcpy(names->data[names->len].name, n);
			names->data[names->len].sex = s;
			names->data[names->len++].freq[year_index] = f;
		}
	}
	
}


// 이진탐색(binary search) 버전
void load_names_bsearch(FILE *fp, int year_index, tNames *names)
{	
	
	char *c = 0;
	char tmp[100];
	
	int *key = 0;
	tName tmp_str;
	size_t size = 64; 
	
	while(!feof(fp))
	{		
		fgets(tmp, sizeof(tmp), fp);
		
		for(c = tmp; ; c++)
		{
			if(!*c)
				break;
			if(*c == ',')
				*c = ' ';
		}
		sscanf(tmp, "%s %c %d", tmp_str.name, &tmp_str.sex, &tmp_str.freq[0]);
		
		if(names->capacity == names->len)
			{	
				names->capacity *= 2;
				names->data = (tName*)realloc(names->data, names->capacity * sizeof(tName));
				
				assert(names->data != NULL);
				if(!names->data)
				{
					fprintf(stderr, "Memory capacity warning!");
					return;
				}
			}
			
		if(year_index == 0) {
			strcpy(names->data[names->len].name, tmp_str.name);
			names->data[names->len].sex = tmp_str.sex;
			memset(names->data[names->len].freq, 0, MAX_YEAR_DURATION*4);
			names->data[names->len++].freq[year_index] = tmp_str.freq[0];
		}			
		else if(year_index > 0) 
		{
			key = bsearch(tmp_str.name, names->data, names->len, size, compare);
		
			if(key)	
			{
				key[year_index + 6] = tmp_str.freq[0];
			}
			else
			{
				strcpy(names->data[names->len].name, tmp_str.name);
				names->data[names->len].sex = tmp_str.sex;
				memset(names->data[names->len].freq, 0, MAX_YEAR_DURATION*4);
				names->data[names->len++].freq[year_index] = tmp_str.freq[0];
			}
		}
		key = 0;
	}
}

// 구조체 배열을 화면에 출력
void print_names(tNames *names, int num_year)
{
	int i, j;
	
	for(i = 0; i < names->len; i++)
	{
		printf("%s\t%c", names->data[i].name, names->data[i].sex);
		for(j = 0; j < num_year; j++)
			printf("\t%d", names->data[i].freq[j]);
		printf("\n");
	}
}

// qsort를 위한 비교 함수
int compare( const void *n1, const void *n2)
{
	int c_result;
	c_result = strcmp((const char *)n1, (const char *)n2);
	
	if(!c_result)
	{			
		c_result = (unsigned int)(*((char *)n1 + 20) - *((char *)n2 + 20));
		return c_result;
	}	
	else														   
		return c_result;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	tNames *names;
	int mode;
	
	FILE *fp;
	int num_year = 0;
	
	if (argc <= 2)
	{
		fprintf( stderr, "Usage: %s mode FILE...\n\n", argv[0]);
		fprintf( stderr, "mode\n\t-l\n\t\twith linear search\n\t-b\n\t\twith binary search\n");
		return 1;
	}
	
	if (strcmp( argv[1], "-l") == 0) mode = LINEAR_SEARCH;
	else if (strcmp( argv[1], "-b") == 0) mode = BINARY_SEARCH;
	else {
		fprintf( stderr, "unknown mode : %s\n", argv[1]);
		return 1;
	}
	
	// 이름 구조체 초기화
	names = create_names();

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = atoi( &argv[2][strlen(argv[2])-8]);
	
	for (int i = 2; i < argc; i++)
	{
		num_year++;
		fp = fopen( argv[i], "r");
		if( !fp) {
			fprintf( stderr, "cannot open file : %s\n", argv[i]);
			return 1;
		}

		int year = atoi( &argv[i][strlen(argv[i])-8]); // ex) "yob2009.txt" -> 2009
		
		fprintf( stderr, "Processing [%s]..\n", argv[i]);
		
		if (mode == LINEAR_SEARCH)
		{
			// 연도별 입력 파일(이름 정보)을 구조체에 저장
			// 선형탐색 모드
			load_names_lsearch( fp, year-start_year, names);
		
		}
		else // (mode == BINARY_SEARCH)
		{
			// 이진탐색 모드
			load_names_bsearch( fp, year-start_year, names);
			
			// 정렬 (이름순 (이름이 같은 경우 성별순))
			qsort( names->data, names->len, sizeof(tName), compare);
		}
		fclose( fp);

	}
	
	// 정렬 (이름순 (이름이 같은 경우 성별순))
	qsort( names->data, names->len, sizeof(tName), compare);
	
	// 이름 구조체를 화면에 출력
	print_names( names, num_year);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 0;
}

