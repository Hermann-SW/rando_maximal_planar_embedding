//=======================================================================
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
/*
f=straight_line_drawing
g++ -O3 -Wall -pedantic -Wextra $f.cpp -o $f
cpplint --filter=-legal/copyright,-build/namespaces,-runtime/reference $f.cpp
cppcheck --enable=all --suppress=missingIncludeSystem $f.cpp --check-config
*/
//=======================================================================
#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/planar_canonical_ordering.hpp>
#include <boost/graph/is_straight_line_drawing.hpp>
#include <boost/graph/chrobak_payne_drawing.hpp>


using namespace boost;

struct coord_t {
    std::size_t x;
    std::size_t y;
};

struct output_visitor : public planar_face_traversal_visitor {
    void begin_face() { std::cout << "3 "; }
    void end_face() { std::cout << std::endl; }
};

struct vertex_output_visitor : public output_visitor {
    template < typename Vertex > void next_vertex(Vertex v) {
        std::cout << v << " ";
    }
};


template <class graphType>
void
read_graph(graphType& g, char* gname) {
    int n, m;
    char line[200];

    std::ifstream in(gname);
    assert(in);

    in >> line;
    in >> line;
    in >> line;
    in >> n;

    g = graphType(n);

    for (int i=1; i <= n; ++i) {
        in >> line;
    }

    in >> m;
    for (int i=1; i <= m; ++i) {
        int s, t, v;
        in >> s >> t >> v;
        add_edge(s-1, t-1, g);
    }
}


template <class graphType, class sld_t, class emb_t>
void
write_off(graphType& g, sld_t sld, emb_t embedding,
          std::ostream& out = std::cout) {
    out << "OFF" << std::endl;
    out << num_vertices(g) << " " << num_edges(g)+2-num_vertices(g)
        << " 0" << std::endl;
    out << std::endl;

    BGL_FORALL_VERTICES_T(v, g, graphType) {
        coord_t coord(get(sld, v));
        out << coord.x << " " << coord.y << " 0" << std::endl;
    }
    out << std::endl;

    std::map< typename graph_traits< graphType >::edge_descriptor, size_t >
        mapIndex;
    boost::associative_property_map<
        std::map< typename graph_traits< graphType >::edge_descriptor, size_t >
    > my_edge_index_map(mapIndex);
    typename graph_traits< graphType >::edges_size_type edge_count = 0;
    BGL_FORALL_EDGES_T(e, g, graphType)
        put(my_edge_index_map, e, edge_count++);

    vertex_output_visitor v_vis;
    planar_face_traversal(g, &embedding[0], v_vis, my_edge_index_map);
    out << std::endl;
}

template <class graphType, class emb_t>
void
write_err(graphType& g,  emb_t embedding) {
    BGL_FORALL_VERTICES_T(v, g, graphType) {
        typename std::vector<
            typename graph_traits< graphType >::edge_descriptor
        >::const_iterator ei, ei_end = embedding[v].end();

        for (ei = embedding[v].begin(); ei != ei_end; ++ei) {
            std::cerr << *ei << " ";
        }
        std::cerr << std::endl;
    }
    std::cerr << std::endl;

    print_graph(g, std::cerr);
}


typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> gtype;

typedef std::vector< std::vector< graph_traits< gtype >::edge_descriptor > >
        embedding_storage_t;
typedef boost::iterator_property_map< embedding_storage_t::iterator,
        property_map< gtype, vertex_index_t >::type > embedding_t;

    typedef std::vector< coord_t > straight_line_drawing_storage_t;
    typedef boost::iterator_property_map<
        straight_line_drawing_storage_t::iterator,
        property_map< gtype, vertex_index_t >::type
    >
    straight_line_drawing_t;

template <class graphType, class str, class emb_t, class emb_s>
void
read_and_embedd(graphType& g, str name, emb_t& embedding,
                                        emb_s& embedding_storage) {
    read_graph(g, name);

    embedding_storage = embedding_storage_t(num_vertices(g));
    embedding = embedding_t(embedding_storage.begin(), get(vertex_index, g));

    assert(boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,
        boyer_myrvold_params::embedding = embedding));
}

template <class graphType, class emb_t, class sld_t, class slds_t>
void
do_straight_line_drawing(graphType& g, emb_t& embedding, sld_t& sld,
                                                         slds_t& slds) {
    slds = slds_t(num_vertices(g));
    sld = sld_t(slds.begin(), get(vertex_index, g));

    std::vector< graph_traits< gtype >::vertex_descriptor > ord;

    planar_canonical_ordering(g, embedding, std::back_inserter(ord));

    chrobak_payne_straight_line_drawing(
        g, embedding, ord.begin(), ord.end(), sld);

    assert(is_straight_line_drawing(g, sld));
}

int main(int argc, char** argv) {
    assert(argc == 2);

    gtype g;
    embedding_storage_t embedding_storage;
    embedding_t embedding;

    read_and_embedd(g, argv[1], embedding,
                                embedding_storage);
    //                              lifetime

    straight_line_drawing_storage_t straight_line_drawing_storage;
    straight_line_drawing_t straight_line_drawing;

    do_straight_line_drawing(g, embedding, straight_line_drawing,
                                           straight_line_drawing_storage);
    //                                         lifetime


    write_off(g, straight_line_drawing, embedding);

    write_err(g, embedding);

    return 0;
}
