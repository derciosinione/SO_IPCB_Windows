#include <stdio.h>
#include <windows.h>

#define THREADS1 10
#define THREADS2 5

HANDLE semPrint;

DWORD WINAPI ThreadFunc1(LPVOID param) {
    WaitForSingleObject(semPrint, INFINITE);
    for (int i = 1; i <= 10; i++) {
        printf("%d ", i);
    }
    printf("\n");
    ReleaseSemaphore(semPrint, 1, NULL);
    return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID param) {
    WaitForSingleObject(semPrint, INFINITE);
    for (int i = 1; i <= 20; i++) {
        printf("%d ", i);
    }
    printf("\n");
    ReleaseSemaphore(semPrint, 1, NULL);
    return 0;
}

int main() {
    HANDLE threads[10]; 
    semPrint = CreateSemaphore(NULL, 1, 1, NULL);

    for (int i = 0; i < THREADS1; i++) {
        threads[i] = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, NULL);
    }

    for (int i = 0; i < THREADS1; i++) {
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
    }

    for (int i = 0; i < THREADS2; i++) {
        threads[i] = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, NULL);
    }

    for (int i = 0; i < THREADS2; i++) {
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
    }

    CloseHandle(semPrint);
    return 0;
}
