#include <stdio.h>
#include <stdlib.h>
#include "linked_list.c"

typedef struct{
    Node* topo;
}Pilha;


void empilha(Node **p,int valor);
int desempilha(Node **p);
void varre_pilha(Node* p);
int pilha_vazia(Node **p);
int indice_pilha(Pilha p,int i);
int tamanho_pilha(Node* p);
Pilha invertePilha(Pilha p);
Pilha inicializa();

void empilha(Node **p,int valor){
    Node* q = (Node*) malloc(sizeof(Node));
    q->next = (*p);
    q->val = valor;
    (*p) = q;
};

Pilha copiaPilha(Pilha p) {
    Node* q = p.topo;
    Pilha pilhaCopia = inicializa();
    if(q == NULL) {
        return pilhaCopia;
    }

    pilhaCopia.topo = malloc(sizeof(Node));
    Node* r = pilhaCopia.topo;

    while(q != NULL) {
        r->val = q->val;

        q = q->next;
        if (q != NULL) {
            r->next = malloc(sizeof(Node));
        }
        r = r->next;
    }
    return pilhaCopia;

}

int topo_pilha(Pilha p){
    if(p.topo == NULL){
        return -999;
    }
    else{
        return p.topo->val;
    }
    
}

int desempilha(Node **p){
    if(!pilha_vazia(&(*p))){
        int val = (*p)->val;
        Node* q = (*p);
        (*p) = q->next;
        return val;
    }
    return -999;
}

int pilha_vazia(Node **p){
    return (*p) == NULL;
}
void varre_pilha(Node* p){
    Node* q = p;
    while (q != NULL)
    {
        printf("\n%i",q->val);
        q = q->next;
    }
    
}

int indice_pilha(Pilha p,int i){
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
    return -1;
}

int tamanho_pilha(Node* p){
    if(p == NULL){
        return 0;
    }
    else{
        return 1 + tamanho_pilha(p->next);
    }
    
}

Pilha inicializa(){
    Pilha* novaPilha = malloc(sizeof(Pilha));
    novaPilha->topo = NULL;
    return *novaPilha;
}

Pilha invertePilha(Pilha p){
    Pilha q = inicializa();
    
    while (!pilha_vazia(&(p.topo)))
    {
        int valor = desempilha(&(p.topo));
        empilha(&q.topo,valor);
    }
    return q;
    
}