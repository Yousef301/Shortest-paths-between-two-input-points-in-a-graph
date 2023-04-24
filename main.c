//By Yousef Shamasneh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX 296
#define INF 9999

typedef struct data {
    int distance;
    char city[30];
} data;

typedef struct dataHolder {
    int src;
    int dest;
    int dist;
    char path[600];
    struct dataHolder *next;
} dataHolder;

dataHolder *info;

// Save the data between two entered vertices {path, distance...}
void addInfo(int src, int dest, int distance, char *path) {
    dataHolder *temp = (dataHolder *) malloc(sizeof(dataHolder));
    dataHolder *ptr = info;

    if (temp == NULL) {
        printf("Out of space!!");
        return;
    }
    temp->src = src;
    temp->dist = distance;
    temp->dest = dest;
    strcpy(temp->path, path);

    temp->next = NULL;

    if (info == NULL) {
        info = temp;
        return;
    }
    while (ptr->next != NULL)
        ptr = ptr->next;
    ptr->next = temp;
}

// Create a graph.
data graph[MAX][MAX];

// Load the graph with the data that is in the file.
void BuildGraph(const char *line) {
    char v1[4], v2[4], city[30], distance[4];
    int l_cnt = 0, c_cnt = 0;
    while (line[l_cnt] != '\n') {
        while (!isspace(line[l_cnt])) {
            v1[c_cnt] = line[l_cnt];
            c_cnt++;
            l_cnt++;
        }
        l_cnt++;
        v1[c_cnt] = '\0';
        c_cnt = 0;
        while (!isspace(line[l_cnt])) {
            v2[c_cnt] = line[l_cnt];
            c_cnt++;
            l_cnt++;
        }
        l_cnt++;
        v2[c_cnt] = '\0';
        c_cnt = 0;
        while (!isspace(line[l_cnt])) {
            city[c_cnt] = line[l_cnt];
            c_cnt++;
            l_cnt++;
            if (isspace(line[l_cnt]) && isalpha(line[l_cnt + 1])) {
                city[c_cnt] = line[l_cnt];
                c_cnt++;
                l_cnt++;
            }
        }
        l_cnt++;
        city[c_cnt] = '\0';
        c_cnt = 0;
        while (line[l_cnt] != '\n') {
            distance[c_cnt] = line[l_cnt];
            c_cnt++;
            l_cnt++;
        }
        distance[c_cnt] = '\0';
        c_cnt = 0;
    }
    int vert1, vert2, dist;
    vert1 = atoi(v1);
    vert2 = atoi(v2);
    dist = atoi(distance);
    graph[vert1][vert2].distance = dist;
    graph[vert2][vert1].distance = dist;
    strcpy(graph[vert1][vert2].city, city);
    strcpy(graph[vert2][vert1].city, city);
}

