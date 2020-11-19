#include <omp.h>
#include<chrono>
#include <iostream>  
#include <iomanip> 

using namespace std;
using namespace std::chrono;

// idea - we can create maximum N thread where each thread has its own array to maintain positions of queen
// In this program work is divided among threads in first row itelf
// It can go upto d level and then we can divide the work using master slave


int tot_sol=0; // total solutions
int n; // number of queens


void setQueen(int queens[], int row, int col)
{

    for(int i=0; i<row; i++)
    {
        // vertical attacks
        if (queens[i]==col)
        {
            return;
        }
        // diagonal attacks
        if ( abs(queens[i]-col) == (row-i) )
        {
            return;
        }
    }

    //set the queen in this row at column number col
    queens[row]=col;

    if(row==n-1) // this is a valid configuration
    {

        // only one thread should increase tot_sol at a time otherwise data race happens in tot_sol( this is a shared var)
        #pragma omp critical
            tot_sol++;

    }
    else 
    {
        // fill next row
        for(int i=0; i<n; i++) 
        {
          setQueen(queens, row+1, i);
        }
    }


}


   
    


int main(int argc, char*argv[])
{

  
    n = atoi(argv[1]); // number of queens in n*n matrix

    

    auto start = high_resolution_clock::now(); 

    
    #pragma omp parallel for
    for(int i=0; i<n; i++) 
    {
          
        // try all positions in first row
        // create separate array for each recursion
        // and put it in different threads

        setQueen(new int[n], 0, i);
    }

    


    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);
        
    // cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;

    cout << "Number of total solutions: " << tot_sol << endl;

    return 0;
}

// Reference paper - Accelerating N-queens problem using OpenMP