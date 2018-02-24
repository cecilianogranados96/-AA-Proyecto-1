#include <stdio.h>

int file_2_sudoku(int *sudoku,char *filename);
int file_checker(int *sudoku, int file);
int column_checker(int *sudoku, int column);
int box_checker(int *sudoku, int box);
int check_all(int *sudoku);
int solve_sudoku(int *problem);
int print_sudoku(int *sudoku);

int main(int argc, char *argv[])
{
  int sudoku[81];
  int *ptr;
  int i;
  i=0;

    
    
    
  file_2_sudoku(sudoku,"eje.txt");

    
    

  print_sudoku(sudoku);
 
  solve_sudoku(sudoku);
if(!(solve_sudoku(sudoku))){
     printf("PASO 1");
}else{
     printf("PASO 2");
}
   

    
    
  print_sudoku(sudoku);
  return 0;
}



int file_2_sudoku(int *sudoku, char *filename)
{
  FILE *fp;
  int c;
  int i;
  int error;
  error=0;
  i=0;
  fp=fopen(filename,"r");
  if (fp==0)
    return 1;
  // Convert the file to an array
  while ((c!=EOF))
    {
      c=fgetc(fp);
      // ASCII to int conversion
      if ((((c-48)<0) || ((c-48)>9))&& (c!=EOF))
	error=1;
      *sudoku++=(c-48);
      i++;
    }
  if ((i!=82) || (error=1))
    return 1;
  else
    return 0;
}




int file_checker(int *sudoku, int file)
{
  int number;
  int i;
  int j;
  i=0;
  for (i=0;i<9;i++)
    {
      number=*(sudoku+9*file+i);
      if (number!=0)
	{
	  for (j=0;j<9;j++)
	    {
	      if ((i!=j)&&(number==*(sudoku+9*file+j)))
		return 1;// Error. Duplicate found
	    }
	}
    }
  // If we are here, there is no error
  return 0;
}



/*
 *  Function: column checker                                      
 *  Checks the given column of the sudoku looking for repetitions
 *  Imput arguments: 
 *   int *sudoku : pointer where the sudoku is stored
 *   int column : column that we want to check
 *  Return:
 *   1 if there is an error
 *   0 otherwise
 */
int column_checker(int *sudoku, int column)
{
  int number;
  int i;
  int j;
  i=0;
  for (i=0;i<9;i++)
    {
      number=*(sudoku+9*i+column);
      if (number!=0)
	{
	  for (j=0;j<9;j++)
	    {
	      if ((i!=j)&&(number==*(sudoku+9*j+column)))
		return 1;// Error. Duplicate found
	    }
	}
    }
  // If we are here, there is no error
  return 0;
}


/*
 *  Function: box checker                                      
 *  Checks the given box of the sudoku looking for repetitions
 *  The first box has the number '1' and the last, '9'. From left to right
 *  and top to down
 *  Imput arguments: 
 *   int *sudoku : pointer where the sudoku is stored
 *   int box : box that we want to check
 *  Return:
 *   1 if there is an error
 *   0 otherwise
 */
int box_checker(int *sudoku, int box)
{
  int filemult;
  int columnmult;

  int i;
  int j;
  
  int buffer[9];

  int cont=0;

  int number;

  switch(box)
    {
    case 1 :
      filemult=0;
      columnmult=0;
      break;
    case 2 :
      filemult=0;
      columnmult=1;
      break;
    case 3 :
      filemult=0;
      columnmult=2;
      break;
    case 4 :
      filemult=1;
      columnmult=0;
      break;
    case 5 :
      filemult=1;
      columnmult=1;
      break;
    case 6 :
      filemult=1;
      columnmult=2;
      break;
    case 7 :
      filemult=2;
      columnmult=0;
      break;
    case 8 :
      filemult=2;
      columnmult=1;
      break;
    case 9 :
      filemult=2;
      columnmult=2;
      break;
    default :
      return 1;
    }

  // Store the 'box' in a buffer to make duplicate checking easier
  for(i=0;i<3;i++)
    {
      for(j=0;j<3;j++)
	{
	  buffer[cont]=*(sudoku+(i+3*filemult)*9+j+3*columnmult);
	  cont++;
	}
    }
  // And now check the buffer
  for (i=0;i<9;i++)
    {
      number=*(buffer+i);
      if (number!=0)
	{
	  for (j=0;j<9;j++)
	    {
	      if ((i!=j)&&(number==*(buffer+j)))
		return 1;// Error. Duplicate found
	    }
	}
    }
  // If we are here, there is no error
  return 0;
}

