#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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
    point start;
    point end;
    //int pathWidth;
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

void initMaze(maze* m , int width, int height){
    m->mazeWidth = width;
    m->mazeHeight = height;

    //implements stack
    m->stackTop = -1;
    m->stack = (point*)malloc(width * height * sizeof(point));

    //allocate mem and set maze elements to 0
    m->maze = (int*)calloc(width * height,sizeof(int));

    m->visitedNodes = 0;

    if (!m->stack || !m->maze) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    m->start = (point){0,0};
    m->end = (point){width -1, height-1};
}

void drawMaze(maze *m, FILE *output) {
    // Draw top border with opening for start
    fprintf(output, " "); // Left edge
    for (int x = 0; x < m->mazeWidth; x++) {
        int cell = m->maze[0 * m->mazeWidth + x]; // First row cells
        if (x == m->start.x && (cell & CELL_PATH_N)) {
            fprintf(output, "  "); // Start entrance (no wall)
        } else if (cell & CELL_PATH_N) {
            fprintf(output, "  "); // Open north path
        } else {
            fprintf(output, "_ "); // Closed north path
        }
    }
    fprintf(output, "\n");

     // Draw maze cells
    for (int y = 0; y < m->mazeHeight; y++) {
        fprintf(output, "|"); // Left wall
        for (int x = 0; x < m->mazeWidth; x++) {
            int cell = m->maze[y * m->mazeWidth + x];
            
            // Draw south wall (underscore if closed)
            fprintf(output, "%c", (cell & CELL_PATH_S) ? ' ' : '_');
            
            // Draw east wall (pipe if closed)
            fprintf(output, "%c", (cell & CELL_PATH_E) ? ' ' : '|');
        }
        fprintf(output, "\n");
    }
}

void genMaze(maze *m){

    int directions[4][2] = {
        {0,-1}, //  North
        {1,0},  //  East
        {0,1},  //  South
        {-1,0}  //  West
    };

    //if visited less than all the nodes...
    if(m->visitedNodes < m->mazeWidth * m->mazeHeight){

        int validDirections[4];
        int numValidDirections = 0;

        //get the top(current cell) we are working with
        point currentCell = top(m);
        
        //loop for each direction avialable 
        for(int i = 0; i < 4 ; i++){
            int newX = currentCell.x + directions[i][0];
            int newY = currentCell.y + directions[i][1];  

            //if the X & Y are in bounds            (might have to be <= width / size)
            if (newX >= 0 && newX < m->mazeWidth && newY >= 0 && newY < m->mazeHeight) {
                
                //get index of the cell & if not visited
                int cellIndex = newY * m->mazeWidth + newX;
                if(!(m->maze[cellIndex] & CELL_VISITED)){
                    validDirections[numValidDirections++] = i;
                }

            }
        }


        //if there are empty neighbors available
        if(numValidDirections > 0){
            int cellDirection = validDirections[rand() % numValidDirections];  

            // get new coordinates
            int newX = currentCell.x + directions[cellDirection][0];
            int newY = currentCell.y + directions[cellDirection][1];
            
            // mark the path
            int currentCellIndex = currentCell.y * m->mazeWidth + currentCell.x;
            int newCellIndex = newY * m->mazeWidth + newX;

            switch(cellDirection){
                case 0: //North
                    m->maze[currentCellIndex] |= CELL_PATH_N; 
                    m->maze[newCellIndex] |= CELL_PATH_S;
                    break;
                case 1: //East
                    m->maze[currentCellIndex] |= CELL_PATH_E;
                    m->maze[newCellIndex] |= CELL_PATH_W;
                    break;
                case 2: //South
                    m->maze[currentCellIndex] |= CELL_PATH_S;
                    m->maze[newCellIndex] |= CELL_PATH_N;
                    break;
                case 3: //West
                    m->maze[currentCellIndex] |= CELL_PATH_W;
                    m->maze[newCellIndex] |= CELL_PATH_E;
                    break;

            }

            m->maze[newCellIndex] |= CELL_VISITED;
            push(m,newX,newY);
            m->visitedNodes++;

        }
        else{
            pop(m);
        }
    }
}

void genWholeMaze(maze *m){
    //add start and end points at Top left and Bottom right
    m->start = (point){0,0};
    m->end = (point){m->mazeWidth -1, m->mazeHeight-1};

    int startX = rand() % m->mazeWidth;
    int startY = rand() % m->mazeHeight;
    
    m->maze[startY * m->mazeWidth + startX] |= CELL_VISITED;
    push(m,startX,startY);
    m->visitedNodes = 1;

    while (m->visitedNodes < m->mazeWidth * m->mazeHeight) {
        genMaze(m);
    }

    // Force open paths for start/end
    int startIdx = m->start.y * m->mazeWidth + m->start.x;
    int endIdx = m->end.y * m->mazeWidth + m->end.x;

    // Open north path for start (top entrance)
    m->maze[startIdx] |= CELL_PATH_N;
    // Open south path for end (bottom exit)
    m->maze[endIdx] |= CELL_PATH_S;

}

int main(){

    srand(time(NULL));

    int maze_height,maze_width;

    //gets the length and width of the maze from the user
    //gets the length and width of the maze from the user
    printf("Enter maze height and width (e.g., 10 20): ");
    scanf("%d %d", &maze_height, &maze_width);
    if (maze_width < 1 || maze_width > 100 || maze_height < 1 || maze_height > 100) {
        printf("Invalid dimensions. Using default 10x20 maze.\n");
        maze_height = 10;
        maze_width = 20;
    }

    //create and initilize the maze
    maze myMaze;
    initMaze(&myMaze,maze_width, maze_height);

    genWholeMaze(&myMaze);
    //print maze to console
    drawMaze(&myMaze, stdout);

    // save maze to file
    FILE *file = fopen("maze.txt", "w");
    if (file) {
        drawMaze(&myMaze, file); // Write to file
        fclose(file);
        printf("\nMaze saved to maze.txt\n");
    } else {
        printf("Failed to save maze to file.\n");
    }


    freeMaze(&myMaze);

    return 0;
}