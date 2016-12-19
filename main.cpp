/*
 * C++ Program to Solve Sudoku Problem using BackTracking
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath> //needed for ceil function
#include <fstream>
#include <cctype>
#include <sstream>                     // Need to have in the preamble to convert integers into strings

using namespace std;
#define UNASSIGNED 0
#define N 9

bool FindUnassignedLocation(int grid[N][N], int &row, int &col);
bool isSafe(int grid[N][N], int row, int col, int num);

// Declare global arrays
char table[9][9]; //2d array used for storing table as character values
int grid[9][9]; //2d array used for storing table as integer values
int blanks = 0; //set the number of blanks in the sudoku as 0
int value[9];


/* print grid  */
void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            cout<<grid[row][col]<<"  ";
            if ((col+1)%3 == 0)
                cout << " ";

        }
        cout<<endl;
        if ((row+1)%3 == 0)
            cout << endl;

    }
}

string charToStr(int i, int j)
{
    stringstream ss;
    string s;
    char c = table[i][j];
    ss << c;
    ss >> s;
    return s;
}

int strToInt (string s)
{
    int num;
    stringstream ss;
    ss << s;
    ss >> num;
    return num;
}

void convertToInt(void)
{
    //  From integer to string
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            string str = charToStr(i, j);
            int num = strToInt (str);
            grid[j][i] = num;
        }
    }
}

void readTXT(void)
{
    cout<<"Place the table as a c.txt file in the folder where this code exists"<<endl;
    cout<<"Enter 1 when ready ";
    char ready;
    cin>>ready;
    if(ready=='1')
    {
        int y=0;
        fstream tableTXT; //declaration of txt file of sudoku table
        string line; //declaration of string for sudoku lines
        tableTXT.open("c.txt"); //open text file
        if(tableTXT.is_open()) //check that file was correctly opened
        {
            while(getline(tableTXT, line)) //loop through every line
            {
                for(int x=0; x<9; x++)
                {
                    table[x][y]=line[x]; //store characters from txt file line in global table array
                    if(line[x]=='0') //if a blank is seen
                        blanks++; //increment blanks
                }
                y++; //increment line position or y axis position
            }
        }
        else //in case it's unable to open .txt file
            cout<<"Error: Unable to retrieve table txt file"<<endl; //failure statement
    }
    else
    {
        cout<<"Invalid"<<endl;
    }
}

void numGenerator (int num [9])
{
    for (int i=0; i<9; i++)
    {
        bool check; //variable to check or number is already used
        int n; //variable to store the number in
        do
        {
            n=rand()%9+1;
            //check or number is already used:
            check=true;
            for (int j=0; j<i; j++)
                if (n == num[j]) //if number is already used
                {
                    check=false; //set check to false
                    break; //no need to check the other elements of value[]
                }
        }
        while (!check);   //loop until new, unique number is found
        num[i]=n; //store the generated number in the array
    }
}


/* assign values to all unassigned locations for Sudoku solution
 */
bool SolveSudoku(int grid[N][N])
{
    int row, col;
    if (!FindUnassignedLocation(grid, row, col))
        return true;
    for (int i = 0; i < 9; i++)
    {
        int num = value[i];
        if (isSafe(grid, row, col, num))
        {
            grid[row][col] = num;
            if (SolveSudoku(grid))
                return true;
            grid[row][col] = UNASSIGNED;
        }
    }
    return false;
}
/* assign values to all unassigned locations for Sudoku solution
 */

/* Searches the grid to find an entry that is still unassigned. */
bool FindUnassignedLocation(int grid[N][N], int &row, int &col)
{
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == UNASSIGNED)
                return true;
    return false;
}

/* Returns whether any assigned entry n the specified row matches
   the given number. */
bool UsedInRow(int grid[N][N], int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Returns whether any assigned entry in the specified column matches
   the given number. */
bool UsedInCol(int grid[N][N], int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}

/* Returns whether any assigned entry within the specified 3x3 box matches
   the given number. */
bool UsedInBox(int grid[N][N], int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
    return false;
}

/* Returns whether it will be legal to assign num to the given row,col location.
 */
bool isSafe(int grid[N][N], int row, int col, int num)
{
    return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) &&
           !UsedInBox(grid, row - row % 3 , col - col % 3, num);
}


bool rowTest(int row, int number)
{
    for(int d=0; d<9; d++) //loop through columns
        if(table[row][d]==number) //if any numbers in that row conflicts
            return false; //fail
    return true; //success
}

bool colTest(int col, int number)
{
    for(int e=0; e<9; e++) //loop through rows
        if(table[e][col]==number) //if any numbers in that column conflicts
            return false; //fail
    return true; //success
}

bool boxTest(int row, int col, int number)
{
    int r=ceil((row+1)/3.); //determine which box row the blank falls in
    int c=ceil((col+1)/3.); //determine which box column the blank falls in
    for(int g=(r-1)*3; g<(r-1)*3+3; g++) //cycle through that box's rows
        for(int h=(c-1)*3; h<(c-1)*3+3; h++) //cycle through that box's columns
            if(table[g][h]==number) //if any number conflicts
                return false; //fail
    return true; //success
}

