#ifndef CGAL_DRAW_PLANAR_H
#define CGAL_DRAW_PLANAR_H

#include <CGAL/license/Polyhedron.h>
#include <CGAL/Qt/Basic_viewer_qt.h>
//#include <CGAL/Qt/Basic_viewer.h>

#ifdef CGAL_USE_BASIC_VIEWER
#include <CGAL/Qt/init_ogl_context.h>
#include <CGAL/Polyhedron_3.h>
#include "draw_face_graph.mod.h"
#include <CGAL/Random.h>

namespace CGAL
{
#define CGAL_POLY_TYPE CGAL::Polyhedron_3 \
  <PolyhedronTraits_3, PolyhedronItems_3, T_HDS, Alloc>

template<class PolyhedronTraits_3,
         class PolyhedronItems_3,
         template < class T, class I, class A>
         class T_HDS,
         class Alloc>
void draw(const CGAL_POLY_TYPE& apoly,
          const char* title="Polyhedron Basic Viewer",
          bool nofill=false)
{
    CGAL::Qt::init_ogl_context(4,3);
    int argc=1;
    const char* argv[2]={"polyhedron_viewer", nullptr};
    QApplication app(argc,const_cast<char**>(argv));
    SimpleFaceGraphViewerQt
      mainwindow(app.activeWindow(), apoly, title, nofill);
    mainwindow.show();
    app.exec();
}

#undef CGAL_POLY_TYPE
}
#endif
#endif
