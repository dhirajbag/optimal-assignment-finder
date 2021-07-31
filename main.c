/*
____________________________________________________________________________________
     Student:
        Name: DHIRAJ BAG
        Roll: 001911001033
        B.E. in Information Technology
        2nd Year, 2nd Semester
        Jadavpur University (SaltLake Campus)
        Email: dhirajbag.db@gmail.com
        Date: 19th Apr, 2021
    
    Assignment:
        Title: Assignments for S/W Engg Lab

        Problem 2.
         => solution: Program will find optimal solution for an assignment
         problem.
            - Handles unbalanced problem
            - Handles Maximization problem
            - Handles restricted (forbidden assignment) problem
            - Draws visual repressentation of the lines used to cover zeros.

        Assumptions:
            - All the cost are non negetive integers

    Compiling and running:
        To compile use:
            gcc R33_P2.c -o main2
        
        To run use:
            ./main2

        Further instructions will be provided at runtime.
____________________________________________________________________________________
*/



#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define min(x, y) ((x < y) ? x : y)

/* node : used to represent a cell's position with row_index and col_index */
typedef struct node
{
    int row;
    int col;
} node;

/*
Helper Functions implemented:
    - void subtractMinFromRows(cost, num_rows, num_cols) : subtracts from all the elements of each row of cost matrix the value of minimum element in corresponding row

    - void subtractMinFromCols(cost, num_rows, num_cols) : does the same as above, but for all the columns of cost matrix

    - int findMinUncovered(cost, m, n, isLinedRow, isLinedCol) : After all the zeros in cost matrix are covered(lined) by minimum number of lines, 
        this function return the  minimum among the uncovered cells in cost matrix
    
    - node findOnlyNondiscardedZero(cost, ..., isDiscardedRow, isDiscardedCol) : Whenever a cell is allocated, corresponding row and column is discarded,
        this function returns the location node of a nondiscarded zero cell that is the only nondiscarded zero cell in the specified row or column.
    
    - void printLines(isLinedRow, isLinedCol, ...) : Helps visualize the lines drawn to cover the zeros in cost matrix

    - void makeNegetive(cost, ...) : multiplies -1 with all the elements of the cost matrix, for handling maximization problems


Some other helper functions impelemented:

    - int* allocate_row(int n) : allocates an array of n integers all set to zero

    - int** allocate_matrix_int(int num_rows, int num_cols) : allocates a matrix of integer elements all set to 0

    - void deallocate_matrix_int(int **matrix, int num_rows, int num_cols) : deallocates the integer matrix pointed by 'matrix'

    - void input_row_int(int *row, int n) : takes n integers as input (in the form of a row) and stores them in 'row'

    - void input_matrix_int(int **matrix, int num_rows, int num_cols) : takes num_rows*num_cols number of integers as input
        (in the form of a matrix) and stores them in 'matrix'

    - void set_all_to(int* row, int n, int value) : sets values of all the n elements in the array 'row' to 'value'

    - copy_matrix_int(int **src, int **dest, ...) : copies elements from src matrix to dest matrix

*/

/* Prototypes */
void subtractMinFromRows(int **cost, int num_rows, int num_cols);
void subtractMinFromCols(int **cost, int num_rows, int num_cols);
int findMinUncovered(int **cost, int num_rows, int num_cols, int *isLinedRow, int *isLinedCol);
node findOnlyNondiscardedZero(int **cost, int r1, int r2, int c1, int c2, int *isDiscardedRow, int *isDiscardedCol);
void printLines(int *isLinedRow, int *isLinedCol, int num_rows, int num_cols);
void makeNegetive(int **cost, int num_rows, int num_cols);

int *allocate_row(int n);
int **allocate_matrix_int(int num_rows, int num_cols);
void deallocate_matrix_int(int **matrix, int num_rows, int num_cols);
void input_row_int(int *row, int n);
void input_matrix_int(int **matrix, int num_rows, int num_cols);
void printMatrix(int **A, int r1, int r2, int c1, int c2);
void set_all_to(int *arr, int n, int val);
void copy_matrix_int(int **src, int **dest, int r1, int r2, int c1, int c2);
/*__________________________________*/

