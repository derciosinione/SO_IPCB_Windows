#include <stdio.h>
#include <windows.h>

HANDLE hThread1, hThread2;

DWORD WINAPI Thread1Func(LPVOID param) {
    char* mensagem = (char*)param;
    while (1) {
        printf("%s\n", mensagem);
        Sleep(2000);
    }
    return 0;
}

DWORD WINAPI Thread2Func(LPVOID param) {
    int valor = *(int*)param;
    while (1) {
        printf("%d\n", valor);
        Sleep(1000);
    }
    return 0;
}

DWORD WINAPI Thread3Func(LPVOID param) {
    Sleep(10000);
    TerminateThread(hThread1, 0);
    printf("Thread1 terminada\n");
    return 0;
}

int main() {
    int valor = 42;
    DWORD threadId;

    hThread1 = CreateThread(NULL, 0, Thread1Func, "Mensagem da thread 1", 0, &threadId);
    hThread2 = CreateThread(NULL, 0, Thread2Func, &valor, 0, &threadId);
    HANDLE hThread3 = CreateThread(NULL, 0, Thread3Func, NULL, 0, &threadId);

    WaitForSingleObject(hThread3, INFINITE);
    Sleep(5000);
    TerminateThread(hThread2, 0);

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);

    printf("Programa terminou\n");
    return 0;
}
