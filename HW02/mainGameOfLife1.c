#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printBoard(int, int, int* );
void updateBoard(int, int, int*, int*);

int main(){

  // Game Parameters
  int N = 10; // width of board
  int M = 10; // height of board
  int T = 11; // number of iterations

  // Initialize board
  int *boardA = (int*) calloc((N+2)*(M+2), sizeof(int));
  int *boardB = (int*) calloc((N+2)*(M+2), sizeof(int));

  // For glider, starting bottom left
  int LiveList_i[] = {1,2,3,3,3}; // Uses 1 indexing to account for boundary layer
  int LiveList_j[] = {2,3,1,2,3}; 

  // Spawn Cells 
  for(int n = 0; n<5; ++n){
    int i = LiveList_i[n]; int j = LiveList_j[n];
    boardA[j+i*(N+2)] = 1;
  }

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