void dijkstra(int n, int src) {
    //This 2d matrix shows the cost of going from i to j, but if there is no edge between them then the cost is INF
    int cost[MAX][MAX];
    //Visited array is used to check if the path is already visited while finding the shortest path
    int distances[MAX], visited[MAX], pred[MAX];
    int i, j, count, minDistance, nextN;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (graph[i][j].distance == 0) {
                cost[i][j] = INF;
            } else
                cost[i][j] = graph[i][j].distance;
        }
    }

    for (i = 0; i < n; i++) {
        distances[i] = cost[src][i];
        pred[i] = src;
        visited[i] = 0;
    }
    // Make the source vertex distance equals to 1 and visited[src] = 1
    distances[src] = 0;
    visited[src] = 1;
    count = 1;

    while (count < n - 1) {
        minDistance = INF;
        // Finding short path.
        for (i = 0; i < n; i++) {
            if (distances[i] < minDistance && !visited[i]) {
                minDistance = distances[i];
                nextN = i;
            }
        }
        visited[nextN] = 1;
        // Checking if there is a better path.
        for (i = 0; i < n; i++) {
            if (!visited[i]) {
                if ((minDistance + cost[nextN][i]) < distances[i]) {
                    distances[i] = minDistance + cost[nextN][i];
                    pred[i] = nextN;
                }
            }
        }
        count++;
    }
    char path[600];
    path[0] = '\0';
    int distance, destination, source;
    int indexes[60], i_cnt = 0;
    for (int k = 0; k < 59; k++) {
        indexes[k] = 0;
    }
    printf("Enter the destination:");
    scanf("%d", &i);
    if (i > MAX || i < 1) {
        printf("Destination can't be zero, negative or bigger than %d ^_^\n\n", (MAX - 1));
        return;
    }
    source = src;
    destination = i;
    //Printing the path from source vertex to the destination and the shortest distance
    if (i != src) {
        if (distances[i] == INF) {
            printf("Sorry, but there is no path from %d to %d\n\n", i, src);
            return;
        }
        printf("The shortest path from %d to %d is %d\nPath: ", src, i, distances[i]);
        distance = distances[i];
        j = i;
        while (j != src) {
            j = pred[j];
            indexes[i_cnt] = j;
            i_cnt++;
        }
        for (int l = i_cnt - 2; l >= 0; l--) {
            setbuf(stdout, 0);
            printf(" %s --> ", graph[indexes[l]][pred[indexes[l]]].city);
            strcat(path, graph[indexes[l]][pred[indexes[l]]].city);
            strcat(path, " --> ");
        }
        printf(" %s", graph[i][pred[i]].city);
        strcat(path, graph[i][pred[i]].city);
        printf("\n");
        addInfo(source, destination, distance, path);
        path[0] = '\0';
        distance = destination = source = 0;
    } else
        printf("The distance from start point to itself (%d --> %d) is 0\n", src, i);
    printf("\n");
}

void Menu() {
    printf("^^Menu^^\n");
    printf("(1) Read the file segments.txt and load the data\n(2) Enter 2 points to compute the shortest path between them\n"
           "(3) Print the route of the shortest distance to a file called 'route.txt'\n(4) Exit\n");
}

void printInFile(FILE *file) {
    struct dataHolder *temp = info;
    while (temp != NULL) {
        fprintf(file, "The shortest path from %d to %d is %d\n", temp->src, temp->dest, temp->dist);
        fprintf(file, "Path: %s\n\n", temp->path);
        temp = temp->next;
    }
}

int main() {
    bool created = false, not_saved = true, noEdges = true;\
    char save;
    info = NULL;
    int source, cases;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            graph[i][j].distance = 0;
        }
    }
    char line[100];
    FILE *data, *write;
    data = fopen("segments.txt", "r");
    write = fopen("route.txt", "w");
    if (data == NULL) {
        printf("File is not exist!!\n");
    }
    while (1) {
        Menu();
        printf("Enter your choice:");
        scanf("%d", &cases);
        fflush(stdin);
        switch (cases) {
            case 1:
                while (fgets(line, 100, data)) {
                    BuildGraph(line);
                }
                printf("Graph created successfully\n\n");
                fclose(data);
                created = true;
                break;
            case 2:
                if (created) {
                    printf("Enter source point:");
                    scanf("%d", &source);
                    for (int i = 0; i < MAX; i++) {
                        if (graph[source][i].distance != 0)
                            noEdges = false;
                    }
                    if (source > MAX || source < 1) {
                        printf("Please check the value of start point!!\n\n");
                    } else if (noEdges == true) {
                        printf("vertx %d has no edges\n\n", source);
                    } else
                        dijkstra(MAX, source);
                } else
                    printf("Make sure you created the graph first ^_^\n\n");
                break;
            case 3:
                printInFile(write);
                printf("Data has been written successfully\n\n");
                not_saved = false;
                break;
            case 4:
                if (not_saved) {
                    printf("Data is not saved yet, do you want save before exit (Y/N)?");
                    scanf("%c", &save);
                    if (save == 'y' || save == 'Y') {
                        printInFile(write);
                        printf("Data has been written successfully\n");
                        printf("GoodBye (^_^)\n");
                        exit(1);
                    } else if (save == 'n' || save == 'N') {
                        printf("GoodBye (^_^)\n");
                        exit(1);
                    } else {
                        printf("\n");
                        break;
                    }
                } else {
                    printf("GoodBye (^_^)\n");
                    exit(1);
                }
            default:
                printf("Unexpected value..\n\n");
        }
    }
}
