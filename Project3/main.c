#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<limits.h>
#define CAPACITY 100

typedef struct orasele
{
    char nume[40];
    int id_uri;
    struct orasele *next;
} orase;

typedef struct
{
    int V;
    int E;
    int ** a;

} Graph;

typedef struct node
{
    char *val;
    struct node *next;

} stack_node_t;

typedef struct stack
{
    stack_node_t *head;
    unsigned int length;

} stack_t;

Graph * create (int v)
{
    int i;
    Graph *g = (Graph *) malloc (sizeof(Graph));
    if (g==NULL)
    {
        printf ( " no memory for this graph");
        return NULL;
    }

    g->V=v;

    g->a = (int **) malloc(g->V*sizeof(int*));

    for (i=0; i<g->V; i++)
        g->a[i] = (int *) calloc(g->V, sizeof(int));

    if (g->a==NULL)
    {
        printf ( " no memory for this matrix");
        return NULL;
    }

    g->E = 0;

    return g;
}

stack_t *stack_create()
{
    stack_t *stack = NULL;
    stack=(stack_t*)malloc(sizeof(stack_t));
    stack->length=0;
    stack->head=NULL;
    return stack;
}

void stack_push(stack_t *stack,char *val)
{
    stack_node_t *node = NULL;
    node = (stack_node_t *)malloc(sizeof(stack_node_t));
    if (stack->length >= CAPACITY)
    {
        printf("Stack Overflow, can't add more element to stack.\n");
        return;
    }

    node->val=strdup(val);
    node->next = stack->head;
    stack->head= node;
    stack->length++;

}

char * stack_pop(stack_t *stack)
{
    char *val;
    stack_node_t *aux;
    if (stack->head == NULL)
    {
        return NULL;
    }
    aux=stack->head;
    val=strdup(aux->val);
    stack->head=stack->head->next;
    free(aux);
    stack->length--;
    return val;
}

orase *append_node(orase *head, char *a)
{
    orase *A;
    A = (orase *)calloc(1,sizeof(orase));
    strcpy(A->nume,a);
    A->next = NULL;

    if (head == NULL)
    {
        head = A;
        head->id_uri = 0;
        return head;
    }
    else
    {
        orase *aux = head;
        while (aux->next != NULL)
            aux = aux->next;
        aux->next = A;
        A->id_uri = aux->id_uri + 1;
        return head;
    }
}

int searching(orase *head, char *a)
{
    if(head==NULL)
        return 0;
    while(head!=NULL)
    {
        if(strcmp(head->nume,a)==0)
            return 1;
        head=head->next;
    }
    return 0;
}

int searching2(orase *head, char *a)
{

    while(head!=NULL)
    {
        if(strcmp(head->nume,a)==0)
            return head->id_uri;
        head=head->next;
    }
    return -1;
}

char *getcity(orase *head,int poz)
{
    int k=0;
    while(k!=poz)
    {
        head=head->next;
        k++;
    }
    return head->nume;

}

