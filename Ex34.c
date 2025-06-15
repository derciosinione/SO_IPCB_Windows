#include <stdio.h>
#include <windows.h>

// Thread que recebe um inteiro (passado por ponteiro)
DWORD WINAPI ThreadPrintInt(LPVOID param) {
    int value = *(int*)param;
    printf("Inteiro recebido: %d\n", value);
    return 0;
}

// Thread que recebe uma string
DWORD WINAPI ThreadPrintString(LPVOID param) {
    char* str = (char*)param;
    printf("String recebida: %s\n", str);
    return 0;
}

int main() {
    HANDLE hThreadInt, hThreadStr;
    int num = 42;
    char mensagem[] = "Ola, thread!";

    hThreadInt = CreateThread(NULL, 0, ThreadPrintInt, &num, 0, NULL);
    if (hThreadInt == NULL) {
        printf("Erro ao criar thread para inteiro.\n");
        return 1;
    }

    hThreadStr = CreateThread(NULL, 0, ThreadPrintString, mensagem, 0, NULL);
    if (hThreadStr == NULL) {
        printf("Erro ao criar thread para string.\n");
        return 1;
    }

    // Esperar as threads terminarem, uma a uma
    WaitForSingleObject(hThreadInt, INFINITE);
    CloseHandle(hThreadInt);

    WaitForSingleObject(hThreadStr, INFINITE);
    CloseHandle(hThreadStr);

    return 0;
}
