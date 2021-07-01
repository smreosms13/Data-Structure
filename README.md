# Data-Structure
KU - Data Structure Implementation in C 

<contents>
1.Dynamic array
2.General List (Dynamic array + Ordered linear list) 
3.General List (Doubly linked list + Ordered linear list)
4.Binary expession tree
5.Binary Search Tree
6.Heap - int_heap/ADT_heap
7.AVL tree - string_avl
8.Trie - trie / premuterm_trie

1. Dynamic Array
// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함

[Excution] 
linear search : $ ./name -l *.txt > 출력파일명  
binary search : $ ./name -b *.txt > 출력파일명
-------------------------------------------------------------------------------------

2.General List (Dynamic array + Ordered linear list) 
//dynamic array와 실행결과 동일.  
//don't use "qsort" fuction C provided
[Excution] : $ ./name2 name2.txt > 출력파일명

---------------------------------------------------------------------------------------
3.General List (Doubly linked list + Ordered linear list)
//실행 파일은 주어진 입력 파일(name3.txt)에 포함된 이름(name)들을 doubly linked list에 순서를 유지하여 삽입한다. (사전순. 남여 구분 없음)
//만약 이미 리스트에 저장된 이름인 경우 빈도(freq)를 증가시킨다.
//파일 입력이 완료되면, 사용자로부터 메뉴를 선택받는다.
  
  <menu>
  Q : 실행 종료 (프로그램 종료 전에 리스트를 위해 할당한 메모리를 모두 해제해야 함)
  F : 화면에 (순방향으로) list를 출력한다. (이름(name)과 빈도(freq)를 탭(tab)문자로 구분하여 출력)
  B : 화면에 역방향으로 list를 출력한다. 
  S : 사용자가 입력한 문자열(이름)을 리스트에서 찾는다.
  D : 사용자가 입력한 문자열(이름)을 리스트에서 삭제한다.
  C : 리스트에 포함된 이름(노드)의 수를 출력한다.

[Excution] 
 ./name_dlist name3.txt
----------------------------------------------------------------------------------------------
4.Binary expession tree

<program process>
1. 사용자로부터 후위표기식(postfix expression)을 입력받는다. 
(단, 수식은 한자리 수(digit)와 사칙 연산(+, -, *, /)의 조합으로만 이루어져야 함. 연산자는 모두 이진(binary) 연산자라고 가정함
ex) 중위표기식 (1+2)*(5-3)의 후위표기식은 12+53-*임)
2. 후위표기식으로부터 수식트리(expression tree)를 만든다.
3. 수식트리로부터 중위표기식(infix expression)을 출력한다.
4. 수식트리를 화면에 출력한다.
5. 후위표기식의 값을 계산하여 출력한다.

[Excution] ./expression_tree
----------------------------------------------------------------------------------------------
5.Binary Search Tree
//왼쪽과 오른쪽 서브트리를 모두 가지고 있는 노드를 삭제하는 경우는 
오른쪽 서브트리의 가장 작은 값을 가진 노드가 삭제될 노드의 자리로 위치하도록 해야 함
[Excution] ./intbst

----------------------------------------------------------------------------------------------
6.Heap - int_heap/ADT_heap
[Excution] 
integer type : ./intheap
abstract type : ./adttheap
----------------------------------------------------------------------------------------------
7.AVL tree - string_avl
<setting> 
#define SHOW_STEP 0
#define BALANCING 1

[Excution] 
./stravlt words_ordered.txt
----------------------------------------------------------------------------------------------
8.Trie - trie / premuterm_trie
need to implement the fuction for wildcard search
[Excution] 
trie : $ ./trie words_ordered.txt
permuter_trie : $ ./permuterm_trie words_ordered.txt
