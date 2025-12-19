#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
    ===== 정렬 알고리즘 성능 비교 프로그램 =====

    [비교 대상 정렬 알고리즘]
    1) 선택 정렬 (Selection Sort)
    2) 삽입 정렬 (Insertion Sort)
    3) 퀵 정렬 (Quick Sort)

    [실험 데이터 유형]
    12.1) 랜덤 데이터
    12.2.1) 이미 오름차순으로 정렬된 데이터
    12.2.2) 이미 내림차순으로 정렬된 데이터

    [측정 항목]
    - 실행 시간 (clock() 함수 사용, 초 단위)

    [실험 목적]
    - 입력 데이터 상태에 따른 각 정렬 알고리즘의 성능 차이 관찰
    - O(n²) 정렬과 O(n log n) 정렬의 실제 실행 시간 비교
*/

/*
    ===== selectionSort =====
    선택 정렬 알고리즘

    원리:
    - 배열에서 가장 작은 원소를 찾아 맨 앞과 교환
    - 그 다음 위치부터 다시 최소값 탐색 반복

    시간 복잡도:
    - 최선 / 평균 / 최악 모두 O(n²)

    특징:
    - 비교 횟수는 많지만 교환 횟수는 적음
    - 데이터 상태(정렬 여부)에 영향을 거의 받지 않음
*/
void selectionSort(double a[], int n) {
    int i, j, min;
    double temp;

    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[min])
                min = j;
        }
        // 최소값을 현재 위치로 이동
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}

/*
    ===== insertionSort =====
    삽입 정렬 알고리즘

    원리:
    - 앞부분은 이미 정렬되어 있다고 가정
    - 현재 원소를 적절한 위치에 삽입

    시간 복잡도:
    - 최선: O(n) (이미 정렬된 경우)
    - 평균 / 최악: O(n²)

    특징:
    - 거의 정렬된 데이터에 매우 빠름
*/
void insertionSort(double a[], int n) {
    int i, j;
    double key;

    for (i = 1; i < n; i++) {
        key = a[i];
        j = i - 1;

        // key보다 큰 원소를 한 칸씩 뒤로 이동
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        // key를 올바른 위치에 삽입
        a[j + 1] = key;
    }
}

/*
    ===== partition =====
    퀵 정렬을 위한 분할 함수 (Hoare Partition 방식)

    원리:
    - 피벗(pivot)을 기준으로
      작은 값은 왼쪽, 큰 값은 오른쪽으로 이동

    반환값:
    - 분할 기준 인덱스 j
*/
int partition(double a[], int left, int right) {
    double pivot = a[(left + right) / 2];  // 중앙값을 피벗으로 선택
    int i = left - 1;
    int j = right + 1;
    double temp;

    while (1) {
        // pivot보다 작은 값 찾기
        do {
            i++;
        } while (a[i] < pivot);

        // pivot보다 큰 값 찾기
        do {
            j--;
        } while (a[j] > pivot);

        // 인덱스가 교차되면 종료
        if (i >= j)
            return j;

        // 두 원소 교환
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
    }
}

/*
    ===== quickSort =====
    퀵 정렬 알고리즘 (재귀 + 꼬리 재귀 제거)

    원리:
    - 분할(partition)을 통해 배열을 둘로 나눈 뒤
    - 각각을 재귀적으로 정렬

    시간 복잡도:
    - 평균: O(n log n)
    - 최악: O(n²) (이미 정렬된 경우, 피벗 선택이 나쁠 때)

    특징:
    - 가장 작은 구간을 먼저 재귀 호출하여
      재귀 깊이를 줄이는 최적화 적용
*/
void quickSort(double a[], int left, int right) {
    while (left < right) {
        int p = partition(a, left, right);

        // 더 작은 구간을 먼저 재귀 호출
        if (p - left < right - p) {
            quickSort(a, left, p);
            left = p + 1;
        }
        else {
            quickSort(a, p + 1, right);
            right = p;
        }
    }
}

