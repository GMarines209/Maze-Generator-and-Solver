#include <stdio.h>
#include <stdlib.h>


#define PATHWIDTH 1

#define CELL_PATH_N  0x01  // North path open
#define CELL_PATH_E  0x02  // East path open
#define CELL_PATH_S  0x04  // South path open
#define CELL_PATH_W  0x08  // West path open
#define CELL_VISITED 0x10  // Cell has been visited
//bit is left shifted over and compared in order to rep multiple bool values in a single number


typedef struct point{
    int x;
    int y;
}point;

typedef struct maze{
    int mazeWidth;
    int mazeHeight;
    int stackTop;   
    point *stack;
    int* maze;
    int visitedNodes;
    int pathWidth;
}maze;

// Push a point to the stack
void push(maze *m, int x, int y) {
    // increase top and add point to stack
    m->stack[++(m->stackTop)] = (point){x, y};
}

// Pop a point from the stack
point pop(maze *m) {
    // Return and remove top point
    return m->stack[(m->stackTop)--];
}

point top(maze *m) {
    return m->stack[m->stackTop];
}

// Free allocated memory
void freeMaze(maze *m) {
    free(m->maze);
    free(m->stack);
}

void initMaze(maze* m , int width, int height,int pathWidth){
    m->mazeWidth = width;
    m->mazeHeight = height;

    //implements stack
    m->stackTop = -1;
    m->stack = (point*)malloc(width * height * sizeof(point));

    //allocate mem and set maze elements to 0
    m->maze = (int*)calloc(width * height,sizeof(int));

    m->visitedNodes = 0;
    m->pathWidth = PATHWIDTH;
}

void drawMaze(maze *m) {
    //system("cls");
    //loop over column
    for (int y = 0; y < m->mazeHeight; y++) {
        //loop over row
        for (int x = 0; x < m->mazeWidth; x++) {
            //A visited cell is a space, while unvisited is a # (wall)
            printf("%c", (m->maze[y * m->mazeWidth + x] & CELL_VISITED) ? ' ' : '#');
        }
        printf("\n");
    }
}

void genMaze(maze *m){
    //if visited less than all the nodes...
    if(m->visitedNodes < m->mazeWidth * m->mazeHeight){
        
        //Looking North

        //if the node has one above it 
        if(top(&m).y > 0){
            
            if(m->maze[(top(&m).y -1) * m->mazeWidth + (top(&m).x + 0)] & CELL_VISITED){

            }
        }


    }
}

int main(){

    int maze_height,maze_width;

    //gets the length and width of the maze from the user
    printf("How large do you want the maze to be? \n\t (e.g., Length: 10 Width: 20) \n");
    if (scanf("%d %d", &maze_height, &maze_width) != 2) {
        printf("Invalid input. Using default 10x20 maze.\n");
        maze_height = 10;
        maze_width = 20;
    }

    //create and initilize the maze
    maze myMaze;
    initMaze(&myMaze,maze_width, maze_height, PATHWIDTH);

    drawMaze(&myMaze);

    return 0;
}