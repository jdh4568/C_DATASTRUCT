#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*
    ===== 연결 리스트(원형 리스트) 기반 다항식 연산 프로그램 =====

    [표현 방식]
    - 다항식은 "원형 연결 리스트(circular linked list)"로 표현한다.
    - 각 노드(polyNode)는 하나의 항(term)을 의미:
        coef  : 계수
        expon : 지수
        link  : 다음 항을 가리키는 포인터
    - 헤더 노드(header)는 실제 항이 아니라, 리스트의 시작을 표시하는 "더미 노드" 역할
        header->expon = -1 로 두어 구분한다.
        마지막 노드의 link는 다시 header를 가리켜 원형 구조가 된다.

    [정렬/전제]
    - cpadd(덧셈)는 두 다항식이 "지수(expon)가 내림차순(큰 지수 -> 작은 지수)"으로
      정렬되어 들어온다는 전제를 갖는다.
      (입력도 그 순서로 하거나, 별도의 정렬 로직이 필요)

    [메모리 관리]
    - avail: free list(사용 가능한 노드들의 연결 리스트)
      retNode()로 반환된 노드를 avail에 모아두고,
      getNode()는 avail에서 꺼내거나 없으면 malloc으로 새로 할당한다.
      => malloc/free 호출을 줄여 효율적으로 노드를 재사용하려는 목적
*/

typedef struct polyNode* polyPointer;

/* 다항식 노드(항) 구조체 */
typedef struct polyNode {
    float coef;          // 계수 coefficient
    int expon;           // 지수 exponent
    polyPointer link;    // 다음 노드
} polyNode;

/* free list의 헤드 포인터 (재사용 가능한 노드들이 연결된 리스트) */
polyPointer avail = NULL;

/* 함수 원형(프로토타입) */
polyPointer getNode(void);
void retNode(polyPointer node);
void cerase(polyPointer* ptr);
void attach(float coefficient, int exponent, polyPointer* last);
int compare(int x, int y);
polyPointer cpadd(polyPointer a, polyPointer b);
polyPointer createPoly();
void printPoly(polyPointer p);
polyPointer cpmul(polyPointer a, polyPointer b);

/*
    ===== getNode =====
    - 새로운 노드를 하나 얻어오는 함수
    - avail에 재사용 가능한 노드가 있으면 그것을 꺼내서 사용
    - avail이 비어 있으면 malloc으로 새 노드 생성
*/
polyPointer getNode(void)
{
    polyPointer node;

    if (avail) {
        // free list에서 하나 꺼냄
        node = avail;
        avail = avail->link;
    }
    else {
        // free list가 비어있으면 새로 할당
        node = (polyPointer)malloc(sizeof(polyNode));
    }
    return node;
}

/*
    ===== retNode =====
    - 사용이 끝난 노드를 free list(avail)에 반납하는 함수
    - node를 avail의 맨 앞에 연결해서 재사용 가능 상태로 만든다.
*/
void retNode(polyPointer node)
{
    node->link = avail;
    avail = node;
}

/*
    ===== cerase =====
    - 원형 연결 리스트로 된 다항식을 모두 지우는 함수
    - ptr은 "헤더 노드 포인터의 주소"를 받는다 (지운 뒤 NULL로 만들기 위해)

    동작:
    1) 헤더 노드가 가리키는 첫 항부터 순회
    2) 다시 헤더로 돌아오기 전까지 모든 항 노드를 retNode로 반납
    3) 마지막에 헤더 노드도 retNode로 반납
    4) *ptr = NULL로 설정해 다항식이 비어있음을 표시
*/
void cerase(polyPointer* ptr)
{
    polyPointer p, q;

    // 이미 NULL이면 지울 것이 없음
    if (!*ptr) return;

    // 헤더 다음 노드(첫 항)부터 시작
    p = (*ptr)->link;

    // 원형 리스트이므로, p가 다시 헤더(*ptr)를 만나면 종료
    while (p != *ptr) {
        q = p->link;   // 다음 노드 저장
        retNode(p);    // 현재 노드를 free list로 반납
        p = q;         // 다음으로 이동
    }

    // 헤더 노드도 반납
    retNode(*ptr);
    *ptr = NULL;
}

