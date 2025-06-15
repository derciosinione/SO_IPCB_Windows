#include<stdio.h>
#include<Windows.h>

// Crie a fun��o de uma thread que apresente os n�meros inteiros entre 1 e 20
// no ecr�.Os n�meros devem ser separados por um espa�o e no final dos 20
// n�meros deve aparecer uma quebra de linha(\n).
// � Crie a thread na fun��o principal do programa usando a fun��o
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