#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void delay(int number_of_seconds);
void escreveArquivo(char string[], int pid);

int main() {
    return 0;
}


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