int minDistance(int dist[], int sps[], int V)
{
    int v, min = INT_MAX, min_index;
    for ( v = 0; v < V; v++)
        if (sps[v] == 0 && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    return min_index;
}

void dijkstra(orase *head,Graph *g, int s,int k )
{
    FILE *file_ruta,*file_departe;

    file_ruta=fopen("ruta.txt","wt");
    if(file_ruta==NULL)
    {
        printf("Fisierul %s nu se poate deschide !","ruta.txt");
        exit(1);
    }

    int i, j, u,pozitie=0,maxim=INT_MIN,poz2=-1,pred[g->V],pozitie2;
    int dist[g->V];
    int sps[g->V];
    stack_t * stack=NULL;
    stack=stack_create();
    orase *headcopy;
    headcopy=head;
    pozitie=searching2(headcopy,"Slatina");
    for ( i = 0; i < g->V; i++)
    {
        dist[i] = INT_MAX;
        sps[i] = 0;
        pred[i]=-1;
    }

    dist[s] = 0;

    for (j = 0; j < g->V-1; j++)
    {
        u = minDistance(dist, sps, g->V);
        sps[u] = 1;
        for (i = 0; i < g->V; i++)
        {
            if (sps[i] == 0 && g->a[u][i]!=0 && dist[u] != INT_MAX && dist[u]+g->a[u][i] < dist[i])
            {
                dist[i] = dist[u] + g->a[u][i];
                pred[i]=u;
            }
        }
    }
    fprintf(file_ruta,"Drumul de cost minim de la Bucuresti la Slatina este: %d km .\n", dist[pozitie]);
    fclose(file_ruta);

    file_departe=fopen("departe.txt","wt");
    if(file_ruta==NULL)
    {
        printf("Fisierul %s nu se poate deschide !","departe.txt");
        exit(1);
    }
    for(i=0; i<g->V; i++)
    {
        if(maxim<dist[i])
        {
            maxim=dist[i];
            poz2=i;
        }
    }
    pozitie2=poz2;
    fprintf(file_departe,"Statia cea mai departe de Bucuresti este : %s .\nDrumul este: \n",getcity(headcopy,poz2));

    while(pozitie2!=-1)
    {
        stack_push(stack,getcity(head,pozitie2));
        pozitie2=pred[pozitie2];
    }

    while(stack->head!=NULL)
    {
        fprintf(file_departe,"%s\n",stack_pop(stack));
    }

    fclose(file_departe);

}


void dijkstra2(orase *head,Graph *g, int s,int *maxim,int *pozitia)
{

    int i, j, u;
    int dist[g->V];
    int sps[g->V];
    int maxi=INT_MIN;
    for ( i = 0; i < g->V; i++)
    {
        dist[i] = INT_MAX;
        sps[i] = 0;
    }

    dist[s] = 0;

    for (j = 0; j < g->V-1; j++)
    {
        u = minDistance(dist, sps, g->V);
        sps[u] = 1;
        for (i = 0; i < g->V; i++)
        {
            if (sps[i] == 0 && g->a[u][i]!=0 && dist[u] != INT_MAX && dist[u]+g->a[u][i] < dist[i])
            {
                dist[i] = dist[u] + g->a[u][i];
            }
        }
    }
    for(i=0; i<g->V; i++)
    {
        if(maxi<dist[i])
        {
            maxi=dist[i];
            j=i;
        }
    }
    *maxim=maxi;
    *pozitia=j;

}


int main()
{
    FILE *fisier,*file2,*file_min,*file_max,*file_total,*file_drum;
    char b[40],orasul[50],*token,y[40],city[40];
    int i,k,primul_el=1,poz1=-1,poz2=-1,s=0,km1=0,km2=0,minim=10000,maxim=-10000,j,aux1=0,aux2=0,poz_Bucuresti=0,maxim_total,pozitie3=0,pozitie4=-1;
    orase *head,*aux,*headcopy;
    Graph *Adiacenta,*Cost,*Orientat;
    head=aux=NULL;
    k=0;
    maxim_total=INT_MIN;

    fisier=fopen("_all_files.txt","rt");
    if(fisier==NULL)
    {
        printf("Fisierul %s nu se poate deschide !","_all_files.txt");
        exit(1);
    }
    while (fgets(b,40,fisier))
    {

        if(b[0]!='\0')
        {
            b[strlen(b)-1]='\0';
        }

        file2=fopen(b,"rt");
        if(file2==NULL)
        {
            printf("Fisierul %s.txt nu se poate deschide !",b);
            exit(1);
        }
        while(fgets(orasul,50,file2))
        {
            orasul[strlen(orasul)-1]='\0';
            token = strtok (orasul,",");
            if(searching(aux,token)==0)
            {
                aux=append_node(aux,token);
                k++;
            }

        }
        fclose(file2);
    }

    fclose(fisier);

    printf("Sunt %d orase!",k);

    head=aux;

    Adiacenta=create(k);
    Cost=create(k);
    Orientat=create(k);

    fisier=fopen("_all_files.txt","rt");

    while (fgets(b,40,fisier))
    {
        if(b[0]!='\0')
        {
            b[strlen(b)-1]='\0';
        }

        file2=fopen(b,"rt");

        if(file2==NULL)
        {
            printf("Fisierul %s.txt nu se poate deschide !",b);
            exit(1);
        }
        y[0]=' ';
        while(fgets(orasul,50,file2))
        {
            orasul[strlen(orasul)-1]='\0';
            token = strtok (orasul,",");
            strcpy(city,token);
            token = strtok(NULL, " ");
            km1=atoi(token);
            if(primul_el!=1)
            {
                poz1=searching2(head,city);
                poz2=searching2(head,y);
                Adiacenta->a[poz1][poz2]=Adiacenta->a[poz2][poz1]=1;
                Orientat->a[poz2][poz1]=1;
                Cost->a[poz2][poz1]=Cost->a[poz1][poz2]=km1-km2;
                if((km1-km2)< minim)
                    minim=(km1-km2);
                if((km1-km2)> maxim)
                {
                    maxim=(km1-km2);
                    aux1=poz1;
                    aux2=poz2;
                }
            }

            primul_el++;
            strcpy(y,city);
            km2=km1;
        }
        fclose(file2);
        primul_el=1;
    }

    fclose(fisier);

    file_total=fopen("total.txt","wt");
    if(file_total==NULL)
    {
        printf("Fisierul %s nu se poate deschide !","total.txt");
        exit(1);
    }

    for(i=0; i<k; i++)
    {
        for(j=0; j<k; j++)
            s+=Cost->a[i][j];
    }
    headcopy=head;
    fprintf(file_total,"km totali = %d",s/2);
    fclose(file_total);

    file_min=fopen("min.txt","wt");
    if(file_min==NULL)
    {
        printf("Fisierul %s nu se poate deschide !","min.txt");
        exit(1);
    }

    fprintf(file_min,"Distanta minima este de %d km intre statiile:\n",minim);

    for (i = 1; i < k-1; i++)
    {
        for (j = i+1; j < k; j++)
        {
            if ( Cost->a[i][j]==minim)
            {
                fprintf(file_min,"\n%s si %s",getcity(headcopy,i),getcity(headcopy,j));

            }
        }
    }
    fclose(file_min);


    file_max=fopen("max.txt","wt");
    if(file_min==NULL)
    {
        printf("Fisierul %s nu se poate deschide !","max.txt");
        exit(1);
    }

    headcopy=head;
    poz_Bucuresti=searching2(head,"Bucuresti Nord");
    // printf("poz_bucuresti = %d",poz_Bucuresti);
    fprintf(file_max,"Distanta maxima este de %d km intre statiile %s si %s .\n",maxim,getcity(headcopy,aux2),getcity(headcopy,aux1));
    fclose(file_max);

    headcopy=head;
    dijkstra(headcopy,Cost,poz_Bucuresti,k);

    int max1,pozitiee=0;
    for(i=0; i<k; i++)
    {
        dijkstra2(headcopy,Cost,i,&max1,&pozitie3);
        if(max1>maxim_total)
        {
            pozitie4=i;
            maxim_total=max1;
            pozitiee=pozitie3;
        }
    }
    file_drum=fopen("drum_lung.txt","wt");
    if(file_drum==NULL)
    {
        printf("Fisierul %s nu se poate deschide !","drum_lung.txt");
        exit(1);
    }

    fprintf(file_drum,"%d km intre : %s si %s .",maxim_total,getcity(head,pozitie4),getcity(head,pozitiee));
    fclose(file_drum);

    return 0;
}