int main()
{
    printf("Assignment Problem Solver: (- Dhiraj Bag, Roll: 001911001033) \n");
    printf("\nEach row represents a candidate, ie. num(candidates)=num(rows)\n");
    printf("Each column represents a job, ie. num(jobs)=num(columns)\n");

    int num_rows, num_cols;
    printf("\nEnter num(Rows) <space> num(Columns): ");
    scanf("%d %d", &num_rows, &num_cols);

    int **cost = allocate_matrix_int(num_rows, num_cols);

    printf("\nEnter Cost matrix(%dx%d) <in rows and columns> :\n", num_rows, num_cols);

    input_matrix_int(cost, num_rows, num_cols);

    int original_num_rows = num_rows;
    int original_num_cols = num_cols;

    int **originalCost = allocate_matrix_int(num_rows, num_cols);
    copy_matrix_int(cost, originalCost, 0, num_rows - 1, 0, num_cols - 1);

    int option;

    printf("\nEnter corresponding option number: \n\t1.Minimize Assignment Cost\n\t2.Maximize assignment cost(ie profit)\n\t: ");
    scanf("%d", &option);

    char isMaximization = (option == 2) ? 1 : 0;

    if (isMaximization)
        makeNegetive(cost, num_rows, num_cols);

    char isUnbalanced = 0;

    if (num_rows != num_cols)
    {
        isUnbalanced = 1;
        if (num_rows < num_cols)
        { /*add num_cols - num_rows number of rows */
            int **tmp = allocate_matrix_int(num_cols, num_cols);
            copy_matrix_int(cost, tmp, 0, num_rows - 1, 0, num_cols - 1);
            deallocate_matrix_int(cost, num_rows, num_cols);
            cost = tmp;
            num_rows = num_cols;
        }
        else
        { /*add num_rows - num_cols number of columns */
            int **tmp = allocate_matrix_int(num_rows, num_rows);
            copy_matrix_int(cost, tmp, 0, num_rows - 1, 0, num_cols - 1);
            deallocate_matrix_int(cost, num_rows, num_cols);
            cost = tmp;
            num_cols = num_rows;
        }
    }

    printf("\nEnter corresponding option number: \n\t1. Un-restricted assignment\n\t2. Restricted assignment\n\t: ");
    scanf("%d", &option);

    char isRestricted = (option == 2) ? 1 : 0;
    int numRestrictions = 0;

    if (isRestricted)
    {
        puts("Restricted problem: a candidate(row) cannot be assigned to a particular job(column) :\n");
        printf("Enter the number of restrictions: ");
        scanf("%d", &numRestrictions);

        printf("Enter the %d restricted row_number <space> column_number in %d lines(numbering starts from 1) : \n", numRestrictions, numRestrictions);
        int tmp = numRestrictions;
        while (tmp--)
        {
            int row, col;
            scanf("%d %d", &row, &col);
            row--;
            col--;
            cost[row][col] = INT_MAX >> 1;  /* Assigning a very big number as the cost, to handle restriction
                                                - original cost was already preserved */
        }
    }

    subtractMinFromRows(cost, num_rows, num_cols);
    subtractMinFromCols(cost, num_rows, num_cols);

    while (1)
    {

        puts("\nDrawing lines on :");
        printMatrix(cost, 0, num_rows - 1, 0, num_cols - 1);

        int i, j;

        /* Opimizing : Drawing min num of lines */
        int isLinedRow[num_rows]; // = allocate_row(num_rows);
        int isLinedCol[num_cols]; // = allocate_row(num_cols);
        set_all_to(isLinedRow, num_rows, 0);
        set_all_to(isLinedCol, num_cols, 0);
        int numLines = 0;

        /* Method for drawing min number of lines to cover all zeros:
            - assign as many zeros as possible from the rows (one zero per row)
                -discard corresponding row and column

            - Mark all rows having no assignment

            repeat:
                -mark all columns having zero in newly marked row(s)
                -mark all rows haing assignments in newly marked columns
                -repeat until no new marked row / col

            Draw lines:
                -draw lines through all marked cols and UNMARKED rows
        */

        int isDiscardedRow[num_rows]; // = allocate_row(num_rows);
        int isDiscardedCol[num_cols]; // = allocate_row(num_cols);
        set_all_to(isDiscardedRow, num_rows, 0);
        set_all_to(isDiscardedCol, num_cols, 0);

        int **isAssigned = allocate_matrix_int(num_rows, num_cols);

        for (i = 0; i < num_rows; i++)
        {
            for (j = 0; j < num_cols; j++)
            {
                if (cost[i][j] == 0 && !isDiscardedCol[j])
                { /* Assign around it */
                    isDiscardedRow[i] = 1;
                    isDiscardedCol[j] = 1;
                    isAssigned[i][j] = 1;
                    break;
                }
            }
        }

        int isMarkedRow[num_rows]; // = allocate_row(num_rows);
        int isMarkedCol[num_cols]; // = allocate_row(num_cols);
        set_all_to(isMarkedRow, num_rows, 0);
        set_all_to(isMarkedCol, num_cols, 0);

        int isNewlyMarkedRow[num_rows]; // = allocate_row(num_rows);
        int isNewlyMarkedCol[num_cols]; // = allocate_row(num_cols);
        set_all_to(isNewlyMarkedRow, num_rows, 0);
        set_all_to(isNewlyMarkedCol, num_cols, 0);

        int newlyMarkedCount = 0;

        for (i = 0; i < num_rows; i++)
            if (!isDiscardedRow[i])
            {
                isMarkedRow[i] = 1;
                isNewlyMarkedRow[i] = 1;
                newlyMarkedCount++;
            }

        char tmp_bit = 1;

        while (newlyMarkedCount > 0)
        {
            newlyMarkedCount = 0;

            if (tmp_bit)
            {   /* Mark all cols having zero in newly marked row  */

                set_all_to(isNewlyMarkedCol, num_cols, 0); /* clearing NewlyMarkedCol array for remarking */

                int i;
                for (i = 0; i < num_rows; i++)
                {
                    if (isNewlyMarkedRow[i])
                    {
                        int j;
                        for (j = 0; j < num_cols; j++)
                        {
                            if (cost[i][j] == 0 && !isMarkedCol[j])
                            {
                                isMarkedCol[j] = 1;
                                isNewlyMarkedCol[j] = 1;
                                newlyMarkedCount++;
                            }
                        }
                    }
                }
            }

            else
            { /*Mark all rows having assignment in newly marked cols */

                set_all_to(isNewlyMarkedRow, num_rows, 0); /* clearing NewlyMarkedRow array for remarking */

                int j;
                for (j = 0; j < num_cols; j++)
                {
                    if (isNewlyMarkedCol[j])
                    {

                        if (isDiscardedCol[j])
                        {
                            int i;
                            for (i = 0; i < num_rows; i++)
                            {
                                if (isAssigned[i][j] && !isMarkedRow[i])
                                { /*Has assignment on (i,j) position */
                                    isMarkedRow[i] = 1;
                                    isNewlyMarkedRow[i] = 1;
                                    newlyMarkedCount++;
                                }
                            }
                        }
                    }
                }
            }

            tmp_bit ^= 1; /* toggles */
        }

        for (i = 0; i < num_rows; i++)
            if (!isMarkedRow[i])
            {
                isLinedRow[i] = 1;
                numLines++;
            }

        for (j = 0; j < num_cols; j++)
            if (isMarkedCol[j])
            {
                isLinedCol[j] = 1;
                numLines++;
            }

        printf("Drawn lines :\n");
        printLines(isLinedRow, isLinedCol, num_rows, num_cols);

        printf("\nMinimum number of lines = %d\n", numLines);

        

        
        if (numLines > num_rows)
        { //Should never happen
            printf("Error: Unexpected behaviour : numLines > num_rows");
            return 0;
        }

        if(numLines < num_cols)
        { /* numLines < num_rows : Further optimization needed */
            printf("Optimal condition not reached. Doing necessary changes to cost matrix ...\n");

            int minUncovered = findMinUncovered(cost, num_rows, num_cols, isLinedRow, isLinedCol);

            /* subtract minUncovered from all uncovered and add to all intersection of two lines */

            for (i = 0; i < num_rows; i++)
            {
                for (j = 0; j < num_cols; j++)
                {

                    if (!(isLinedRow[i] || isLinedCol[j])) /* Uncovered */
                        cost[i][j] -= minUncovered;

                    else if (isLinedRow[i] && isLinedCol[j])  /* Intersection of two lines (covered twice) */
                        cost[i][j] += minUncovered;
                }
            }
        }

        /*Deallocation*/
        //free(isLinedRow);
        //free(isLinedCol);
        //free(isDiscardedRow);
        //free(isDiscardedCol);
        //free(isMarkedRow);
        //free(isMarkedCol);
        //free(isNewlyMarkedRow);
        //free(isNewlyMarkedCol);
        deallocate_matrix_int(isAssigned, num_rows, num_cols);
        /*________________________________*/

        if (numLines == num_rows) /* Optimal Solution Reached */
            break;
    }

    /* Optimal Solution Already Reached */
    puts("\n\nOptimal condition for cost matrix is reached...\n");

    /* Now assigning zeros (tick) */

    int **isAssignedCell = allocate_matrix_int(num_rows, num_cols);
    int *isDiscardedRow = allocate_row(num_rows);
    int *isDiscardedCol = allocate_row(num_cols);
    int numAssigned = 0;

    while (numAssigned < num_rows)
    {
        char shouldCheckAgain = 1;
        int i, j;

        while (shouldCheckAgain)
        {
            shouldCheckAgain = 0;

            /* check rows */
            for (i = 0; i < num_rows; i++)
            {
                if (!isDiscardedRow[i])
                {
                    node cell = findOnlyNondiscardedZero(cost, i, i, 0, num_cols - 1, isDiscardedRow, isDiscardedCol);

                    if (cell.row != -1)
                    { /* valid cell */
                        isAssignedCell[cell.row][cell.col] = 1;
                        isDiscardedRow[cell.row] = 1;
                        isDiscardedCol[cell.col] = 1;
                        numAssigned++;
                        shouldCheckAgain = 1;
                    }
                }
            }

            /* check columns */
            for (j = 0; j < num_cols; j++)
            {
                if (!isDiscardedCol[j])
                {
                    node cell = findOnlyNondiscardedZero(cost, 0, num_rows - 1, j, j, isDiscardedRow, isDiscardedCol);

                    if (cell.row != -1)
                    { /* valid cell */
                        isAssignedCell[cell.row][cell.col] = 1;
                        isDiscardedRow[cell.row] = 1;
                        isDiscardedCol[cell.col] = 1;
                        numAssigned++;
                        shouldCheckAgain = 1;
                    }
                }
            }
        }

        if (numAssigned < num_rows)
        {
            /* make one assignment around an arbitary non discarded zero */
            for (i = 0; i < num_rows; i++)
            {
                if (!isDiscardedRow[i])
                {
                    for(j=0; j<num_cols; j++){
                        if(cost[i][j]==0 && !isDiscardedCol[j]){
                            isAssignedCell[i][j] = 1;
                            isDiscardedRow[i] = 1;
                            isDiscardedCol[j] = 1;
                            numAssigned++;
                            break;
                        }
                    }
                }
            }
        }
    }




    printf("\nThe assignment as a matrix(includes dummy rows/cols, if any) is (1 indicates assigned cell):\n");
    printMatrix(isAssignedCell, 0, num_rows - 1, 0, num_cols - 1);


    int assignmentCost = 0;
    int i, j;
    puts("\n\n\n  => The final optimal assignment (excludes any dummy rows/cols) is : \n");
    for (i = 0; i < original_num_rows; i++)
    {
        for (j = 0; j < original_num_cols; j++)
        {
            if (isAssignedCell[i][j])
            {
                assignmentCost += originalCost[i][j];

                printf("\tRow %d => Column %d \n", i + 1, j + 1);
            }
        }
    }

    printf("\n  => Total assignment cost (profit if maximization) is = %d\n", assignmentCost);


    /*Deallocation*/
    free(isDiscardedRow);
    free(isDiscardedCol);
    deallocate_matrix_int(isAssignedCell, num_rows, num_cols);
    deallocate_matrix_int(cost, num_rows, num_cols);
    deallocate_matrix_int(originalCost, original_num_rows, original_num_cols);


    printf("\nYou have reached the end of the program. Press enter to exit.");
    getchar(); getchar();

    return 0;
}

