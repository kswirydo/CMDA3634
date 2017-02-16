/* written by Michael Brennan, extended by Kasia Swirydowicz */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printBoard(int, int, int* );
void updateBoard(int, int, int*, int*);

int main(int argc, char **argv){
  int N; // width of board
  int M; // height of boar
  // Game Parameters are read from the file
  // argv[0] is the name of the executable
  // argv[1] is the name of the file we are running
  // compile: gcc main.c -o gameOfLife
  // run: ./gameOfLife inputFile.txt
  
  // open file for reading
  FILE *fp = fopen(argv[1], "r");
  
  if(fp==NULL){
    printf("Game Of Life: could not load input file %s\n", argv[1]);
    exit(0);
  }
  // keep reading the file until you find $Size
  char buf[BUFSIZ];
  do{
    fgets(buf, BUFSIZ, fp);
  }while(!strstr(buf, "$Size"));
  
  // read the size
  fgets(buf, BUFSIZ, fp);
  sscanf(buf, "%d %d", &N, &M);
  
  // Initialize board
  int *boardA = (int*) calloc((N+2)*(M+2), sizeof(int));
  int *boardB = (int*) calloc((N+2)*(M+2), sizeof(int));
  printf("board initialized, board size: %d by %d \n", N,M);
  
  //read number of updates
  int T;
  
  do{
    fgets(buf, BUFSIZ, fp);
  }while(!strstr(buf, "$Updates"));
  fgets(buf, BUFSIZ, fp);
  sscanf(buf, "%d", &T);
  
  printf("number of updates: %d\n", T);
  int numAlive;
  // next, scan for how many alive cells you have
  do{
    fgets(buf, BUFSIZ, fp);
  }while(!strstr(buf, "$Alive"));
  
  // read the number of alive cells
  fgets(buf, BUFSIZ, fp);
  sscanf(buf, "%d", &numAlive);
  printf("initial number of alive cells:  %d \n", numAlive);
  
  //allocate the alive list (one list per every dimension
  int * LiveList_i = (int*) calloc(numAlive, sizeof(int));
  int * LiveList_j = (int*) calloc(numAlive, sizeof(int));
  
  for (int i=0; i<numAlive; i++){
    fgets(buf, BUFSIZ, fp);
    sscanf(buf, "%d %d", &LiveList_i[i], &LiveList_j[i]);
  }
  
  fclose(fp);
  // Spawn Cells
  for(int n = 0; n<numAlive; ++n){
    int i = LiveList_i[n]; int j = LiveList_j[n];
    boardA[j+i*(N+2)] = 1;
  }
  free(LiveList_i);
  free(LiveList_j);
  
  // Print Initial Board
  printf("Initial Condition\n");
  printBoard(N,M,boardA);
  int t = 0;
  // Start Game
  while(t<T){
    
    ++t;
    updateBoard(N,M,boardA,boardB);
    printf("After %d iterations\n",t);
    printBoard(N,M,boardB);
    
    if(t==T) break;
    
    ++t;
    updateBoard(N,M,boardB,boardA);
    printf("After %d iterations\n",t);
    printBoard(N,M,boardA);
  }
  
  // Finish
  free(boardA);
  free(boardB);
  return(0);
}

void updateBoard(int N, int M,  int* oldBoard, int* newBoard){
  // Update the board
  for(int i=1;i<M+1;++i){  //starting at 1 to skip boundary layer
    for(int j=1;j<N+1;++j){ //starting at 1 to skip boundary layer
      
      // Make useful indices
      int cell = j+i*(N+2); // Current cell
      int cellBelow = j+(i-1)*(N+2); // Cell below it
      int cellAbove = j+(i+1)*(N+2); // Cell above it
      
      // Split the sum into the 3 above, 3 below, and 2 level neighbors
      int sumBelow = oldBoard[cellBelow]+oldBoard[cellBelow-1]+oldBoard[cellBelow+1];
      int sumLevel = oldBoard[cell-1]+oldBoard[cell+1];
      int sumAbove = oldBoard[cellAbove]+oldBoard[cellAbove-1]+oldBoard[cellAbove+1];
      
      // Compute the sum
      int sum = sumBelow + sumLevel + sumAbove;
      
      // Get the current state of the cell
      int oldState = oldBoard[cell];
      
      // Change the state of the cell
      int newState;
      
      // If the cell was alive
      if(oldState == 1){
        
        // Exactly 2 or 3 neighbors
        if(sum == 2||sum == 3){
          newState = 1;
        }
        
        // More then 3 or less then 2 neighbors
        else{
          newState = 0;
        }
      }
      
      // If the cell was dead
      else{
        
        // Exactly 3 neighbors
        if(sum == 3){
          newState = 1;
        }
        
        
        else{
          newState = 0;
        }
      }
      
      // Update new board
      newBoard[cell] = newState;
    }
  }
  
}



void printBoard(int N, int M, int* board){
  
  //Formatted to start in bottom left corner, moving across each row
  for(int i=M;i>0;--i){
    for(int j=1;j<N+1;++j){ //starting at 1 to skip boundary layer
      
      // Cell number and state
      int cell = j + i*(N+2);
      int state = board[cell];
      
      if(state == 1){
        printf("X ");
      }
      
      else{
        printf("0 ");
      }
      
    }
    printf("\n");
  }
}
