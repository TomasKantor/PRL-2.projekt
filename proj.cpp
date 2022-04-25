#include <stdio.h>
#include <mpi.h>
#include <vector>

struct AdjListItem
{
    unsigned int edge_num;
    unsigned int reverse_edge_num;
    unsigned int next_edge_num;
    unsigned int from_vertex;
    unsigned int to_vertex;
};


int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);

    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if( rank == 0)
    {
        std::cout << argv[1] << std::endl;

        if( argc != 2)
        {
            std::cerr << "Requires exactly 1 argument\n";
        }

        unsigned int input_size = strlen(argv[1]);


        std::vector<AdjListItem> adj_list(input_size);
        char* val = argv[1];
        std::vector<char> preorder_output(input_size);

        // create adjcency list - represented as adjacency list

        // forward edge is odd, bacward edge is even, they differ by 1
    
    }




    //  subfix sum ??

    // preorder



    MPI_Finalize();
}