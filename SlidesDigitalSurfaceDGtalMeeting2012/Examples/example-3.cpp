/**
 * @file frontierAndBoundary.cpp
 * @ingroup Examples
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2012/02/06
 *
 * An example file named qglViewer.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
//! [frontierAndBoundary-basicIncludes]
#include <iostream>
#include <QtGui/qapplication.h>
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/kernel/sets/SetPredicate.h"
#include "DGtal/io/readers/VolReader.h"
#include "DGtal/io/viewers/Viewer3D.h"
#include "DGtal/io/DrawWithDisplay3DModifier.h"
#include "DGtal/io/Color.h"
#include "DGtal/io/colormaps/HueShadeColorMap.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/shapes/implicit/ImplicitBall.h"
#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/shapes/Shapes.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/topology/ExplicitDigitalSurface.h"
#include "DGtal/topology/helpers/FrontierPredicate.h"
#include "DGtal/topology/helpers/BoundaryPredicate.h"
//! [frontierAndBoundary-basicIncludes]

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////

template <typename Image, typename Domain>
void fill( Image & I, const Domain & d, unsigned int label )
{
  typedef typename Domain::ConstIterator CIter;
  for ( CIter it = d.begin(), itend = d.end(); it != itend; ++it )
    I.setValue( *it, label );
}

int main( int argc, char** argv )
{
  using namespace Z3i;

  //! [frontierAndBoundary-LabelledImage]
  typedef ImplicitBall<Space> EuclideanShape;
  typedef GaussDigitizer<Space,EuclideanShape> DigitalShape; 
  typedef ImageContainerBySTLVector<Domain,DGtal::uint8_t> Image;
  Domain domain0( Point( 0,0,0 ), Point( 10, 10, 10 ) ); 
  Domain domain1( Point( 2,2,2 ), Point( 8, 8, 8 ) ); 
  Domain domain2( Point( 2,4,4 ), Point( 8, 6, 6 ) ); 
  Image image( domain0 );
  fill( image, domain0, 0 );
  fill( image, domain1, 1 );
  fill( image, domain2, 2 );
  //! [frontierAndBoundary-LabelledImage]
  
  //! [frontierAndBoundary-KSpace]
  trace.beginBlock( "Construct the Khalimsky space from the image domain." );
  KSpace K;
  bool space_ok = K.init( domain0.lowerBound(), domain0.upperBound(), true );
  if (!space_ok)
    {
      trace.error() << "Error in the Khamisky space construction."<<std::endl;
      return 2;
    }
  trace.endBlock();
  //! [frontierAndBoundary-KSpace]

  //! [frontierAndBoundary-SetUpDigitalSurface]
  trace.beginBlock( "Set up digital surface." );
  typedef SurfelAdjacency<KSpace::dimension> MySurfelAdjacency;
  MySurfelAdjacency surfAdj( true ); // interior in all directions.
  typedef FrontierPredicate<KSpace, Image> FSurfelPredicate;
  typedef ExplicitDigitalSurface<KSpace,FSurfelPredicate> FrontierContainer;
  typedef DigitalSurface<FrontierContainer> Frontier;
  // frontier between label 1 and 0 (connected part containing bel10)
  SCell vox1  = K.sSpel( Point( 2,2,2 ), K.POS );
  SCell bel10 = K.sIncident( vox1, 0, false );
  FSurfelPredicate surfPredicate10( K, image, 1, 0 );
  Frontier frontier10 =
    new FrontierContainer( K, surfPredicate10, surfAdj, bel10 );
  // frontier between label 2 and 0 (connected part containing bel20)
  SCell vox2  = K.sSpel( Point( 2,4,4 ), K.POS );
  SCell bel20 = K.sIncident( vox2, 0, false );
  FSurfelPredicate surfPredicate20( K, image, 2, 0 );
  Frontier frontier20 =
    new FrontierContainer( K, surfPredicate20, surfAdj, bel20 );
  // frontier between label 2 and 0 (connected part containing bel20bis)
  SCell vox2bis  = K.sSpel( Point( 8,6,6 ), K.POS );
  SCell bel20bis = K.sIncident( vox2bis, 0, true );
  FSurfelPredicate surfPredicate20bis( K, image, 2, 0 );
  Frontier frontier20bis =
    new FrontierContainer( K, surfPredicate20bis, surfAdj, bel20bis );
  trace.endBlock();
  // frontier between label 2 and 0 (connected part containing bel20)
  SCell bel21 = K.sIncident( vox2, 1, false );
  FSurfelPredicate surfPredicate21( K, image, 2, 1 );
  Frontier frontier21 =
    new FrontierContainer( K, surfPredicate21, surfAdj, bel21 );
  //! [frontierAndBoundary-SetUpDigitalSurface]

  //! [volBreadthFirstTraversal-DisplayingSurface]
  trace.beginBlock( "Displaying surface in Viewer3D." );
  QApplication application(argc,argv);
  Viewer3D viewer;
  viewer.show(); 
  viewer << SetMode3D( domain0.className(), "BoundingBox" )
         << domain0;
  Cell dummy;
  // Display frontier between 1 and 0.
  unsigned int nbSurfels10 = 0;
  viewer << CustomColors3D( Color::Red, Color::Red );
  for ( Frontier::ConstIterator 
          it = frontier10.begin(), it_end = frontier10.end();
        it != it_end; ++it, ++nbSurfels10 )
    viewer << *it; 
  // Display frontier between 2 and 0.
  unsigned int nbSurfels20 = 0;
  viewer << CustomColors3D( Color::Yellow, Color::Yellow );
  for ( Frontier::ConstIterator 
          it = frontier20.begin(), it_end = frontier20.end();
        it != it_end; ++it, ++nbSurfels20 )
    viewer << *it;
  // Display frontier between 2 and 0.
  unsigned int nbSurfels20bis = 0;
  viewer << CustomColors3D( Color::Magenta, Color::Magenta );
  for ( Frontier::ConstIterator 
          it = frontier20bis.begin(), it_end = frontier20bis.end();
        it != it_end; ++it, ++nbSurfels20bis )
    viewer << *it;
  // Display frontier between 2 and 1.
  unsigned int nbSurfels21 = 0;
  viewer << CustomColors3D( Color::Cyan, Color::Cyan );
  for ( Frontier::ConstIterator 
          it = frontier21.begin(), it_end = frontier21.end();
        it != it_end; ++it, ++nbSurfels21 )
    viewer << *it;
  trace.info() << "nbSurfels10 = " << nbSurfels10
               << ", nbSurfels20 = " << nbSurfels20
               << ", nbSurfels20bis = " << nbSurfels20bis
               << ", nbSurfels21 = " << nbSurfels21 << std::endl;
  viewer << Viewer3D::updateDisplay;
  trace.endBlock();
  return application.exec();
  //! [volBreadthFirstTraversal-DisplayingSurface]
}
