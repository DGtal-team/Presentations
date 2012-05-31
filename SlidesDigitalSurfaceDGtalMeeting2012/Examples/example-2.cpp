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

  Point p1( 0, 0, 0 ), p2( 1, 1, 1 );
  KSpace K; K.init( p1, p2, true ); // init space
  Domain d( p1, p2 );
  QApplication application( argc, argv );
  Viewer3D viewer;  
  viewer.show();
  viewer << SetMode3D( "HyperRectDomain", "Paving" ) << d;
  viewer << K.uCell( Point( 2, 1, 2 ) )
	 << K.uCell( Point( 1, 2, 2 ) )
	 << K.uCell( Point( 2, 2, 3 ) )
	 << K.uCell( Point( 3, 2, 2 ) )
	 << K.uCell( Point( 2, 2, 1 ) )
    	 << K.uCell( Point( 2, 1, 1 ) )
    	 << K.uCell( Point( 1, 1, 2 ) )
    	 << K.uCell( Point( 1, 2, 3 ) )
    	 << K.uCell( Point( 3, 2, 3 ) )
    	 << K.uCell( Point( 3, 2, 1 ) )
	 << K.uCell( Point( 2, 2, 2 ) );
  viewer<< Viewer3D::updateDisplay;
  return application.exec();
}
