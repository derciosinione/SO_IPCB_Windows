#include<stdio.h>
#include<Windows.h>

DWORD WINAPI Contador(LPVOID parameter) {
	for (int i = 1; i <= 20; i++) {
		printf("%d ", i);
	}

	printf("\n");

	return 0;
}

int main() {
	HANDLE myThread = CreateThread(NULL, 0, Contador, NULL, 0, NULL);

	if (myThread == NULL) {
		printf("Erro ao criar a thread\n");
		return 1;
	}

	WaitForSingleObject(myThread, INFINITE);

	CloseHandle(myThread);

	return 0;
}