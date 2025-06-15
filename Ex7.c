#include <stdio.h>
#include <windows.h>

#define MAX_CLIENTES 50
#define MAX_DENTRO 20
#define NUM_CADEIRAS 3
#define NUM_BARBEIROS 3
#define NUM_SOFAS 4

// Semáforos para controlar recursos limitados
HANDLE semClientesDentro;     // Limita clientes dentro da barbearia (20)
HANDLE semSofas;              // Limita lugares no sofá (4)
HANDLE semCadeiras;           // Limita cadeiras de barbeiro (3)
HANDLE semBarbeiros;          // Limita barbeiros disponíveis (3)
HANDLE semCaixa;              // Caixa registadora (1)

// Mutex para controlar output
HANDLE mutexPrint;

DWORD WINAPI Cliente(LPVOID idParam) {
    int id = (int)(size_t)idParam;

    // Cliente tenta entrar na barbearia
    if (WaitForSingleObject(semClientesDentro, 0) == WAIT_TIMEOUT) {
        // Barbeária cheia
        WaitForSingleObject(mutexPrint, INFINITE);
        printf("Cliente %d: Barbearia cheia, vai embora.\n", id);
        ReleaseMutex(mutexPrint);
        return 0;
    }

    WaitForSingleObject(mutexPrint, INFINITE);
    printf("Cliente %d: Entrou na barbearia.\n", id);
    ReleaseMutex(mutexPrint);

    // Cliente tenta sentar no sofá ou fica na sala de espera (espera por sofá)
    WaitForSingleObject(semSofas, INFINITE);

    WaitForSingleObject(mutexPrint, INFINITE);
    printf("Cliente %d: Sentou no sofá.\n", id);
    ReleaseMutex(mutexPrint);

    // Espera por cadeira de barbeiro
    WaitForSingleObject(semCadeiras, INFINITE);
    ReleaseSemaphore(semSofas, 1, NULL);  // Liberta sofá

    WaitForSingleObject(mutexPrint, INFINITE);
    printf("Cliente %d: Sentou na cadeira do barbeiro.\n", id);
    ReleaseMutex(mutexPrint);

    // Espera barbeiro disponível
    WaitForSingleObject(semBarbeiros, INFINITE);

    // Simula corte de cabelo
    Sleep(500);

    WaitForSingleObject(mutexPrint, INFINITE);
    printf("Cliente %d: Corte terminado.\n", id);
    ReleaseMutex(mutexPrint);

    // Liberta barbeiro e cadeira
    ReleaseSemaphore(semBarbeiros, 1, NULL);
    ReleaseSemaphore(semCadeiras, 1, NULL);

    // Vai para a caixa registadora (1 cliente por vez)
    WaitForSingleObject(semCaixa, INFINITE);

    WaitForSingleObject(mutexPrint, INFINITE);
    printf("Cliente %d: A pagar.\n", id);
    ReleaseMutex(mutexPrint);

    Sleep(200); // Simula pagamento

    WaitForSingleObject(mutexPrint, INFINITE);
    printf("Cliente %d: Pagamento efetuado e sai da barbearia.\n", id);
    ReleaseMutex(mutexPrint);

    ReleaseSemaphore(semCaixa, 1, NULL);
    ReleaseSemaphore(semClientesDentro, 1, NULL);  // Sai da barbearia

    return 0;
}

int main() {
    HANDLE threads[MAX_CLIENTES];
    DWORD threadId;

    // Criação dos semáforos
    semClientesDentro = CreateSemaphore(NULL, MAX_DENTRO, MAX_DENTRO, NULL);
    semSofas = CreateSemaphore(NULL, NUM_SOFAS, NUM_SOFAS, NULL);
    semCadeiras = CreateSemaphore(NULL, NUM_CADEIRAS, NUM_CADEIRAS, NULL);
    semBarbeiros = CreateSemaphore(NULL, NUM_BARBEIROS, NUM_BARBEIROS, NULL);
    semCaixa = CreateSemaphore(NULL, 1, 1, NULL);

    mutexPrint = CreateMutex(NULL, FALSE, NULL);

    if (!semClientesDentro || !semSofas || !semCadeiras || !semBarbeiros || !semCaixa || !mutexPrint) {
        printf("Erro ao criar sincronizadores\n");
        return 1;
    }

    // Criar threads clientes
    for (int i = 0; i < MAX_CLIENTES; i++) {
        threads[i] = CreateThread(NULL, 0, Cliente, (LPVOID)(size_t)(i + 1), 0, &threadId);
        if (threads[i] == NULL) {
            printf("Erro ao criar thread cliente %d\n", i + 1);
            return 1;
        }
        Sleep(50);  // Stagger para melhor visualização
    }

    // Esperar todas terminarem
    WaitForMultipleObjects(MAX_CLIENTES, threads, TRUE, INFINITE);

    // Fechar handles
    for (int i = 0; i < MAX_CLIENTES; i++) {
        CloseHandle(threads[i]);
    }
    CloseHandle(semClientesDentro);
    CloseHandle(semSofas);
    CloseHandle(semCadeiras);
    CloseHandle(semBarbeiros);
    CloseHandle(semCaixa);
    CloseHandle(mutexPrint);

    printf("Dia de trabalho terminado.\n");
    return 0;
}
