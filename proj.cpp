#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <math.h> 

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

inline bool is_edge_forward(int edge)
{
    return edge % 2;
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

void create_adj_list(unsigned int vertices, AdjList &adj_list, std::vector<int> &adj_vertex_list)
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

            adj_vertex_list[i-1] = pe -1;

            adj_list[pe-1]= p_item;
        }

        if(le) // valid edge
        {
            l_item.edge_num = le;
            l_item.reverse_edge_num = le + 1;
            l_item.from_vertex = i;
            l_item.to_vertex = get_left_children(i);
            l_item.next = re; // edge or 0 if invalid re

            if(!pe)
            {
                adj_vertex_list[i-1] = le -1;
            }

            adj_list[le-1] = l_item;
        }

        if(re) // valid edge
        {
            r_item.edge_num = re;
            r_item.reverse_edge_num = re + 1;
            r_item.from_vertex = i;
            r_item.to_vertex = get_right_children(i);
            r_item.next = 0;

            adj_list[re-1] = r_item;
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

    int processor_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &processor_rank);

    // reading input, create adjcency list
    if( processor_rank == 0)
    {
        if( argc != 2)
        {
            std::cerr << "Requires exactly 1 argument\n";
        }
    }

    unsigned int input_size = strlen(argv[1]);

    int processors = 2*input_size - 2;
    AdjList adj_list(processors);
    std::vector<int> adj_vertex_list(input_size);
    create_adj_list(input_size, adj_list, adj_vertex_list);

    char *val = argv[1];

    // parallel part ------------------

    // compute Etour

    int edge = processor_rank + 1;
    int reverse_edge = adj_list[edge-1].reverse_edge_num;
    int next_reverse_edge = adj_list[reverse_edge-1].next;
    int etour;

    if( next_reverse_edge != 0 )
    {
        etour = next_reverse_edge;
    }else
    {
        int vertex_v = adj_list[edge-1].to_vertex;
        int vertex_start_in_adj = adj_vertex_list[vertex_v-1];
        etour = adj_list[vertex_start_in_adj].edge_num;
    }

    // Etour(e) = e, for edge e to root

    if(edge == 2 && input_size <= 1)
    {
        etour = 2;
    }

    if( edge == 4)
    {
        etour = 4;
    }

    // count_forward_edges = SuffixSum(Etour)

    int my_value;
    if( is_edge_forward(edge) )
    {
        my_value = 1;
    }else
    {
        my_value = 0;
    }

    std::vector<int> succ(processors);
    std::vector<int> value(processors);

    MPI_Allgather(
        &etour,
        1,
        MPI_INT,
        succ.data(),
        1,
        MPI_INT,
        MPI_COMM_WORLD);

    int iterations = ceil(log2(processors));
    for(int k = 1; k <= iterations; k++)
    {
        MPI_Allgather(
            &my_value,
            1,
            MPI_INT,
            &value[0],
            1,
            MPI_INT,
            MPI_COMM_WORLD);
        
        my_value = my_value + value[succ[processor_rank]-1];
        succ[processor_rank] = succ[succ[processor_rank]-1];

        MPI_Allgather(
            &succ[processor_rank],
            1,
            MPI_INT,
            succ.data(),
            1,
            MPI_INT,
            MPI_COMM_WORLD);
    }

    int vertex = 0;
    int preorder = 0;
    if(is_edge_forward(edge))
    {
        vertex = adj_list[edge-1].to_vertex;
        preorder = input_size - my_value + 1;
    }

    std::vector<int> preorder_arr(input_size);
    std::vector<int> preorder_index(processors);
    std::vector<int> preorder_value(processors);

    MPI_Gather(
    &preorder,
    1,
    MPI_INT,
    preorder_index.data(),
    1,
    MPI_INT,
    0,
    MPI_COMM_WORLD);

    MPI_Gather(
    &vertex,
    1,
    MPI_INT,
    preorder_value.data(),
    1,
    MPI_INT,
    0,
    MPI_COMM_WORLD);

    if(processor_rank==0)
    {
        preorder_arr[0] = 1;

        for(int i = 0; i < processors;i++)
        {
            int p_index = preorder_index[i];
            if(p_index != 0)
            {
                preorder_arr[p_index-1] = preorder_value[i];
            }
        }

        for(int i = 0; i < input_size; i++)
        {
            std::cout << val[preorder_arr[i]-1];
        }
        std::cout << std::endl;
    }
    
    MPI_Finalize();
}