/*
    ===== attach =====
    - 다항식의 "마지막 뒤"에 새 항을 붙이는 함수
    - last는 현재 마지막 노드를 가리키는 포인터의 주소(갱신해야 하므로)

    사용 방법:
    - attach(coef, expon, &last);
    - 내부에서 last->link = 새노드, last = 새노드 로 갱신된다.

    주의:
    - attach는 "정렬"을 해주지 않는다.
      호출자가 올바른 순서(지수 내림차순 등)로 붙여줘야 한다.
*/
void attach(float coefficient, int exponent, polyPointer* last)
{
    polyPointer temp = getNode();  // 노드 확보(재사용 or malloc)
    temp->coef = coefficient;
    temp->expon = exponent;

    // 현재 마지막 노드의 다음에 temp를 연결
    (*last)->link = temp;

    // last를 새로 붙인 노드로 갱신
    *last = temp;
}

/*
    ===== compare =====
    두 정수(x, y)를 비교
    - x < y  -> -1
    - x == y ->  0
    - x > y  ->  1

    cpadd에서 지수 비교에 사용된다.
*/
int compare(int x, int y)
{
    if (x < y) return -1;
    else if (x > y) return 1;
    else return 0;
}

/*
    ===== cpadd : 두 다항식 덧셈 =====
    a, b: 원형 연결 리스트로 표현된 다항식(헤더 노드 포인터)

    전제:
    - a와 b의 항들이 "지수(expon) 내림차순"으로 정렬되어 있다고 가정
      (그래야 merge처럼 한 번에 덧셈 가능)

    반환:
    - 덧셈 결과 다항식의 헤더 노드 포인터(c)

    핵심 아이디어:
    - a와 b를 동시에 한 칸씩 이동하면서 지수를 비교
      * 지수가 큰 쪽 항을 결과에 그대로 붙임
      * 지수가 같으면 계수를 더해서 0이 아니면 붙임
    - 원형 리스트이므로, a가 startA로 돌아오면(한 바퀴) done=1로 종료
*/
polyPointer cpadd(polyPointer a, polyPointer b)
{
    polyPointer startA, c, lastC;
    float sum;
    int done = 0;

    // 원형 리스트에서 "시작점(헤더)" 저장
    startA = a;

    // 실제 항은 헤더 다음부터 시작
    a = a->link;
    b = b->link;

    // 결과 다항식 헤더 생성
    c = getNode();
    c->expon = -1;   // 헤더 표시
    lastC = c;       // 현재 결과 리스트의 마지막 노드(초기에는 헤더)

    do {
        switch (compare(a->expon, b->expon))
        {
        case -1:
            /*
                a의 지수 < b의 지수:
                => b의 항이 더 "큰 지수"이므로 결과에 b 항을 그대로 붙이고 b 이동
            */
            attach(b->coef, b->expon, &lastC);
            b = b->link;
            break;

        case 0:
            /*
                지수가 같음:
                - 원형 리스트에서 a가 다시 startA(헤더)로 왔다면
                  한 바퀴를 다 돈 것이므로 종료 처리(done=1)
                - 그게 아니면 계수 합(sum)을 구해서 0이 아니면 결과에 추가
                  그리고 a, b 둘 다 다음으로 이동
            */
            if (startA == a)
                done = 1;
            else {
                sum = a->coef + b->coef;
                if (sum != 0)
                    attach(sum, a->expon, &lastC);
                a = a->link;
                b = b->link;
            }
            break;

        case 1:
            /*
                a의 지수 > b의 지수:
                => a의 항이 더 큰 지수이므로 결과에 a 항을 그대로 붙이고 a 이동
            */
            attach(a->coef, a->expon, &lastC);
            a = a->link;
            break;
        }
    } while (!done);

    // 결과 리스트를 원형으로 만들기: 마지막 노드가 헤더를 가리키게
    lastC->link = c;

    return c;
}

