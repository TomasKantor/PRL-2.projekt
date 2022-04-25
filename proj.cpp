#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <list>

struct AdjListItem
{
    unsigned int edge_num;
    unsigned int reverse_edge_num;
    unsigned int from_vertex;
    unsigned int to_vertex;
    unsigned int next;
};

using AdjList = std::vector<AdjListItem>;

inline unsigned int get_parent(unsigned int i)
{
    return i >> 1;
}

inline unsigned int get_left_children(unsigned int i)
{
    return i << 1;
}

inline unsigned int get_right_children(unsigned int i)
{
    return (i << 1) + 1;
}

inline unsigned int get_edge_from_parent(unsigned int i)
{
    return (i << 1) - 3;
}

void get_edges_from_vertex(unsigned int i, unsigned int vertices, unsigned int &pe, unsigned int  &le, unsigned int &re)
{
    unsigned int min_edge = 1;
    unsigned int max_edge = 2*vertices - 2;

    unsigned int l = get_left_children(i);
    unsigned int r = get_right_children(i);
    pe = get_edge_from_parent(i) + 1; // edge to parent
    le = get_edge_from_parent(l); // edge to left child
    re = get_edge_from_parent(r); // edge to right child

    if( pe < min_edge || pe > max_edge ) // if valid edge
    {
        pe = 0;
    }

    if( le < min_edge || le > max_edge )
    {
        le = 0;
    }

    if( re < min_edge || re > max_edge )
    {
        re = 0;
    }
}

void create_adj_list(unsigned int vertices, AdjList &adj_list)
{
    for( int i = 1; i <= vertices; i++ )
    {
        unsigned int pe,le,re; // edges from vertex i
        AdjListItem p_item,l_item,r_item; //adj_list items

        get_edges_from_vertex(i, vertices, pe, le, re);


        if(pe) // valid edge to parent
        {
            p_item.edge_num = pe;
            p_item.reverse_edge_num = pe - 1;
            p_item.from_vertex = i;
            p_item.to_vertex = get_parent(i);
            p_item.next = le; // edge or 0 if invalid le

            adj_list[pe]= p_item;
        }

        if(le) // valid edge
        {
            l_item.edge_num = le;
            l_item.reverse_edge_num = le + 1;
            l_item.from_vertex = i;
            l_item.to_vertex = get_left_children(i);
            l_item.next = re; // edge or 0 if invalid re

            adj_list[le] = l_item;
        }

        if(re) // valid edge
        {
            r_item.edge_num = re;
            r_item.reverse_edge_num = re + 1;
            r_item.from_vertex = i;
            r_item.to_vertex = get_right_children(i);
            r_item.next = 0;

            adj_list[re] = r_item;
        }
    }
    
}

void print_adj_list(AdjList adj_list)
{
    for(auto adj_item : adj_list )
    {
        std::cout << "f:" << adj_item.from_vertex << " t:" << adj_item.to_vertex << " e:" << adj_item.edge_num << " r:" << adj_item.reverse_edge_num << "\n";
    }
}

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);

    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // reading input, create adjcency list
    if( rank == 0)
    {
        std::cout << argv[1] << std::endl;

        if( argc != 2)
        {
            std::cerr << "Requires exactly 1 argument\n";
        }

        unsigned int input_size = strlen(argv[1]);


        AdjList adj_list(2*input_size - 1); // 2n - 2 + 1
        create_adj_list(input_size, adj_list);
        // create adjcency list - represented as adjacency list
        // forward edge is even, bacward edge is odd, backward = forward + 1

        print_adj_list(adj_list);

        char *val = argv[1];
        std::vector<char> preorder_output(input_size + 1);
        std::vector<int> etour(2*input_size - 2 + 1);

    }

    // parallel part

    // create etour





    //  subfix sum ??

    // preorder



    MPI_Finalize();
}