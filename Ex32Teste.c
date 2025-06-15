#include <stdio.h>
#include <windows.h>

#define N 4

DWORD WINAPI Contador(LPVOID parameter) {
    for (int i = 1; i <= 20; i++) {
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}

int main() {
    HANDLE threads[N];

    for (int i = 0; i < N; i++) {

        threads[i] = CreateThread(NULL, 0, Contador, NULL, 0, NULL);

        if (threads[i] == NULL) {
            printf("Erro ao criar a thread %d\n", i);
            return 1;
        }
    }

    // Esperar todas as threads terminarem
    WaitForMultipleObjects(N, threads, TRUE, INFINITE);

    // Fechar handles
    for (int i = 0; i < N; i++) {
        CloseHandle(threads[i]);
    }

    return 0;
}
