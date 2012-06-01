/**
 * @file volMarchingCubes.cpp
 * @ingroup Examples
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2012/02/06
 *
 * An example file named volMarchingCubes.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
//! [volMarchingCubes-basicIncludes]
#include <iostream>
#include <queue>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/sets/SetPredicate.h"
#include "DGtal/kernel/CanonicEmbedder.h"
#include "DGtal/io/readers/VolReader.h"
#include "DGtal/images/ImageSelector.h"
#include "DGtal/images/imagesSetsUtils/SetFromImage.h"
#include "DGtal/images/ImageLinearCellEmbedder.h"
#include "DGtal/shapes/Shapes.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/topology/helpers/Surfaces.h"
#include "DGtal/topology/DigitalSurface.h"
#include "DGtal/topology/SetOfSurfels.h"
//! [volMarchingCubes-basicIncludes]

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;
using namespace Z3i;

typedef SCell MCVertex;
struct MCFace {
  unsigned int nbVertices;
  MCVertex vertices[ 7 ]; // face has at most 7 vertices
};

struct MCConfiguration {
  unsigned int nbFaces;
  MCFace faces[ 4 ]; // configuration has at most 4 faces
};

struct MCTable {
  MCTable( bool interior = true ) : surfAdj( interior ) {}
  SurfelAdjacency<3> surfAdj;
  MCConfiguration configurations[ 256 ]; // 2^8 configurations
};

class CfgPredicate {
public:
  typedef Z3i::Point Point;

  CfgPredicate( unsigned int cfg )
    : myCfg( cfg ) {}
  bool operator()( const Point & p ) const
  {
    return ( 1 << ( p[ 0 ] + 2 * p[ 1 ] + 4 * p[2] ) ) & myCfg;
  }

  unsigned int myCfg;
};

void makeConfiguration( MCConfiguration & config, 
			const SurfelAdjacency<3> & surfAdj,
			unsigned int code )
{
  // std::cerr << "------ Config " << code << " -------------------------"
  // 	    << std::endl;
  KSpace K;
  Point p0( 0, 0, 0 );
  Point p1( 1, 1, 1 );
  K.init( p0, p1, true );
  CfgPredicate cpred( code );
  typedef KSpace::SurfelSet SurfelSet;
  // typedef ImplicitDigitalSurface<KSpace, CfgPredicate> Container;
  typedef SetOfSurfels<KSpace, SurfelSet> Container;
  typedef DigitalSurface<Container> MyDigitalSurface;
  Container aSetOfSurfels( K, surfAdj );
  // Extracts all the surfels
  Surfaces<KSpace>::sMakeBoundary( aSetOfSurfels.surfelSet(),
				   K, cpred, p0, p1 );
  MyDigitalSurface digSurf( aSetOfSurfels );
  // Extract all faces
  typedef MyDigitalSurface::FaceRange FaceRange;
  typedef MyDigitalSurface::FaceSet FaceSet;
  typedef MyDigitalSurface::VertexRange VertexRange;
  FaceSet faces = digSurf.allClosedFaces();
  unsigned int idxFace = 0;
  for ( FaceSet::const_iterator it = faces.begin(),
	  itend = faces.end(); it != itend; ++it )
    {
      // std::cerr << idxFace << ":";
      MCFace & face = config.faces[ idxFace++ ];
      unsigned int idxVertex = 0;
      VertexRange vtcs = digSurf.verticesAroundFace( *it );
      for ( VertexRange::const_iterator itv = vtcs.begin(),
	      itvend = vtcs.end(); itv != itvend; ++itv )
	{
	  // std::cerr << " " << *itv;
	  face.vertices[ idxVertex++ ] = *itv;
	}
      face.nbVertices = idxVertex;
      // std::cerr << std::endl;
    }
  config.nbFaces = idxFace;
}

void makeMCTable( MCTable & table,
		  const SurfelAdjacency<3> & surfAdj )
{
  for ( unsigned int cfg = 0; cfg < 256; ++cfg )
    {
      makeConfiguration( table.configurations[ cfg ], surfAdj, cfg );
    }
  table.surfAdj = surfAdj;
}

///////////////////////////////////////////////////////////////////////////////

void usage( int, char** argv )
{
  std::cerr << "Usage: " << argv[ 0 ] << " <fileName.vol> <minT> <maxT> <Adj>" << std::endl;
  std::cerr << "\t - displays the boundary of the shape stored in vol file <fileName.vol>." << std::endl;
  std::cerr << "\t - voxel v belongs to the shape iff its value I(v) follows minT <= I(v) <= maxT." << std::endl;
  std::cerr << "\t - 0: interior adjacency, 1: exterior adjacency." << std::endl;

}

int main( int argc, char** argv )
{
  if ( argc < 5 )
    {
      usage( argc, argv );
      return 1;
    }
  std::string inputFilename = argv[ 1 ];
  unsigned int minThreshold = atoi( argv[ 2 ] );
  unsigned int maxThreshold = atoi( argv[ 3 ] );
  bool intAdjacency = atoi( argv[ 4 ] ) == 0;

  //! [volMarchingCubes-readVol]
  trace.beginBlock( "Reading vol file into an image." );
  //  typedef ImageSelector < Domain, int>::Type Image;
  typedef ImageContainerBySTLVector< Domain, int> Image;
  Image image = VolReader<Image>::importVol(inputFilename);
  DigitalSet set3d (image.domain());
  SetPredicate<DigitalSet> set3dPredicate( set3d );
  SetFromImage<DigitalSet>::append<Image>(set3d, image,
                                          minThreshold, maxThreshold);
  trace.endBlock();
  //! [volMarchingCubes-readVol]

  trace.beginBlock( "Constructing table." );
  MCTable aTable;
  makeMCTable( aTable, SurfelAdjacency<3>( intAdjacency ) );
  trace.endBlock();
  
  //! [volMarchingCubes-KSpace]
  trace.beginBlock( "Construct the Khalimsky space from the image domain." );
  KSpace ks;
  bool space_ok = ks.init( image.domain().lowerBound(),
			   image.domain().upperBound(), true );
  if (!space_ok)
    {
      trace.error() << "Error in the Khamisky space construction."<<std::endl;
      return 2;
    }
  trace.endBlock();
  //! [volMarchingCubes-KSpace]

  typedef CanonicEmbedder< Space > MyEmbedder;
  typedef
     ImageLinearCellEmbedder< KSpace, Image, MyEmbedder > CellEmbedder;
  CellEmbedder cellEmbedder;
  MyEmbedder trivialEmbedder;
  cellEmbedder.init( ks, image, trivialEmbedder, minThreshold );

  typedef DGtal::uint64_t Number;
  trace.beginBlock( "Extracting and numbering vertices." );
  // Numbers all vertices.
  std::map<SCell, Number> index;
  typedef KSpace::SurfelSet SurfelSet;
  SurfelSet theSurfels;
  Point pLow = image.domain().lowerBound();
  Point pUp = image.domain().upperBound();
  Point pUpm1 = pUp - Point::diagonal( 1 );
  Surfaces<KSpace>::sMakeBoundary( theSurfels, ks,
				   set3dPredicate, pLow, pUp );
  Number nbv = 0;
  for ( SurfelSet::const_iterator it = theSurfels.begin(), it_end = theSurfels.end();
        it != it_end; ++it )
    {
      index[ *it ] = nbv++;
    }
  trace.endBlock();

  trace.beginBlock( "Counting faces." );
  const Point dx( 1, 0, 0 );
  const Point dy( 0, 1, 0 );
  const Point dz( 0, 0, 1 );
  const Point dxy( 1, 1, 0 );
  const Point dxz( 1, 0, 1 );
  const Point dyz( 0, 1, 1 );
  const Point dxyz( 1, 1, 1 );
  Number nbf = 0;
  for ( unsigned int z = pLow[ 2 ]; z < pUp[ 2 ]; ++z )
    for ( unsigned int y = pLow[ 1 ]; y < pUp[ 1 ]; ++y )
      for ( unsigned int x = pLow[ 0 ]; x < pUp[ 0 ]; ++x )
	{
	  Point p( x, y, z );
	  unsigned int cfg = ( set3dPredicate( p ) ? 1 : 0 )
	    + ( set3dPredicate( p + dx ) ? 2 : 0 )
	    + ( set3dPredicate( p + dy ) ? 4 : 0 )
	    + ( set3dPredicate( p + dxy ) ? 8 : 0 )
	    + ( set3dPredicate( p + dz ) ? 16 : 0 )
	    + ( set3dPredicate( p + dxz ) ? 32 : 0 )
	    + ( set3dPredicate( p + dyz ) ? 64 : 0 )
	    + ( set3dPredicate( p + dxyz ) ? 128 : 0 );
	  nbf += aTable.configurations[ cfg ].nbFaces;
	}
  trace.endBlock();

  trace.beginBlock( "Generating OFF vertices." );
  // Outputs OFF header.
  Number nbe = 0;
  std::cout << "OFF" << std::endl
	    << "# Generated by DGtal::DigitalSurface." << std::endl
	    << nbv << " " << nbf << " " << ( nbe / 2 ) << std::endl;
  typedef CellEmbedder::RealPoint RealPoint;
  for ( SurfelSet::const_iterator it = theSurfels.begin(), it_end = theSurfels.end();
        it != it_end; ++it )
    {
      RealPoint p( cellEmbedder( ks.unsigns( *it ) ) );
      std::cout << p[ 0 ] << " " << p[ 1 ] << " " << p[ 2 ] << std::endl;
    }
  trace.endBlock();

  trace.beginBlock( "Generating OFF faces." );
  for ( unsigned int z = pLow[ 2 ]; z < pUp[ 2 ]; ++z )
    for ( unsigned int y = pLow[ 1 ]; y < pUp[ 1 ]; ++y )
      for ( unsigned int x = pLow[ 0 ]; x < pUp[ 0 ]; ++x )
	{
	  Point p( x, y, z );
	  unsigned int cfg = ( set3dPredicate( p ) ? 1 : 0 )
	    + ( set3dPredicate( p + dx ) ? 2 : 0 )
	    + ( set3dPredicate( p + dy ) ? 4 : 0 )
	    + ( set3dPredicate( p + dxy ) ? 8 : 0 )
	    + ( set3dPredicate( p + dz ) ? 16 : 0 )
	    + ( set3dPredicate( p + dxz ) ? 32 : 0 )
	    + ( set3dPredicate( p + dyz ) ? 64 : 0 )
	    + ( set3dPredicate( p + dxyz ) ? 128 : 0 );
	  unsigned int nbfaces = aTable.configurations[ cfg ].nbFaces;
	  for ( unsigned int f = 0; f < nbfaces; ++f )
	    {
	      const MCFace & face = aTable.configurations[ cfg ].faces[ f ];
	      for ( unsigned int v = 0; v < face.nbVertices; ++v )
		{
		  SCell shiftedCell = ks.sTranslation( face.vertices[ v ], p );
		  std::cout << index[ shiftedCell ] << " ";
		}
	      std::cout << std::endl;
	    }
	}
  trace.endBlock();

  // //! [volMarchingCubes-SurfelAdjacency]
  // typedef SurfelAdjacency<KSpace::dimension> MySurfelAdjacency;
  // MySurfelAdjacency surfAdj( intAdjacency ); // interior in all directions.
  // //! [volMarchingCubes-SurfelAdjacency]

  // //! [volMarchingCubes-ExtractingSurface]
  // trace.beginBlock( "Extracting boundary by scanning the space. " );
  // typedef KSpace::SurfelSet SurfelSet;
  // typedef SetOfSurfels< KSpace, SurfelSet > MySetOfSurfels;
  // typedef DigitalSurface< MySetOfSurfels > MyDigitalSurface;
  // MySetOfSurfels theSetOfSurfels( ks, surfAdj );
  // Surfaces<KSpace>::sMakeBoundary( theSetOfSurfels.surfelSet(),
  //                                  ks, set3dPredicate,
  //                                  image.domain().lowerBound(),
  //                                  image.domain().upperBound() );
  // MyDigitalSurface digSurf( theSetOfSurfels );
  // trace.info() << "Digital surface has " << digSurf.size() << " surfels."
  //              << std::endl;
  // trace.endBlock();
  // //! [volMarchingCubes-ExtractingSurface]

  // //! [volMarchingCubes-makingOFF]
  // trace.beginBlock( "Making OFF surface <marching-cube.off>. " );
  // typedef CanonicEmbedder< Space > MyEmbedder;
  // typedef
  //   ImageLinearCellEmbedder< KSpace, Image, MyEmbedder > CellEmbedder;
  // CellEmbedder cellEmbedder;
  // MyEmbedder trivialEmbedder;
  // cellEmbedder.init( ks, image, trivialEmbedder, minThreshold );
  // ofstream out( "marching-cube.off" );
  // if ( out.good() )
  //   digSurf.exportEmbeddedSurfaceAs3DOFF( out, cellEmbedder );
  // out.close();
  // trace.endBlock();
  // //! [volMarchingCubes-makingOFF]
  return 0;
}

