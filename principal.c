#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUMBER_FRAME 64
#define WSL 4
#define NUMBER_PAGES 10
#define NUMBER_PROCESS 20

typedef struct
{
    int index;
    int indexProccess;
    int address;
} page;

typedef struct
{
    int ativo;
    int pid;
    page paginationTable[NUMBER_PAGES];
} processo;

typedef struct 
{
    page pageInUse[NUMBER_FRAME];
} LRU;

void delay(int number_of_seconds);
void escreveArquivo(char string[], int pid);
void runProcesses();
void gerarNovoProcesso();
void inserirPageNaMemoria(int pid, int page);
void solicitaListaProcessos();
void algoritmoLRU(int pid, int page);
void updateList(int pid, int page);
void add(int pid, int page);
void updateNotFault(int pid, int pageId);

processo listaProcessos[NUMBER_PROCESS];
LRU algoritmo;

int main() {
    //INICIAR MEMORIA
    srand(time(NULL));

    //iniciando vetor de processos
    for(int i = 0; i < NUMBER_PROCESS; i++) {
        listaProcessos[i].pid = i+1;
        listaProcessos[i].ativo = 0;
        
        //inicializando a tabela de paginas
        for(int j = 0; i < NUMBER_PAGES; i++){
            listaProcessos[i].paginationTable[j].index = j+1;
            listaProcessos[i].paginationTable[j].indexProccess = i+1;
            listaProcessos[i].paginationTable[j].address = -1;
        }
    }

    //inicializar algoritmo
    for(int i = 0; i < NUMBER_FRAME; i++) {
        algoritmo.pageInUse[i].index = 0;
        algoritmo.pageInUse[i].indexProccess = 0;
        algoritmo.pageInUse[i].address = -1;
    }

    runProcesses();

    return 0;
}

// =============== BEGIN GERENCIADOR MEMORIA =============== //
void runProcesses() {   
    int elapseTime = 1;
    int run = 1;

    while(run) {
        printf("Tempo corrido = %d \n", elapseTime);

        if ( elapseTime%3==0 ) {
            gerarNovoProcesso();
            solicitaListaProcessos();
        }

        elapseTime++;

        if (elapseTime == 5) { run = 0; }
    }
}

// Faz a simulação de criação de um novo processo, ativando-o para execução
void gerarNovoProcesso() {
    for(int i = 0; i < NUMBER_PROCESS; i++) {
        if ( listaProcessos[i].ativo == 0 ) {
            listaProcessos[i].ativo = 1;

            printf("Processo %d criado.\n", i);
            break;
        }
    }
}

//percorre o vetor de processo e solicita uma pagina aleatoria para cada um que estiver ativo
void solicitaListaProcessos() {
    for(int i = 0; i < NUMBER_PROCESS; i++) {
        
        if( listaProcessos[i].ativo == 1 ) {
            int randomPage = rand() % NUMBER_PAGES;
            
            printf("Tabela de paginas do processo %d:\n", listaProcessos[i].pid);
            for (int j = 0; j < NUMBER_PAGES; j++) {
                printf("Pagina = %d | Frame = %d \n", listaProcessos[i].paginationTable[j].index, listaProcessos[i].paginationTable[j].address);
            }
        }

        // if(listaProcessos[i].ativo == 1){

        //     int randomPage = rand() % NUMBER_PAGES;
        //     printf("ramdomPage: %d \n", randomPage);
        //     //algoritmoLRU(listaProcessos[i].pid, randomPage);

        //     printf("Tabela de paginas do processo %d:\n", listaProcessos[i].pid);
        //     for (int j = 0; j < listaProcessos[i].paginationTable; j++)
        //     {
        //         printf("Pagina = %d | Frame = %d \n", listaProcessos[i].paginationTable[j].index, listaProcessos[i].paginationTable[j].address);
        //     }

        //     break;
            
        // }
    }
}

// algoritmo de substituição LRU
void algoritmoLRU(int pid, int page) {

    //percorrendo a lista de substituicao 
    for(int i = 0; i < NUMBER_FRAME; i++) {
        
        // checar se já existe na lista
        if ( algoritmo.pageInUse[i].index == page && algoritmo.pageInUse[i].indexProccess == pid ) {
            updateNotFault(pid, page);
            break;
        }
        else {
            inserirPageNaMemoria(pid, page);
            break;
        }

    }   
}

// verifica se vai ocacionar troca de pegainas ou só um page fault
void inserirPageNaMemoria(int pid, int page) {
    int qtdPageInMemory = 0;

    // verifica a quantidade de paginas na lista 
    for (int i = 0; i < NUMBER_FRAME; i++) {
        if ( algoritmo.pageInUse[i].indexProccess == pid ) {
            qtdPageInMemory++;
        }
    }

    // verifica se passou do limite
    if (qtdPageInMemory >= WSL) {
        // remover uma pagina para adiciona
        updateList(pid, page);
    }
    else {
        add(pid, page);
    }
}

// adiciona na lista LRU
void add(int pid, int page) {
    for(int i = 0; i < NUMBER_FRAME; i++) {

        if (algoritmo.pageInUse[i].index == 0) {
            algoritmo.pageInUse[i].index = page;
            algoritmo.pageInUse[i].indexProccess = pid;
            algoritmo.pageInUse[i].address = i;
            
            printf("PAGE FAULT: Processo = %d | Pagina = %d \n", pid, page);
            printf("Sem substituição! \n");
            
            break;
        }
    }
}

// remove o processo mais antigo para trocar pela pagina
void updateList(int pid, int pageId) {
    page removido = algoritmo.pageInUse[0];

    for(int i = 0; i < NUMBER_FRAME - 1; i++) { 
        algoritmo.pageInUse[ i ] = algoritmo.pageInUse[ i + 1 ]; 
    }

    for (int frame = 0; frame < NUMBER_FRAME - 1; frame++) {
        if ( algoritmo.pageInUse[ frame ].index == 0 ) {
            algoritmo.pageInUse[ frame ].index = pageId;
            algoritmo.pageInUse[ frame ].indexProccess = pid;
            algoritmo.pageInUse[ frame ].address = frame;
        }
    }

    printf("PAGE FAULT: Processo = %d | Pagina = %d \n", pageId, pid);
    printf("Pagina %d do processo %d substituida! \n", removido.index, removido.indexProccess);
}

void updateNotFault(int pid, int pageId) {
    page removido = algoritmo.pageInUse[0];

    for(int i = 0; i < NUMBER_FRAME - 1; i++) { 
        algoritmo.pageInUse[ i ] = algoritmo.pageInUse[ i + 1 ]; 
    }

    for (int frame = 0; frame < NUMBER_FRAME - 1; frame++) {
        if ( algoritmo.pageInUse[ frame ].index == 0 ) {
            algoritmo.pageInUse[ frame ].index = pageId;
            algoritmo.pageInUse[ frame ].indexProccess = pid;
            algoritmo.pageInUse[ frame ].address = frame;
        }
    }

    printf("NOT PAGE FAULT: Processo = %d | Pagina = %d \n", pageId, pid);
    printf("Sem substituição! \n");
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