/*
    ===== cpmul : 두 다항식 곱셈 =====
    a, b: 원형 연결 리스트 다항식

    반환:
    - 곱셈 결과 다항식의 헤더 노드 포인터(result)

    동작 개념(분배법칙):
    A(x) * B(x)
    = (a1 + a2 + ...) * B(x)
    = a1*B(x) + a2*B(x) + ...

    구현 방식:
    1) a의 각 항 aterm에 대해,
       b의 모든 항 bterm과 곱해서 "부분 다항식 d"를 만든다.
       - 계수: aterm->coef * bterm->coef
       - 지수: aterm->expon + bterm->expon
    2) result가 비어있으면 result = d
       아니면 result = result + d (cpadd 이용)
          - 기존 result와 d는 cerase로 정리(메모리 free list로 반납)
    3) 모든 aterm에 대해 반복하면 result가 최종 곱 결과

    주의/한계:
    - 여기 attach로 만든 d는 "b의 입력 순서 그대로" 지수를 생성한다.
      즉, b가 지수 내림차순이면 d도 내림차순이 유지될 가능성이 높지만,
      곱셈 후 같은 지수 항이 합쳐지는 과정은 cpadd에 의존한다.
    - cpadd가 제대로 동작하려면 각 다항식이 정렬되어 있어야 한다.
*/
polyPointer cpmul(polyPointer a, polyPointer b)
{
    polyPointer aterm, bterm, d, lastd;
    polyPointer temp;
    polyPointer result = NULL;

    // a의 각 항(헤더 다음부터 시작, 다시 헤더 만나면 종료)
    for (aterm = a->link; aterm != a; aterm = aterm->link) {

        // 부분 결과 다항식 d의 헤더 생성
        d = getNode();
        d->expon = -1;
        lastd = d;

        // b의 각 항과 aterm을 곱해서 d에 붙임
        for (bterm = b->link; bterm != b; bterm = bterm->link) {
            attach(aterm->coef * bterm->coef,
                   aterm->expon + bterm->expon,
                   &lastd);
        }

        // d를 원형 리스트로 닫음
        lastd->link = d;

        // result가 아직 없으면 첫 부분 다항식이 result
        if (result == NULL)
            result = d;
        else {
            /*
                기존 result와 새 부분 다항식 d를 더해서 result 갱신
                - cpadd는 새로운 리스트를 생성해서 반환
                - 이전 리스트(temp)와 d는 더 이상 필요 없으므로 cerase로 반납
            */
            temp = result;          // 기존 result 백업
            result = cpadd(temp, d);
            cerase(&temp);
            cerase(&d);
        }
    }

    return result;
}

/*
    ===== createPoly : 사용자 입력으로 다항식 생성 =====
    - 헤더 노드 만들고, (coef expon)을 반복 입력받아 attach로 붙인다.
    - expon < 0이 들어오면 입력 종료 (예: -1)

    주의:
    - 입력을 "지수 내림차순"으로 넣어야 cpadd/cpmul 결과가 정상적으로 기대됨.
    - 중복 지수 항을 입력해도 여기서는 합치지 않음(그대로 여러 노드가 존재할 수 있음)
*/
polyPointer createPoly()
{
    polyPointer header, last;
    float coef;
    int expon;

    // 헤더 노드 생성
    header = getNode();
    header->expon = -1;  // 헤더 표시
    last = header;       // 현재 마지막 노드는 헤더

    while (1) {
        printf("다항식의 항을 입력하세요. (coef expon) : ");
        scanf("%f %d", &coef, &expon);

        // exponent가 음수면 입력 종료
        if (expon < 0) break;

        // 새 항을 마지막에 붙임
        attach(coef, expon, &last);
    }

    // 원형 연결 리스트 완성: 마지막 노드가 헤더를 가리킴
    last->link = header;

    return header;
}

/*
    ===== printPoly : 다항식 출력 =====
    - 헤더 다음부터 시작해서 다시 헤더로 돌아올 때까지 출력
*/
void printPoly(polyPointer p)
{
    polyPointer temp = p->link;

    printf("    coef    expon\n");

    while (temp != p) {
        printf("%8.2f%10d\n", temp->coef, temp->expon);
        temp = temp->link;
    }
}

int main(void)
{
    polyPointer A, B, Dadd, Dmul;

    /*
        7.1) 다항식 A, B를 입력으로 생성
    */
    printf("7.1. 다항식 생성\n");

    printf("다항식 A(x)\n");
    A = createPoly();
    printf("다항식 A(x) :\n");
    printPoly(A);

    printf("\n다항식 B(x)\n");
    B = createPoly();
    printf("다항식 B(x) :\n");
    printPoly(B);

    /*
        7.2) 다항식 덧셈
        - cpadd(A,B) 결과를 Dadd에 저장
    */
    printf("\n7.2 다항식 덧셈\n");
    Dadd = cpadd(A, B);
    printf("다항식 덧셈 결과 : D(x)\n");
    printPoly(Dadd);

    /*
        7.3) 다항식 곱셈
        - cpmul(A,B) 결과를 Dmul에 저장
    */
    printf("\n7.3 다항식 곱셈\n");
    Dmul = cpmul(A, B);
    printPoly(Dmul);

    /*
        생성했던 모든 다항식을 free list로 반납(메모리 정리)
        - cerase는 리스트 노드들을 retNode로 되돌리고,
          헤더 포인터를 NULL로 바꿔준다.
    */
    cerase(&A);
    cerase(&B);
    cerase(&Dadd);
    cerase(&Dmul);

    return 0;
}
