#include <stdio.h>
#include <windows.h>

#define N 5

HANDLE hSemaphore;

DWORD WINAPI ThreadCountNumber(LPVOID parameter) {
    WaitForSingleObject(hSemaphore, INFINITE);

    for (int i = 1; i <= 20; i++) {
        printf("%d ", i);
    }
    printf("\n");

    ReleaseSemaphore(hSemaphore, 1, NULL);

    return 0;
}

int main() {
    HANDLE threads[N];

    hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);

    if (hSemaphore == NULL) {
        printf("Erro ao criar o semáforo\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        threads[i] = CreateThread(NULL, 0, ThreadCountNumber, NULL, 0, NULL);
        if (threads[i] == NULL) {
            printf("Erro ao criar a thread %d\n", i);
            return 1;
        }
    }

    WaitForMultipleObjects(N, threads, TRUE, INFINITE);

    for (int i = 0; i < N; i++) {
        CloseHandle(threads[i]);
    }

    CloseHandle(hSemaphore);

    return 0;
}
