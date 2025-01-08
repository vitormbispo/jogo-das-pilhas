#include <math.h>
#include <ncurses.h>

#define MAX_RAND_COUNT 100000

float randomNumbers[MAX_RAND_COUNT];

int lastN = 0;
int curRandom = 0;

/*
*   Gera uma lista de números pseudo-aleatórios.
*/
int generateRandomNumberList(int seed){
    int const AA = 5;
    int const BB = 7;
    int const MAX = 100000;
    int X0 = seed;

    for(int i=0; i < MAX_RAND_COUNT;i++){
        float X1 = (AA*X0+BB)%MAX;
        X0 = X1;
        float resultado = X1/MAX;
        randomNumbers[i] = resultado;
    }
    lastN = X0;
    return 0;
}

/* Gera um número inteiro pseudo-aleatório
*  rMin = mínimo
*  rMax = máximo
*/
int randNextInt(int rMin, int rMax){
    float pseudo = randomNumbers[curRandom];
    curRandom++;
    if (curRandom >= MAX_RAND_COUNT)
    {
        generateRandomNumberList(lastN);
        curRandom = 0;
        
    }
    
    return(round(((rMax-rMin)*pseudo)+rMin));
}

/* Gera um número inteiro pseudo-aleatório
*  rMin = mínimo
*  rMax = máximo
*/
float randNextFloat(int rMin, int rMax){
    float pseudo = randomNumbers[curRandom];
    curRandom++;
    if (curRandom >= MAX_RAND_COUNT)
    {
        generateRandomNumberList(lastN);
        curRandom = 0;   
    }
    
    return(((rMax-rMin)*pseudo)+rMin);
}