bool solveTable()
{
    int emptytries = 0;
    int possibilities=0; //declaration of possibilities for a specific blank
    while(blanks>0) //run this code so long as the grid still has spaces
    {
        int blanksNum = blanks;
        for(int a=0; a<9; a++) //loop through y axis
        {
            for(int b=0; b<9; b++) //loop through x axis
            {
                if(grid[b][a]!=0) //if isn't a blank, ignore it
                    continue;
                else
                {
                    possibilities=0; //reset possibilities for each new grid blank
                    for(int c=1; c<=9; c++) //loop through char values of 1 through 9
                        if(rowTest(b, c) and colTest(a, c) and boxTest(b, a, c)) //test functions to see if number conflicts with grid
                        {
                            if(possibilities==0) //if there is no other possibility so far
                            {
                                grid[b][a]=c; //set that grid position at the number that works
                                blanks--; //reduce blanks
                                possibilities++; //increase possibilities
                            }
                            else //when theres more than one possibility
                            {
                                grid[b][a]=0; //ignore first possibility
                                blanks++; //reincrement blank
                                break; //break out of this spot on the grid and go to next blank
                            }
                        }
                }
            }
        }
        if (blanksNum == blanks)
        {
            emptytries++;
        }
        if (emptytries == 500)
        {
            return true;
        }
    }
    return true;
}

void removeNumbers()
{
    int t=0,counter=0;
    blanks = 0;
    int i, j, k, oldValue1;
    while(true)
    {
        t++;
        j=rand()%8+0;
        i=rand()%8+0;
        oldValue1 = grid[j][i];
        grid[j][i] = 0;
        blanks ++;
        if (!solveTable())
        {
            grid[j][i] = oldValue1;
            blanks --;
            counter++;
            if (counter > 300)
                break;
        }
        if (t==50)
            break;
    }
    printGrid(grid);
}

/* Main */
int main()
{
    numGenerator(value);
    /* To print the list of random values for debuggings
    for (int q=0; q<9; q++)
    {
        cout << value[q]<< endl;
    }
    */
    cout << "Press 1 for the program to solve a sudoku grid and 2 for the program to generate a grid" << endl;
    int input;
    cin >> input;
    if (input == 1)
    {
        readTXT();
        convertToInt();
        cout << "The solution to the following sudoku grid: " << endl;
        printGrid(grid);
        cout << endl;
        cout << "is: " << endl;
        if (SolveSudoku(grid) == true)
            printGrid(grid);
        else
            cout<<"No solution exists"<<endl;
    }
    if (input == 2)
    {
        for (int i = 0; i< 9; i++)
        {
            for (int j = 0; j<9; j++)
            {
                grid [i][j] = 0;
            }
        }
        //enterRandomValues();
        cout << "initialized grid" << endl;
        printGrid(grid);
        cout << endl;
        SolveSudoku(grid);
        cout << "solved grid" << endl;
        printGrid(grid);
        cout << endl;
        removeNumbers();
        cout << "unsolved grid" << endl;
        printGrid(grid);
    }
    return 0;
}






/*
void enterRandomValues()
{
    int n, i, j, k;
    for (k=0; k<7;)
    {
        n=rand()%9+1;
        j=rand()%9+1;
        i=rand()%9+1;
        if (isSafe(grid, j, i, n))
        {
            grid[j][i] = n;
            k++;
        }
    }
    for (int a=0; a<9; a++)
    {
        for (int b=0; b<9; b++)
        {
            if (grid[a][b] == 1 ||grid[a][b] == 2 ||grid[a][b] == 3 ||grid[a][b] == 4 ||grid[a][b] == 5 ||grid[a][b] == 6 ||grid[a][b] == 9 ||grid[a][b] == 8 ||grid[a][b] == 9)
            {
                ;
            }
            else
                grid [a][b] == 0;
        }
    }
    for (int i=0; i<9; i++)
    {
        bool check; //variable to check or number is already used
        int n; //variable to store the number in
        do
        {
            n=rand()%9+1;
            //check or number is already used:
            check=true;
            for (int j=0; j<i; j++)
                if (n == value[j]) //if number is already used
                {
                    check=false; //set check to false
                    break; //no need to check the other elements of value[]
                }
        }
        while (!check);   //loop until new, unique number is found
        value[i]=n; //store the generated number in the array
    }
}
*/

/*
if (t == 0)
    {int blanksCounter = 0, oldValue1;
    int row[9], col[9];
    numGenerator(row);
    numGenerator(col);
    oldValue1 = grid[row[3]][col[3]];
    cout << oldValue1 << endl;

    for (; solveTable(); i++)
    {
        oldValue1 = grid[row[i]][col[i]];
        blanks = blanksCounter;
        grid[row[i]][col[i]] = 0;
        blanksCounter++;
    }
    t++;
    if (t<10)
        removeNumbers();
    cout << i << endl;
    grid[row[i]][col[i]] = oldValue1;
*/
