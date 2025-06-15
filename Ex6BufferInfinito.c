#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 1000  // buffer grande o suficiente para ser considerado "infinito"

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

HANDLE hMutex;     // exclusão mútua

DWORD WINAPI Produtor(LPVOID param) {
    for (int i = 1; i <= 50; i++) {
        WaitForSingleObject(hMutex, INFINITE);

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produzido: %d\n", i);

        ReleaseMutex(hMutex);
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI Consumidor(LPVOID param) {
    int valor;
    for (int i = 1; i <= 50; i++) {
        WaitForSingleObject(hMutex, INFINITE);

        valor = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumido: %d\n", valor);

        ReleaseMutex(hMutex);
        Sleep(150);
    }
    return 0;
}

int main() {
    HANDLE hProdutor, hConsumidor;

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("Erro ao criar mutex\n");
        return 1;
    }

    hProdutor = CreateThread(NULL, 0, Produtor, NULL, 0, NULL);
    hConsumidor = CreateThread(NULL, 0, Consumidor, NULL, 0, NULL);

    WaitForSingleObject(hProdutor, INFINITE);
    WaitForSingleObject(hConsumidor, INFINITE);

    CloseHandle(hProdutor);
    CloseHandle(hConsumidor);
    CloseHandle(hMutex);

    return 0;
}