/*
    ===== copyArray =====
    - 원본 배열(src)을 대상 배열(dest)로 복사
    - 동일한 입력 데이터를 각 정렬 알고리즘에 제공하기 위해 사용
*/
void copyArray(double* dest, double* src, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

/*
    ===== getTime =====
    - selection / insertion sort 실행 시간 측정

    sortFunc:
    - (double[], int) 형태의 정렬 함수 포인터
*/
double getTime(void (*sortFunc)(double[], int), double a[], int n) {
    clock_t start = clock();
    sortFunc(a, n);
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

/*
    ===== getQuickTime =====
    - quick sort 실행 시간 측정
    - quickSort는 인자가 (배열, left, right)이므로 별도 함수 사용
*/
double getQuickTime(void (*quickFunc)(double[], int, int), double a[], int n) {
    clock_t start = clock();
    quickFunc(a, 0, n - 1);
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

/*
    ===== main =====
    - 서로 다른 데이터 크기 및 상태에 대해
      세 가지 정렬 알고리즘 성능 비교
*/
int main() {
    int sizes[] = { 1000, 10000, 100000 };
    int sizeCount = 3;

    srand((unsigned)time(NULL));

    /*
        12.1 랜덤 데이터 정렬
    */
    printf("12.1. 랜덤 생성 데이터의 정렬\n");

    for (int s = 0; s < sizeCount; s++) {
        int n = sizes[s];
        double* A = (double*)malloc(sizeof(double) * n);
        double* B = (double*)malloc(sizeof(double) * n);

        if (A == NULL || B == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            return 1;
        }

        // 랜덤 데이터 생성
        for (int i = 0; i < n; i++)
            A[i] = (rand() / (RAND_MAX + 1.0)) * 100000000;

        printf("  원소 수 : %d\n", n);

        copyArray(B, A, n);
        printf("  selection sort time: %.6f\n", getTime(selectionSort, B, n));

        copyArray(B, A, n);
        printf("  insertion sort time: %.6f\n", getTime(insertionSort, B, n));

        copyArray(B, A, n);
        printf("      quick sort time: %.6f\n\n", getQuickTime(quickSort, B, n));

        free(A);
        free(B);
    }

    /*
        12.2 이미 정렬된 데이터 정렬
    */
    printf("12.2. 정렬된 데이터의 정렬\n");

    /*
        12.2.1 오름차순 정렬된 데이터
    */
    printf("12.2.1. 오름차순 정렬된 데이터의 정렬\n");

    for (int s = 0; s < sizeCount; s++) {
        int n = sizes[s];
        double* A = (double*)malloc(sizeof(double) * n);
        double* B = (double*)malloc(sizeof(double) * n);

        if (A == NULL || B == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            return 1;
        }

        // 이미 오름차순 정렬된 데이터
        for (int i = 0; i < n; i++)
            A[i] = (double)(i + 1);

        printf("  원소 수 : %d\n", n);

        copyArray(B, A, n);
        printf("  selection sort time: %.6f\n", getTime(selectionSort, B, n));

        copyArray(B, A, n);
        printf("  insertion sort time: %.6f\n", getTime(insertionSort, B, n));

        copyArray(B, A, n);
        printf("      quick sort time: %.6f\n\n", getQuickTime(quickSort, B, n));

        free(A);
        free(B);
    }

    /*
        12.2.2 내림차순 정렬된 데이터
    */
    printf("12.2.2. 내림차순 정렬된 데이터의 정렬\n");

    for (int s = 0; s < sizeCount; s++) {
        int n = sizes[s];
        double* A = (double*)malloc(sizeof(double) * n);
        double* B = (double*)malloc(sizeof(double) * n);

        if (A == NULL || B == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            return 1;
        }

        // 이미 내림차순 정렬된 데이터
        for (int i = 0; i < n; i++)
            A[i] = (double)(n - i);

        printf("  원소 수 : %d\n", n);

        copyArray(B, A, n);
        printf("  selection sort time: %.6f\n", getTime(selectionSort, B, n));

        copyArray(B, A, n);
        printf("  insertion sort time: %.6f\n", getTime(insertionSort, B, n));

        copyArray(B, A, n);
        printf("      quick sort time: %.6f\n\n", getQuickTime(quickSort, B, n));

        free(A);
        free(B);
    }

    return 0;
}
