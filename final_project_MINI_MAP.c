
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define N 10000

int graph[N][N];
int distance[N];
bool visited[N];
int parent[N];

struct pairNode {
    char name[1000];
    int index;
};

struct pairNode nodes[N];
int nodeCount = 0;

//Function to get index of a node, or add it if not present
int getNodeIndex(char *name) {
    for (int i = 0; i < nodeCount; i++) {
        if (strcmp(nodes[i].name, name) == 0){
            return nodes[i].index;
        }
    }
    
    //Add new node
    strcpy(nodes[nodeCount].name, name);
    nodes[nodeCount].index = nodeCount;
    return nodeCount++;
}

int findMinVertex(int n) {
    int minVertex = -1;
    for (int i = 0; i < n; i++) {
        /*jodi current index e previous index er ceyeo minimum distance thake*/
        if (!visited[i] && (minVertex == -1 || distance[i] < distance[minVertex])) {
            minVertex = i;  
        }
    }
    return minVertex;
}

void printPath(int node , FILE * output_file) {
    if (parent[node] == -1) {
        fprintf(output_file,"%s", nodes[node].name);
        return;
    }
    printPath(parent[node] , output_file);
    fprintf(output_file, " -> %s", nodes[node].name);
}

void dijkstra(int src, int n) {   /*n=total node*/
    for (int i = 0; i < n; i++) {
        distance[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;
    }
    distance[src] = 0;

    for (int i = 0; i < n - 1; i++) {

        int minVertex = findMinVertex(n); /*parent Node = minvertex*/
        if (minVertex == -1) break;
        visited[minVertex] = true;

        for (int j = 0; j < n; j++) {
            if (graph[minVertex][j] > 0 && !visited[j] && distance[minVertex]!= INT_MAX) {
                int newDist = distance[minVertex] + graph[minVertex][j];  /*graph[minvertex][j] is the child of minVertex*/
                if (newDist < distance[j]) { /*path relaxation hobe */
                    distance[j] = newDist;
                    parent[j] = minVertex; /*j-th index er parent minVertex*/
                }
            }
        }
    }
}

void printShortestPaths(int src, int n , FILE * output_file) {
    fprintf( output_file , "Node -> Distance -> Path\n");
    for (int i = 0; i < n; i++) {
        fprintf(output_file,"%s\t", nodes[i].name);
        if (distance[i] == INT_MAX) {
            fprintf(output_file, "INF\t\t\tNo path\n");
        } else {
            fprintf(output_file, "%d\t\t\t", distance[i]);
            printPath(i,output_file);
            fprintf(output_file, "\n");
        }
    }
}

int main() {
    FILE *input_file=fopen("input.txt","r");
    FILE *output_file = fopen("output.txt" ,"w");
    if(input_file==NULL){
        printf("ERRORS occuring. Input File is not  Opened\n");
        return 1;
    }
    if(output_file==NULL){
        printf("ERRORS occuring. OUTPUT File is not  Opened\n");
        return 1;
    }
    int e;
/*  fprintf(input_file, "Enter the number of edges: "); */ 
    fscanf(input_file,"%d", &e);

    memset(graph, 0, sizeof(graph));

    fprintf(output_file, "Enter the edges (e.g., A B 10):\n");
    for (int i = 0; i < e; i++) {
        char name1[100], name2[100], arrow[3];
        int cost;
        fscanf(input_file, "%s  %s %d", name1,  name2, &cost);

        int a = getNodeIndex(name1);
        int b = getNodeIndex(name2);

        graph[a][b] = cost;
        graph[b][a] = cost; // Undirected graph
    }

    char srcName[1000];
    // fprintf(output_file, "Enter source node: ");
    fscanf(input_file, "%s", srcName);
    fprintf(output_file," \nthe source is : ");
    fprintf(output_file,"%s",&srcName);
    fprintf(output_file,"\n");
    int src = getNodeIndex(srcName);

    dijkstra(src, nodeCount);
    printShortestPaths(src, nodeCount,output_file);
    fclose(input_file);
    fclose(output_file);
    return 0;
}
