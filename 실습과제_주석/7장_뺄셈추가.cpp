#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct polyNode* polyPointer;
typedef struct polyNode {
    float coef;
    int expon;
    polyPointer link;
} polyNode;

polyPointer avail = NULL;

/* 함수 원형 */
polyPointer getNode(void);
void retNode(polyPointer node);
void cerase(polyPointer* ptr);
void attach(float coefficient, int exponent, polyPointer* last);
int compare(int x, int y);

polyPointer cpadd(polyPointer a, polyPointer b);
polyPointer cpnegate(polyPointer p);     // ★ 추가
polyPointer cpsub(polyPointer a, polyPointer b); // ★ 추가
polyPointer cpmul(polyPointer a, polyPointer b);

polyPointer createPoly();
void printPoly(polyPointer p);

/* ===== 노드 관리 ===== */
polyPointer getNode(void)
{
    polyPointer node;
    if (avail) {
        node = avail;
        avail = avail->link;
    }
    else {
        node = (polyPointer)malloc(sizeof(polyNode));
    }
    return node;
}

void retNode(polyPointer node)
{
    node->link = avail;
    avail = node;
}

void cerase(polyPointer* ptr)
{
    polyPointer p, q;
    if (!*ptr) return;

    p = (*ptr)->link;
    while (p != *ptr) {
        q = p->link;
        retNode(p);
        p = q;
    }
    retNode(*ptr);
    *ptr = NULL;
}

/* ===== 다항식 기본 연산 ===== */
void attach(float coefficient, int exponent, polyPointer* last)
{
    polyPointer temp = getNode();
    temp->coef = coefficient;
    temp->expon = exponent;
    (*last)->link = temp;
    *last = temp;
}

int compare(int x, int y)
{
    if (x < y) return -1;
    else if (x > y) return 1;
    else return 0;
}

/* ===== 다항식 덧셈 ===== */
polyPointer cpadd(polyPointer a, polyPointer b)
{
    polyPointer startA, c, lastC;
    float sum;
    int done = 0;

    startA = a;
    a = a->link;
    b = b->link;

    c = getNode();
    c->expon = -1;
    lastC = c;

    do {
        switch (compare(a->expon, b->expon))
        {
        case -1:
            attach(b->coef, b->expon, &lastC);
            b = b->link;
            break;

        case 0:
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
            attach(a->coef, a->expon, &lastC);
            a = a->link;
            break;
        }
    } while (!done);

    lastC->link = c;
    return c;
}

/* ===== 다항식 부호 반전 (B → -B) ===== */
polyPointer cpnegate(polyPointer p)
{
    polyPointer result, last, temp;

    result = getNode();
    result->expon = -1;
    last = result;

    for (temp = p->link; temp != p; temp = temp->link) {
        attach(-temp->coef, temp->expon, &last);
    }

    last->link = result;
    return result;
}

/* ===== 다항식 뺄셈 (A - B) ===== */
polyPointer cpsub(polyPointer a, polyPointer b)
{
    polyPointer negB, result;

    negB = cpnegate(b);      // B → -B
    result = cpadd(a, negB); // A + (-B)
    cerase(&negB);           // 임시 다항식 정리

    return result;
}

/* ===== 다항식 곱셈 ===== */
polyPointer cpmul(polyPointer a, polyPointer b)
{
    polyPointer aterm, bterm, d, lastd;
    polyPointer temp, result = NULL;

    for (aterm = a->link; aterm != a; aterm = aterm->link) {

        d = getNode();
        d->expon = -1;
        lastd = d;

        for (bterm = b->link; bterm != b; bterm = bterm->link) {
            attach(aterm->coef * bterm->coef,
                aterm->expon + bterm->expon,
                &lastd);
        }
        lastd->link = d;

        if (result == NULL)
            result = d;
        else {
            temp = result;
            result = cpadd(temp, d);
            cerase(&temp);
            cerase(&d);
        }
    }
    return result;
}

/* ===== 입출력 ===== */
polyPointer createPoly()
{
    polyPointer header, last;
    float coef;
    int expon;

    header = getNode();
    header->expon = -1;
    last = header;

    while (1) {
        printf("다항식 항 입력 (coef expon) : ");
        scanf("%f %d", &coef, &expon);
        if (expon < 0) break;
        attach(coef, expon, &last);
    }

    last->link = header;
    return header;
}

void printPoly(polyPointer p)
{
    polyPointer temp = p->link;
    printf("    coef    expon\n");
    while (temp != p) {
        printf("%8.2f%10d\n", temp->coef, temp->expon);
        temp = temp->link;
    }
}

/* ===== main ===== */
int main(void)
{
    polyPointer A, B, Dadd, Dsub, Dmul;

    printf("다항식 A(x)\n");
    A = createPoly();
    printPoly(A);

    printf("\n다항식 B(x)\n");
    B = createPoly();
    printPoly(B);

    printf("\n[덧셈] A + B\n");
    Dadd = cpadd(A, B);
    printPoly(Dadd);

    printf("\n[뺄셈] A - B\n");
    Dsub = cpsub(A, B);
    printPoly(Dsub);

    printf("\n[곱셈] A * B\n");
    Dmul = cpmul(A, B);
    printPoly(Dmul);

    cerase(&A);
    cerase(&B);
    cerase(&Dadd);
    cerase(&Dsub);
    cerase(&Dmul);

    return 0;
}
