#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

#include "pilha.c"
#include "random.c"

#define TAMANHO_PILHA 4
#define QUANT_PILHAS 5
#define PILHAS_VAZIAS 2
#define REINICIO1 114
#define REINICIO2 82
#define NOVA1 78
#define NOVA2 110
#define FIM1 102
#define FIM2 70
struct LoL{
    int champ;
};

Pilha *pilhas;
Pilha *pilhasIniciais;
int caractere_no_indice(Pilha p,int i);
void encherPilha(Pilha *p, int tam);
void esvaziaPilha(Pilha p);
void renderizarPilhas();
void iniciaTela();
void movimentarPilhas(int from, int to);
Pilha *iniciaPilhas();
int checar_vitoria();
int checar_entrada(char in);
void salvaInicial();
int checa_reinicio(int in);
int checa_nova(int in);
int checa_fim(int in);
void carregarInicial();
void limparPilhas();
int inseridos[7];
int quantNumeros = 3;

int main(){

    

    //Seed teste: 3267320
    generateRandomNumberList(time(NULL));
    
    iniciaTela();

    INICIO:;
    for (int i = 0; i < 7;i++) {
        inseridos[i] = 0;
    }

    printw("\n---Bem vindo ao jogo!---");
    printw("\n\nRegras: -Selecione uma pilha para remover o número do topo \n -Selecione outra pilha para inserir esse número no topo");
    printw("\n -Um número deve ser inserido em uma pilha vazia ou no topo de um número semelhante");
    printw("\n -Um número não pode ser inserido no topo de uma pilha cheia (com 4 números)");
    printw("\n -Não é possível remover um número de uma pilha vazia");
    printw("\n -O jogo acaba quando todos os números iguais estiverem na mesma pilha e duas pilhas restam vazias.");
    printw("\n -Pressione \'R\' para reiniciar a tentativa.");
    printw("\n -Pressione \'N\' para uma nova partida.");
    printw("\n -Pressione \'F\' para finalizar o jogo.");

    printw("\n\n -Escolha a quantidade de números para jogar (entre 1 e 7)");

    
    do{

        char in;
        if (checa_fim(in = getch())) {
            goto FIM;
        }
        quantNumeros = in-'0';
        if(quantNumeros < 1 || quantNumeros > 7){
            printw("\n /!\\ESCOLHA UM NUMERO ENTRE 1 E 7");
        }
    }
    while (quantNumeros < 1 || quantNumeros > 7);

    pilhas = iniciaPilhas();

    salvaInicial();


    int input = -1;
    int src = -1;
    int des = input;
    
    do{
        JOGO:;
        des = input;
        while (des != -1 && !checar_entrada(des))
        {
            printw("%s\n","/!\\ Entrada inválida! Tente novamente");
            des = getch();

        }
        if (checa_reinicio(des)) {
            carregarInicial();
            input = -1;
            src = -1;
            clear();
            renderizarPilhas();
            goto JOGO;
        }
        if (checa_nova(des)) {
            clear();
            limparPilhas();
            goto INICIO;;
        }
        if (checa_fim(des)) {
            goto FIM;
        }


        des = (des-'0')-1;
        
        if(src != -1 && des != -1){
            movimentarPilhas(src,des);
        }
        clear();
        renderizarPilhas();
        
         if(checar_vitoria()){
            printw("%s\n", "---PARABENS! Você venceu!---");
            printw("%s\n\n"," Pressione \'R\' para uma nova partida. ");

            int continuar = getch();
            if(checa_reinicio(continuar)){
                limparPilhas();
                clear();
                goto INICIO;
            }
        }

        else{
            printw("%s\n"," Escolha a pilha de origem: ");
            
            while (!checar_entrada(((src=getch()))))
            {
                printw("%s\n","/!\\ Entrada inválida! Tente novamente");
            }
            if (checa_reinicio(src)) {
                carregarInicial();
                input = -1;
                src = -1;
                clear();
                renderizarPilhas();
                goto JOGO;
            }
            if (checa_nova(src)) {
                clear();
                limparPilhas();
                goto INICIO;
            }
            if (checa_fim(src)) {
                goto FIM;
            }
            src = (src-'0')-1;
            printw("%s\n","Escolha a pilha de destino: ");
        }
        
    } while ((input = getch()) != KEY_F(2));

    FIM:;
    limparPilhas();
    endwin();
    return 0;
}

void iniciaTela(){
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    clear();
    refresh();
}

void salvaInicial(){
    free(pilhasIniciais);
    pilhasIniciais = malloc((quantNumeros+2)*sizeof(Pilha));
    for (int i = 0;i<quantNumeros+2;i++) {
        if (pilhasIniciais != NULL) {
            esvaziaPilha(pilhasIniciais[i]);
        }
        pilhasIniciais[i] = copiaPilha(pilhas[i]);
    }
}

