#include<stdio.h>
#include<Windows.h>

// 2. Construa o seguinte programa :
// � Altere o programa constru�do no exerc�cio anterior para que sejam criadas N
// threads em que todas elas executam a fun��o que apresenta os n�meros
// inteiros entre 1 e 20 no ecr�.
// � Execute o programa(alterando o valor do N, por exemplo N = 2, N = 3, N = 4, ...)
// e comente os resultados
// e comente os resultados

#define N 5

DWORD WINAPI ThreadCountNumber(LPVOID paramter) {
    for (int i = 1; i <= 20; i++) {
        printf("%d ", i);
    }
    printf("\n");

    return 0;
}

int main() {
    HANDLE thread[N];

    for (int i = 0; i < N; i++)
    {
        thread[i] = CreateThread(NULL, 0, ThreadCountNumber, NULL, 0, NULL);

        if (thread[i] == NULL) {
            printf("Erro ao criar a thread\n");
            return 1;
        }
    }

    for (int i = 0; i < N; i++)
    {
        WaitForSingleObject(thread[i], INFINITE);
        CloseHandle(thread[i]);
    }

    return 0;
}