#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUMBER_FRAME 64
#define WSL 4
#define NUMBER_PAGES 10
#define NUMBER_PROCESS 20

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
    int ativo;
    int pid;
    page paginationTable[NUMBER_PAGES];
    int pagesInMemory[WSL];
    int inMemory;
} processo;

typedef struct 
{
    int wsl;
    int pageInUse;
} LRU;

void delay(int number_of_seconds);
void escreveArquivo(char string[], int pid);
void runProcesses(mainMemory memory);
void gerarNovoProcesso();
int isMemoryFull(mainMemory memory);

processo listaProcessos[NUMBER_PROCESS];

int main() {
    //INICIAR MEMORIA
    mainMemory memory;
    srand(time(NULL));
    memory.frames = NUMBER_FRAME;
    memory.framesInUse = 0;

    //iniciando vetor de processos
    for(int i = 0; i < NUMBER_PROCESS; i++){
        listaProcessos[i].pid = i;
        listaProcessos[i].ativo = 0;
        //inicializando a tabela de paginas
        for(int j = 0; i < NUMBER_PAGES; i++){
                listaProcessos[i].paginationTable[j].index = j;
                listaProcessos[i].paginationTable[j].address = -1;
        }
    }

    runProcesses(memory);

    return 0;
}

// =============== BEGIN GERENCIADOR MEMORIA =============== //
void runProcesses(mainMemory memory) {   
    int elapseTime = 1;
    int run = 1;

    while(run) {
        
        if ( elapseTime%3==0 ) {
            gerarNovoProcesso();
            solicitaListaProcessos();
        }

        elapseTime++;
    }

    //percorre o vetor de processos e solicita uma pagina aleatoria para cada um que esta ativo
}

// Faz a simulação de criação de um novo processo, ativando-o para execução
void gerarNovoProcesso() {
    for(int i = 0; i < NUMBER_PROCESS; i++) {
        if ( listaProcessos[i].ativo == 0 ) {
            listaProcessos[i].ativo = 1;
            break;
        }
    }
}

// Verifica se memória está cheia
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

//percorre o vetor de processo e solicita uma pagina aleatoria para cada um que estiver ativo
void solicitaListaProcessos(){
    for(int i = 0; i < NUMBER_PROCESS; i++){
        if(listaProcessos[i].ativo == 1){
            int randomPage = rand() % NUMBER_PAGES;
            solicitaPagina(i, randomPage);
        }
    }
}

void solicitaPagina(int pid, int page){
    //checar se a pagina requerida já está na memoria
    int pageInMemory = 0;
    int pageIndex;
    for(int i = 0; i < WSL; i++){
        if(listaProcessos[pid].pagesInMemory[i] == page){
            pageInMemory = 1;
            pageIndex = i;
        }
    }
    //se o processo já está na memória, remove ele da posiçao atual e coloca no final
    if(pageInMemory){
        for(int i = pageIndex; i < WSL-1; i++){
            listaProcessos[pid].pagesInMemory[i] = listaProcessos[pid].pagesInMemory[i+1];
        }
    }
    //caso contrário remove o último e coloca a pagina requerida no final
    else{
        for(int i = 0; i < WSL-1; i++){
            listaProcessos[pid].pagesInMemory[i] = listaProcessos[pid].pagesInMemory[i+1];
        }
    }
    listaProcessos[pid].pagesInMemory[WSL-1] = page;
    
}

// =============== END AUXILIARES =============== //