void carregarInicial(){
    for (int i = 0;i<quantNumeros+2;i++) {
        esvaziaPilha(pilhas[i]);
        pilhas[i] = copiaPilha(pilhasIniciais[i]);
    }
}

void limparPilhas() {
    for (int i = 0;i<quantNumeros+2;i++) {
        esvaziaPilha(pilhas[i]);
        esvaziaPilha(pilhasIniciais[i]);
    }
}

Pilha *iniciaPilhas(){
    printw("Iniciando pilhas...");
    Pilha *novasPilhas = (Pilha *)malloc(quantNumeros+2 * sizeof(Pilha));
    for(int i = 0; i < quantNumeros;i++){
        Pilha novaPilha = inicializa();
        encherPilha(&novaPilha,TAMANHO_PILHA);
        novasPilhas[i] = novaPilha;
    }
    
    for(int i = quantNumeros; i < quantNumeros+2;i++){
        novasPilhas[i] = inicializa();
    }
    return novasPilhas;
}

void encherPilha(Pilha *p, int tam){

    for(int i = 0; i < tam; i++){
        const int val = randNextInt(1,quantNumeros);
        if (inseridos[val] < TAMANHO_PILHA) {
            empilha(&(p->topo),val);
            inseridos[val]++;
        }
        else {
            i--;
        }

    }
}

void esvaziaPilha(Pilha p){
    while (!pilha_vazia(&(p.topo)))
    {
        desempilha(&(p.topo));
    }
}

void renderizarPilhas(){
    for (int i = 0;i<quantNumeros+2;i++) {
        printw("|%d",i+1);
    }
    printw("|\n=");
    for (int i = 0;i<(quantNumeros+2)*2;i++) {
        printw("=");
    }
    printw("\n");
    for(int i = TAMANHO_PILHA-1; i >= 0;i--){
        for(int j = 0; j < quantNumeros+2;j++){
            int val = caractere_no_indice(invertePilha(pilhas[j]),i);
            if (val != 32) {
                val = val + '0';
            }
            const char caractere = val;
            printw("%s%c","|",caractere);
        }
        printw("%s\n","|");
    }
}

void movimentarPilhas(int from, int to){
    printw("\nMovimentando: ");
    Pilha *fonte = &pilhas[from];
    Pilha *destino = &pilhas[to];
    
    if(!pilha_vazia(&(fonte->topo))){
        
        if(pilha_vazia(&(destino->topo)) || topo_pilha(*fonte) == topo_pilha(*destino)){
            if(tamanho_pilha(destino->topo) < TAMANHO_PILHA){
                int val = desempilha(&(fonte->topo));
                empilha(&(destino->topo),val);
            }
            else{
                printw("%s\n","Jogada inválida: pilha de destino cheia.");
                printw("%s\n","Pressione qualquer tecla para continuar...");
                getch();
            }
        
        }
        else{
            printw("%s%d%s%d\n","Jogada inválida: topo de destino inválido. Fonte: ",from,". Destino: ",to);
            printw("%s\n","Pressione qualquer tecla para continuar...");
            getch();
        }
        
    }
    else{
        printw("%s\n","Jogada inválida: pilha fonte vazia.");
        printw("%s\n","Pressione qualquer tecla para continuar...");
        getch();
    }
}

int checar_vitoria(){
    int vazias = 0;

    for(int i = 0; i < quantNumeros+2;i++){
        Pilha atual = pilhas[i];
        Node *q = atual.topo;

        if(q == NULL){
            vazias = vazias + 1;
            continue;
        }
        const int inicial = atual.topo->val;

        while (q != NULL)
        {
            if(q->val != inicial){
                return FALSE;
            }
            q = q->next;
        }
    }
    return vazias == 2;
}

int caractere_no_indice(Pilha p,int i){
    Node *atual = p.topo;
    if(atual == NULL) {
        return 32;
    }

    for(int j = 0; j < i;j++){
        if(atual != NULL){
            atual = atual->next;
        }
        else{
            return 32;
        }
    }
    if(atual != NULL){
        return atual->val;
    }
    return 32;
}

int checar_entrada(char in){
    printw("%d\n",in-'0');
    return (in-'0' >= 1 && in-'0' <= quantNumeros+2)||(checa_reinicio(in))||checa_nova(in)||checa_fim((int)in);
}

int checa_reinicio(int in) {
    return in==REINICIO1 || in==REINICIO2;
}

int checa_nova(int in) {
    return in==NOVA1 || in==NOVA2;
}

int checa_fim(int in) {
    return in == FIM1 || in == FIM2;
}

