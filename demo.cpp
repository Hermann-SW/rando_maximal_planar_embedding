#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/draw_polyhedron.h>

#include <signal.h>

typedef CGAL::Simple_cartesian<double>     Kernel;
typedef Kernel::Vector_3                   Vector_3;
typedef Kernel::Point_3                    Point_3;
typedef CGAL::Polyhedron_3<Kernel>         Polyhedron;
typedef Polyhedron::Vertex_iterator        Vertex_iterator;
typedef Polyhedron::Facet_iterator         Facet_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator         HF_circulator;
typedef Polyhedron::Halfedge_handle                          Halfedge_handle;

// from: examples/Polyhedron/polyhedron_prog_subdiv_with_boundary.cpp#L25-L37
//
void create_center_vertex(Polyhedron *P, Facet_iterator f) {
    Vector_3 vec(0.0, 0.0, 0.0);
    std::size_t order = 0;
    HF_circulator h = f->facet_begin();
    do {
        vec = vec + (h->vertex()->point() - CGAL::ORIGIN);
        ++order;
    } while ( ++h != f->facet_begin());
    assert(order >= 3);
    Point_3 center =  CGAL::ORIGIN + (vec / static_cast<double>(order));
    Halfedge_handle new_center = P->create_center_vertex(f->halfedge());
    new_center->vertex()->point() = center;
}

void draw_timeout(const Polyhedron* P, int ms) {
    pid_t pid = fork();
           if (pid == -1) { exit(1);
    } else if  (pid == 0) { CGAL::draw(*P);
    } else if   (pid > 0) { usleep(1000*ms); kill(pid, SIGKILL);
    }
}

int main(int argc, char *argv[]) {
    int ms = (argc < 2) ? 1000 : atoi(argv[1]);

    Point_3 p(0.0, 0.0, 0.0);
    Point_3 q(0.5, sqrt(3.0)/2.0, 0.0);
    Point_3 r(1.0, 0.0, 0.0);
    Point_3 s(0.5, sqrt(3.0)/6.0, 0.0);

    Polyhedron P;
    boost::graph_traits<Polyhedron>::halfedge_descriptor hn;
    boost::graph_traits<Polyhedron>::halfedge_descriptor h
        = P.make_tetrahedron(s, p, q, r);

    CGAL::IO::set_ascii_mode(std::cout);

    for ( Vertex_iterator v = P.vertices_begin(); v != P.vertices_end(); ++v )
        std::cout << v->point() << std::endl;

    std::cout << "=====" << std::endl;

    draw_timeout(&P, ms);

//    P.join_facet(h);
//    P.split_edge(h);

    for (int i = 0; i < 3 + 6+12; ++i) {
        // hn=h->opposite()->prev();
        hn = h->next()->opposite();
        create_center_vertex(&P, h->facet());

        if (i == 20) { CGAL::draw(P); } else { draw_timeout(&P, ms); }

        h = hn;
    }

    return 0;
}
