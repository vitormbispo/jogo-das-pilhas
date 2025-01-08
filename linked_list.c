#include <stdlib.h>
typedef struct Node{
    int val;
    struct Node* next;
} Node;

void addVal(Node *p, int val);
int getVal(Node *p,int i);
void delVal(Node *p,int val);
Node* initializeList();

void addVal(Node *p, int val){
    Node *q = p;

    if(p->val > val){
        Node *new = (struct Node*) malloc(32);
        new->next = q;
        new->val = val;
        p = new;
    }
    else{
        while (q->next != NULL && q->next->val < val){
            q = q->next;
        }
        Node *new = (struct Node*) malloc(32);
        new->val = val;
        new->next = q->next;
        q->next = new;
    }
}

int getVal(Node *p,int i){
    int j = 0;
    Node* q = p;

    while (j < i)
    {
        if(q != NULL){
            q = q->next;
            ++j;
        }
        else{
            break;
        }
    }
    
    return q->val;
}

void delVal(Node *p,int val){
    int j = 0;
    Node* q = p;

    while (q != NULL && q->next->val != val)
    {
        q = q->next;
    }
    Node* r = q->next;
    q->next = q->next->next;
    free(r);
    r = NULL;
}

Node* initializeList(){
    Node* newNode = (struct Node*) malloc(32);
    newNode->next = NULL;
    newNode->val = 0;
    return newNode;
};