void subtractMinFromRows(int **cost, int num_rows, int num_cols)
{

    int i;

    for (i = 0; i < num_rows; i++)
    {
        int minVal = INT_MAX;

        int j;
        for (j = 0; j < num_cols; j++)
            minVal = min(cost[i][j], minVal);

        for (j = 0; j < num_cols; j++)
            cost[i][j] -= minVal;
    }
}

void subtractMinFromCols(int **cost, int num_rows, int num_cols)
{

    int j;

    for (j = 0; j < num_cols; j++)
    {
        int minVal = INT_MAX;

        int i;
        for (i = 0; i < num_rows; i++)
            minVal = min(cost[i][j], minVal);

        for (i = 0; i < num_rows; i++)
            cost[i][j] -= minVal;
    }
}

node findOnlyNondiscardedZero(int **cost, int r1, int r2, int c1, int c2, int *isDiscardedRow, int *isDiscardedCol)
{
    node cell = {-1, -1};
    int count = 0;

    int i, j;
    for (i = r1; i <= r2; i++)
    {
        for (j = c1; j <= c2; j++)
        {
            if (cost[i][j] == 0 && !(isDiscardedRow[i] || isDiscardedCol[j]))
            {
                cell.row = i;
                cell.col = j;
                count++;
            }
        }
    }

    if (count != 1)
    {
        cell.row = cell.col = -1;
    }

    return cell;
}

