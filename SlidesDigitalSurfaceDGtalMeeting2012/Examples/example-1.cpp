#include <QImageReader>
#include <QtGui/qapplication.h>
#include "DGtal/io/viewers/Viewer3D.h"

#include "DGtal/helpers/StdDefs.h"
#include "DGtal/topology/SurfelAdjacency.h"
#include "DGtal/topology/DigitalSetBoundary.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/shapes/Shapes.h"

template <typename MyDigSurf>
int
displaySurface( QApplication & application, MyDigSurf digSurf )
{
  typedef Z3i::Point Point; // 3D digital point
  Viewer3D viewer;
  viewer.show(); 
  for ( typename MyDigSurf::ConstIterator it = digSurf.begin(),
	  itend = digSurf.end(); it != itend; ++it )
    viewer << *it;
  viewer << Viewer3D::updateDisplay;
  return application.exec();
}

template <typename MyDigSurf>
int
displayGraph( QApplication & application, MyDigSurf digSurf )
{
  typedef typename MyDigSurf::KSpace KSpace;
  typedef typename KSpace::Point Point;
  typedef typename MyDigSurf::Vertex Vertex;
  typedef typename MyDigSurf::ConstIterator ConstIterator;
  const KSpace & K = digSurf.container().space();
  Viewer3D viewer;
  viewer.show(); 
  typedef std::vector<Vertex> Neighborhood;
  for ( ConstIterator it = digSurf.begin(),
	  itend = digSurf.end(); it != itend; ++it )
    {
      Neighborhood N;
      std::back_insert_iterator<Neighborhood> itN = std::back_inserter( N );
      digSurf.writeNeighbors( itN , *it );
      Point p = K.sKCoords( *it );
      for ( unsigned int i = 0; i < N.size(); ++i )
	{
	  Point q = K.sKCoords( N[ i ] );
	  viewer.addLine ( p[0]/2.0, p[1]/2.0, p[2]/2.0,
			   q[0]/2.0, q[1]/2.0, q[2]/2.0, 
			   DGtal::Color ( 200,20,20 ), 2.0 );
	}
    }
  viewer << Viewer3D::updateDisplay;
  return application.exec();
}

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
  displaySurface( application, digSurf );
  displayGraph( application, digSurf );
  return 0;
}
