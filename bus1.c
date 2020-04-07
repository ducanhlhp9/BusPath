#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"

typedef struct {
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);
int outdegree(Graph graph, int v, int* output);
void dropGraph(Graph graph);
int getId(Graph graph,char *name);
void printVertex(int v,Graph g)
    {
        printf("%s   ------>     ",getVertex(g,v));
    }
void nhap_thog_so(Graph graph,FILE *fo)
{
    int dem = 0 ;
    while(!(feof(fo)))
        {
            char buff1[20];
            char buff2[800];
            fgets(buff1,20,fo);
            fgets(buff2,800,fo); 
            char *token = strtok(buff2,"-");
            if( (getId(graph,token)) == -1)
                {
                    addVertex(graph,dem,token);
                    dem++;
                }
            while(token != NULL)
                {
           
                    char *str1 = token; 
                    token = strtok(NULL,"-");
                    char *str2 = token;        
                    if(token != NULL)
                        {
                            int x = getId(graph,str1);
                            int y = getId(graph,str2);
                            if(x != -1 &&  y != -1) addEdge(graph,x,y);
                                else if(x != -1 && y == -1)
                                    {
                                        addVertex(graph,dem,str2);
                                        addEdge(graph,x,dem);
                                        dem++;
                                    }
                        }
                }
        }
}

int main()
    {
        FILE* fi=fopen("bus.txt","r");
        Graph g =createGraph();
        int i, n, output[1000], size;
        int x;
        char d1[1000],d2[1000];
        nhap_thog_so(g,fi);
        do
            {
                printf("----------MENU----------\n");
                printf("1:tim duong     --------\n");
                printf("2:dong          --------\n");
                scanf("%d",&x);
                if (x<1||x>2) 
                     {
                        printf("ban nhap sai\n");
                        printf("nhap lai:");
                        scanf("%d",&x);
                     }
                if (x==1)
                    {
                        __fpurge(stdin);
                        printf("nhap diem dau tien:");
                        gets(d1);
                        printf("nhap diem cuoi:");
                        gets(d2);
                        int a,b;
                        a=getId(g,d1);
                        b=getId(g,d2);
                        if (a==-1) printf("khong co diem dau tien\n");
                        if (b==-1) printf("khong co diem cuoi\n");
                        if (a!=-1 && b!=-1) BFS(g,a,b);   
                    }
                if (x==2) break;
            } while(x!=2);
    }
int getId(Graph graph,char *name)
{
    JRB node;
    jrb_traverse(node,graph.vertices){
         if(strcmp(name,jval_s(node->val)) == 0) return (jval_i(node->key));
    }
    return -1;
}
void addVertex(Graph graph, int id, char* name)
    {
        JRB node = jrb_find_int(graph.vertices, id);
        if (node==NULL) jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
    }

char *getVertex(Graph graph, int id)
    {
        JRB node = jrb_find_int(graph.vertices, id);
        if (node==NULL) return NULL;
            else return jval_s(node->val);
    }

void addEdge(Graph graph, int v1, int v2)
    {
        JRB node, tree;
        if (!hasEdge(graph, v1, v2))
            {
                node = jrb_find_int(graph.edges, v1);
                if (node==NULL) 
                    {
                        tree = make_jrb();
                        jrb_insert_int(graph.edges, v1, new_jval_v(tree));
                    } else
                        {
                            tree = (JRB) jval_v(node->val);
                        }
                jrb_insert_int(tree, v2, new_jval_i(1));
            }
    if (!hasEdge(graph, v2, v1))
        {
            node = jrb_find_int(graph.edges, v2);
            if (node==NULL) 
                {
                    tree = make_jrb();
                    jrb_insert_int(graph.edges, v2, new_jval_v(tree));
                } else 
                    {
                        tree = (JRB) jval_v(node->val);
                    }
            jrb_insert_int(tree, v1, new_jval_i(1));
        }
    }

int hasEdge(Graph graph, int v1, int v2)
    {
        JRB node, tree;
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) return 0;
        tree = (JRB) jval_v(node->val);
        if (jrb_find_int(tree, v2)==NULL) return 0;
            else return 1;
    }

int outdegree(Graph graph, int v, int* output)
    {
        JRB tree, node;
        int total;
        node = jrb_find_int(graph.edges, v);
        if (node==NULL) return 0;
        tree = (JRB) jval_v(node->val);
        total = 0;
        jrb_traverse(node, tree)
            {
                output[total] = jval_i(node->key);
                total++;
            }
        return total;
    }

void BFS(Graph graph, int start, int stop, void (*func)(int))
    {
        Dllist queue,node;
        queue = new_dllist();
        dll_append(queue, new_jval_i(start));
        int visited[1000],output[1000],number,value,value2,index,vet[1000],ves[1000];
        memset(visited,0,1000);
        memset(vet,-1,1000);
        memset(ves,-1,1000);
        node=dll_first(queue);
        value2=jval_i(node->val);
        visited[start]=1;
        while(!dll_empty(queue))
            {
                node=dll_first(queue);
                value=jval_i(node->val);
                dll_delete_node(node);
                if(value==stop)
                    {
                         int value1,j;
                         j=0;
                         value1=value;
                         while (vet[value1]!=-1) 
                             {
                                ves[j++]=value1;
                                value1=vet[value1];
                             }
                         ves[j]=value2;
                         //if (j==1) printf("khong co duong di\n"); 
                          for (int i=j;i>0;i--)  printVertex(ves[i],graph);  
                            printf("%s",getVertex(graph,ves[0]));

                            printf("\n");
                         break;
                    }   
                number=outdegree(graph,value,output);
                for(index=0;index<number;index++)
                    {
                        if(visited[output[index]]==0)
                            {
                                visited[output[index]]=1;
                                vet[output[index]]=value;
                                dll_append(queue,new_jval_i(output[index]));
                            }
                    }
            }   
    }
Graph createGraph()
    {
        Graph g;
        g.edges = make_jrb();
        g.vertices = make_jrb();
        return g;
    }

void dropGraph(Graph graph)
    {
        jrb_free_tree(graph.edges);
        jrb_free_tree(graph.vertices);
    }