int findMinUncovered(int **cost, int num_rows, int num_cols, int *isLinedRow, int *isLinedCol)
{

    int minVal = INT_MAX;

    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_cols; j++)
        {
            if (!(isLinedRow[i] || isLinedCol[j]))
                minVal = min(cost[i][j], minVal);
        }
    }

    return minVal;
}

/*   ################################################################################  */

void printLines(int *isLinedRow, int *isLinedCol, int num_rows, int num_cols)
{

    int i, j;

    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_cols; j++)
        {
            if (isLinedRow[i] && isLinedCol[j])
                printf("|");
            else if (isLinedRow[i])
                printf("----");
            else if (isLinedCol[j])
                printf("|");
            else
                printf(" *  ");
        }
        printf("\n");
    }
}

void printMatrix(int **A, int r1, int r2, int c1, int c2)
{
    int i, j;
    for (int i = r1; i <= r2; i++)
    {
        for (int j = c1; j <= c2; j++)
        {
            printf("%d\t", A[i][j]);
        }
        printf("\n");
    }
}

int **allocate_matrix_int(int num_rows, int num_cols)
{
    int **matrix = (int **)malloc(num_rows * sizeof(int *));

    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        matrix[i] = (int *)malloc(num_cols * sizeof(int));

        for (j = 0; j < num_cols; j++)
            matrix[i][j] = 0;
    }

    return matrix;
}

int *allocate_row(int n)
{
    int *row = (int *)malloc(n * sizeof(int));
    int i;
    for (i = 0; i < n; i++)
        row[i] = 0;

    return row;
}

void deallocate_matrix_int(int **matrix, int num_rows, int num_cols)
{
    int i;
    for (i = 0; i < num_rows; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
}

void input_row_int(int *row, int n)
{
    int i;
    for (i = 0; i < n; i++)
        scanf("%d", &row[i]);
}

void input_matrix_int(int **matrix, int num_rows, int num_cols)
{
    int i, j;
    for (i = 0; i < num_rows; i++)
    {
        input_row_int(matrix[i], num_cols);
    }
}

void makeNegetive(int **cost, int num_rows, int num_cols)
{
    int i, j;

    for (i = 0; i < num_rows; i++)
    {
        for (j = 0; j < num_cols; j++)
        {
            cost[i][j] *= -1;
        }
    }
}


void set_all_to(int *arr, int n, int val)
{
    int i;
    for (i = 0; i < n; i++)
        arr[i] = val;
}

void copy_matrix_int(int **src, int **dest, int r1, int r2, int c1, int c2)
{
    int i, j;
    for (i = r1; i <= r2; i++)
    {
        for (j = c1; j <= c2; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}
