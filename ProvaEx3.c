#include <stdio.h>
#include <windows.h>

#define MAX_VALORES 20

int buffer[MAX_VALORES];
int writeIndex = 0;
int readIndex = 0;
int valoresLidos = 0;
int somatorio = 0;

HANDLE mutexBuffer;
HANDLE mutexSomatorio;
HANDLE semDados;    
HANDLE semEspaco;   

DWORD WINAPI Thread1(LPVOID param) {
    for (int i = 0; i < MAX_VALORES; i++) {
        WaitForSingleObject(semEspaco, INFINITE);
        WaitForSingleObject(mutexBuffer, INFINITE);

        buffer[writeIndex++] = i;

        ReleaseMutex(mutexBuffer);
        ReleaseSemaphore(semDados, 1, NULL);
    }
    return 0;
}

DWORD WINAPI ThreadLeitura(LPVOID param) {
    int valorLido;
    while (1) {
        WaitForSingleObject(semDados, INFINITE);
        WaitForSingleObject(mutexBuffer, INFINITE);

        if (valoresLidos >= MAX_VALORES) {
            ReleaseMutex(mutexBuffer);
            ReleaseSemaphore(semDados, 1, NULL); 
            break;
        }

        valorLido = buffer[readIndex++];
        valoresLidos++;

        ReleaseMutex(mutexBuffer);
        ReleaseSemaphore(semEspaco, 1, NULL);

        WaitForSingleObject(mutexSomatorio, INFINITE);
        somatorio += valorLido;
        printf("Thread %d leu valor %d, somatorio atual: %d\n", GetCurrentThreadId(), valorLido, somatorio);
        ReleaseMutex(mutexSomatorio);

        if (valoresLidos >= MAX_VALORES)
            break;
    }
    return 0;
}

int main() {
    HANDLE hThread1, hThread2, hThread3;

    mutexBuffer = CreateMutex(NULL, FALSE, NULL);
    mutexSomatorio = CreateMutex(NULL, FALSE, NULL);
    semDados = CreateSemaphore(NULL, 0, MAX_VALORES, NULL);
    semEspaco = CreateSemaphore(NULL, MAX_VALORES, MAX_VALORES, NULL);

    hThread1 = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
    hThread2 = CreateThread(NULL, 0, ThreadLeitura, NULL, 0, NULL);
    hThread3 = CreateThread(NULL, 0, ThreadLeitura, NULL, 0, NULL);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    WaitForSingleObject(hThread3, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);

    CloseHandle(mutexBuffer);
    CloseHandle(mutexSomatorio);
    CloseHandle(semDados);
    CloseHandle(semEspaco);

    return 0;
}
