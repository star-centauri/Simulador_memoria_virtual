#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUMBER_FRAME 64
#define WSL 4
#define NUMBE_PAGES 10

typedef struct {
    int frames;
    int framesInUse;
} mainMemory;

typedef struct
{
    int index;
    int address;
} page;

typedef struct
{
    int pid;
    page paginationTable[NUMBE_PAGES];
} processo;

typedef struct 
{
    int wsl;
    int pageInUse;
} LRU;

void delay(int number_of_seconds);
void escreveArquivo(char string[], int pid);
void runProcesses(processo processos[20], mainMemory memory);
int isMemoryFull(mainMemory memory);

int main() {
    //INICIAR MEMORIA
    mainMemory memory;
    memory.frames = NUMBER_FRAME;
    memory.framesInUse = 0;

    processo processos[20];

    runProcesses(processos, memory);

    return 0;
}

// =============== BEGIN GERENCIADOR MEMORIA =============== //
void runProcesses(processo processos[20], mainMemory memory) {

}

int isMemoryFull(mainMemory memory) {
    if (memory.frames == memory.framesInUse) {
        return 1;
    }

    return 0;
}
// =============== END GERENCIADOR MEMORIA =============== //

// =============== BEGIN AUXILIARES =============== //

/// @brief SIMULACAO DE UM TIMEOUT
/// @param number_of_seconds 
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

/// @brief CRIA UM LOG DE CADA PROCESSO
/// @param string 
/// @param pid 
void escreveArquivo(char string[], int pid){
    
    char buff[100];    
    sprintf(buff, "%d", pid);
    char arquivo[20] = "processo";
    strcat(arquivo, buff);
    strcat(arquivo,".txt");

	FILE *pa;
	pa = fopen(arquivo, "a+");
	if (pa == NULL){
		printf("Arquivo nao pode ser aberto.");
	}

	fputs(string, pa);
	fclose(pa);	
}
// =============== END AUXILIARES =============== //
