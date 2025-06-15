#include <stdio.h>
#include <windows.h>

// Handle global da thread infinita para podermos terminar a partir da outra thread
HANDLE hThreadInfinita;

// Thread que imprime "I'M ALIVE!" em loop a cada 500 ms
DWORD WINAPI ThreadInfinita(LPVOID param) {
    while (1) {
        printf("I'M ALIVE!\n");
        Sleep(500);
    }
    return 0;
}

// Thread que aguarda 5 segundos e termina a thread infinita
DWORD WINAPI ThreadControladora(LPVOID param) {
    Sleep(5000);
    // Termina a thread infinita
    TerminateThread(hThreadInfinita, 0);
    printf("TERMINOU A THREAD EM CICLO INFINITO!\n");
    return 0;
}

int main() {
    HANDLE hThreadControl;

    // Criar thread infinita
    hThreadInfinita = CreateThread(NULL, 0, ThreadInfinita, NULL, 0, NULL);
    if (hThreadInfinita == NULL) {
        printf("Erro ao criar a thread infinita.\n");
        return 1;
    }

    // Criar thread controladora
    hThreadControl = CreateThread(NULL, 0, ThreadControladora, NULL, 0, NULL);
    if (hThreadControl == NULL) {
        printf("Erro ao criar a thread controladora.\n");
        TerminateThread(hThreadInfinita, 0);
        CloseHandle(hThreadInfinita);
        return 1;
    }

    // Esperar a thread controladora terminar
    WaitForSingleObject(hThreadControl, INFINITE);

    // Fechar handles
    CloseHandle(hThreadControl);
    CloseHandle(hThreadInfinita);

    return 0;
}
