#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 10  // buffer finito

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

HANDLE hMutex;
HANDLE hItems;    // semáforo para contar itens no buffer
HANDLE hSpaces;   // semáforo para contar espaços livres no buffer

DWORD WINAPI Produtor(LPVOID param) {
    for (int i = 1; i <= 50; i++) {
        WaitForSingleObject(hSpaces, INFINITE);  // espera espaço disponível
        WaitForSingleObject(hMutex, INFINITE);

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produzido: %d\n", i);

        ReleaseMutex(hMutex);
        ReleaseSemaphore(hItems, 1, NULL);  // sinaliza item disponível

        Sleep(100);
    }
    return 0;
}

DWORD WINAPI Consumidor(LPVOID param) {
    int valor;
    for (int i = 1; i <= 50; i++) {
        WaitForSingleObject(hItems, INFINITE);  // espera item disponível
        WaitForSingleObject(hMutex, INFINITE);

        valor = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumido: %d\n", valor);

        ReleaseMutex(hMutex);
        ReleaseSemaphore(hSpaces, 1, NULL);  // sinaliza espaço disponível

        Sleep(150);
    }
    return 0;
}

int main() {
    HANDLE hProdutor, hConsumidor;

    hMutex = CreateMutex(NULL, FALSE, NULL);
    hItems = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    hSpaces = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);

    if (hMutex == NULL || hItems == NULL || hSpaces == NULL) {
        printf("Erro ao criar sincronizadores\n");
        return 1;
    }

    hProdutor = CreateThread(NULL, 0, Produtor, NULL, 0, NULL);
    hConsumidor = CreateThread(NULL, 0, Consumidor, NULL, 0, NULL);

    WaitForSingleObject(hProdutor, INFINITE);
    WaitForSingleObject(hConsumidor, INFINITE);

    CloseHandle(hProdutor);
    CloseHandle(hConsumidor);
    CloseHandle(hMutex);
    CloseHandle(hItems);
    CloseHandle(hSpaces);

    return 0;
}
