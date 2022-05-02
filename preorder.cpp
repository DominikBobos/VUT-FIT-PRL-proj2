/**
 * Subject: PRL
 * Algorithm: Pre-order tree tour
 * Author: Dominik Boboš <xbobos00@stud.fit.vutbr.cz>
 * Ac.year: 2021/2022
 * Description: C++ source of an parallel algorithm of pre-order tree tour by using OpenMPI lib.
 */

#include <mpi.h>
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

#define MASTER 1 /// Rank of the master process.
#define COMM MPI_COMM_WORLD    /// MPI communicator.


/**
 * Shows err message to stderr and terminates the program with macro value EXIT_FAILURE
 */
auto MPI_error() -> void
{
    cerr << "Some error in MPI library call." << endl;
    MPI_Abort(COMM, EXIT_FAILURE);
}


/**
 * Find Euler tour, find successors amd gather the information
 * @param rank processor rank
 * @param n count of vertices
 * @return the new value
 */
int euler_tour(int rank, int n)
{
    if (rank == n+1)                                        // root
        return rank;
    if (rank < n)                                           // edge is forward
    {
        if ((rank >= floor(n / 2)) && (rank < n))        // going to the leaves
            return rank + n - 1;
        else
            return 2 * rank + 1;
    }
    else
    {
        if (((rank - n) % 2 == 0) && (rank < 2 * n - 2))    // if the node, which I am returning to, has right child
            return rank - n + 2;
        else
            return (n - 2) / 2 + (rank / 2);
    }
}


/**
 * Count forward edges
 * @param rank processor rank
 * @param arr int array with indices of Euler tour
 * @param arr_weights int array with info about forward edges
 * @return count of forward edges
 */
int count_forward_edges(int rank, const int* arr, const int* arr_weights)
{
    int forward_edges = 0;

    for(int i = rank - 1; arr[i] != i + 1; i = arr[i] - 1)
    {
        if (arr_weights[i])
            forward_edges++;
    }
    return forward_edges;
}

/**
 * Shows the preorder sequence to STDOUT
 * @param input Sequence of characters from the input
 * @param arr final array with correct pre order tour of the binary tree composed from the input
 * @param n number of vertices == the same as the length of input, but this way it is more readable
 */
void show_preorder(const char* input, const int* arr, int n)
{
    char preorder[n+1];
    preorder[0] = input[0];
    for(int i = 0; i < n - 1; i++)
    {
        preorder[n-arr[i]] = input[i+1];
    }
    preorder[n] = '\0';
    cout << preorder << endl;
}
/**
 * Main program of the algorithm
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    int proc_count;                             // Number of processors
    int rank;                                   // Processor rank
    char input[strlen(argv[1])];             // Input sequence
    strcpy(input, argv[1]);
    int n = (int) strlen(input);             // Nodes/vertices count

    if (MPI_Init(&argc,&argv))
        MPI_error();
    if (MPI_Comm_size(COMM, &proc_count))
        MPI_error();
    if (MPI_Comm_rank(COMM, &rank))
        MPI_error();

    rank++;  // index processors from 1

    // If the input is less than 4 characters long it is always returning the same order as the input
    if (strlen(input) < 4)
    {
        if (rank == MASTER)
            cout<<input;
        MPI_Finalize();
        return EXIT_SUCCESS;
    }

    // Find Euler tour and gather the information
    int new_value = euler_tour(rank, n);
    int arr[proc_count];
    MPI_Allgather(&new_value, 1, MPI_INT, &arr, 1, MPI_INT, COMM);

    // Find weights
    int arr_weights[proc_count];
    int weight = (rank < n) ? 1 : 0;  // if the edge is forward or not... rank < n is sufficient standalone but this is more readable
    MPI_Allgather(&weight, 1, MPI_INT, &arr_weights, 1, MPI_INT, COMM);

    // Count parallel suffix sum
    int arr_final[proc_count];
    int forward_edges = count_forward_edges(rank, arr, arr_weights);
    MPI_Allgather(&forward_edges, 1, MPI_INT, &arr_final, 1, MPI_INT, COMM);

    if(rank == MASTER)
        show_preorder(input, arr_final, n);

    MPI_Finalize();
    return EXIT_SUCCESS;
}