int check_all(int *sudoku)
{
  int i;
  // Check Sudoku
  for (i=0;i<9;i++)
    {
      if((file_checker(sudoku,i)))
	return 1;
    }
  for (i=0;i<9;i++)
    {
      if((column_checker(sudoku,i)))
	return 1;
    }
  for (i=0;i<9;i++)
    {
      if(box_checker(sudoku,i+1))
	return 1;
    }
  return 0;
}

/*
 *  Function: solve sudoku
 *  This is the 'heart'  of the program. solve_sudoku is a recursive function
 *  which solves the given sudoku.
 *  Imput arguments: 
 *   int *problem : pointer where the sudoku is stored
 *  Return:
 *   1 if there is an error
 *   0 otherwise
 */
int solve_sudoku(int *problem)
{
  int *ptr;
  int i=0;
  ptr=problem;
  int solve=1;
  // Looking for first non-zero (i.e. blank) character
  while((*ptr!=0))
    {
      ptr++;
      i++;
      //If we are in the last cell, return Ok!
      if (i==82)
	{
	  return 0;
	}
    }
  // We write the first valid number in that position
  (*ptr)=1;
  while(check_all(problem)!=0)
    {
      (*ptr)++;
      if ((*ptr)==10)
	{
	  // If there is not a valid number, we write a 0 and return an error
	  (*ptr)=0;
	  return 1;
	}
    }
  // If there is a number, we call the function solve_sudoku with the new sudoku.
  solve=solve_sudoku(problem);  
  // If we are returned a '1' (problem!) we increment the cell and call the function again.
  // If we can't increment the cell more, we write a 0 and return an error.
  while(solve==1)
    {

      (*ptr)++;

      if ((*ptr)==10)
	{
	  (*ptr)=0;
	  return 1;
	}
      solve=solve_sudoku(problem);
    }
  
  // If we reach this instruction, all is good.
  return 0;
}

int print_sudoku(int *sudoku)
{
  int i;
  int *ptr;
  ptr=sudoku;

  //First three lines
  for (i=0;i<19;i++)
    printf("-");
  printf("\n");


  for (i=0;i<3;i++)
    {
      printf("|");
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|",*ptr++);
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|",*ptr++);
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|\n",*ptr++);
    }
  for (i=0;i<6;i++)
    printf("-");
  printf("+");
  for (i=0;i<5;i++)
    printf("-");
  printf("+");
  for (i=0;i<6;i++)
    printf("-");
  printf("\n");
  //Second three lines
  for (i=0;i<3;i++)
    {
      printf("|");
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|",*ptr++);
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|",*ptr++);
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|\n",*ptr++);
    }
  for (i=0;i<6;i++)
    printf("-");
  printf("+");
  for (i=0;i<5;i++)
    printf("-");
  printf("+");
  for (i=0;i<6;i++)
    printf("-");
  printf("\n");
  //Third three lines
  for (i=0;i<3;i++)
    {
      printf("|");
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|",*ptr++);
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|",*ptr++);
      printf("%i ",*ptr++);      
      printf("%i ",*ptr++);
      printf("%i|\n",*ptr++);
    }
  for (i=0;i<6;i++)
    printf("-");
  printf("+");
  for (i=0;i<5;i++)
    printf("-");
  printf("+");
  for (i=0;i<6;i++)
    printf("-");
  printf("\n");
    return 1;
}