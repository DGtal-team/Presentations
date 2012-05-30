#include <QImageReader>
#include <QtGui/qapplication.h>
#include "DGtal/io/viewers/Viewer3D.h"

#include "DGtal/helpers/StdDefs.h"
#include "DGtal/topology/SurfelAdjacency.h"
#include "DGtal/topology/DigitalSetBoundary.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/shapes/Shapes.h"

int main( int argc, char** argv )
{
  typedef Z3i::Point Point; // 3D digital point
  typedef Z3i::Domain Domain; // the domain where lies the set
  typedef Z3i::DigitalSet DigitalSet; // a set of digital points
  typedef Z3i::KSpace KSpace; // 3D cellular grid space
  typedef SurfelAdjacency<3> SAdj; // surfel adjacency.
  typedef DigitalSetBoundary<KSpace,DigitalSet> Container; // kind of surface container
  typedef DigitalSurface<Container> MyDigSurf; // concrete digital surface

  Point p1( -20, -20, -20 ), p2( 20, 20, 20 );
  KSpace K; K.init( p1, p2, true ); // init space
  DigitalSet someShape( Domain( p1, p2 ) );
  Shapes<Domain>::addNorm2Ball( someShape, Point(-3,0,0), 4 );
  Shapes<Domain>::addNorm2Ball( someShape, Point( 3,0,0), 4 );
  SAdj surfAdj( true );      // the adjacency
  Container surfContainer( K, someShape, surfAdj );
  MyDigSurf digSurf( surfContainer );
  
  std::cout << "- nb surfels/vertices = " << digSurf.size() << std::endl;

  QApplication application( argc, argv );
  Viewer3D viewer;
  viewer.show(); 
  for ( MyDigSurf::ConstIterator it = digSurf.begin(),
	  itend = digSurf.end(); it != itend; ++it )
    viewer << *it;
  viewer << Viewer3D::updateDisplay;
  return application.exec();
}
