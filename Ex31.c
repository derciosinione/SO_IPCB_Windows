#include<stdio.h>
#include<Windows.h>

// Crie a função de uma thread que apresente os números inteiros entre 1 e 20
// no ecrã.Os números devem ser separados por um espaço e no final dos 20
// números deve aparecer uma quebra de linha(\n).
// • Crie a thread na função principal do programa usando a função
// CreateThread.


DWORD WINAPI ThreadCountNumber(LPVOID paramter) {
    for (int i = 1; i <= 20; i++) {
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}

int main() {
    HANDLE thread;

    thread = CreateThread(NULL, 0, ThreadCountNumber, NULL, 0, NULL);

    if (thread == NULL) {
        printf("Erro ao criar a thread\n");
        return 1;
    }

    // Esperar a thread terminar
    WaitForSingleObject(thread, INFINITE);

    // Fechar o identificador da thread
    CloseHandle(thread);

    return 0;
}