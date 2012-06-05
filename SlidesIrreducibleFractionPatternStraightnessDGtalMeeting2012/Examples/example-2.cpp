#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/Color.h"
#include "DGtal/math/arithmetic/IntegerComputer.h"
#include "DGtal/math/arithmetic/LighterSternBrocot.h"

int main()
{
  using namespace Z2i;

  Point plow(0,0);
  Point pup (13,5);
  Domain domain( plow, pup );
  Board2D board; // for 2D display
  board << domain;
  // board.setPenColor( DGtal::Color::Blue );
  // board << CustomStyle( plow.className(),
  // 			new CustomPen( Color(0,0,255),
  // 				       Color(0,0,255),
  // 				       2.0,
  // 				       Board2D::Shape::SolidStyle ) );
  board.setPenColor( Color(0,0,255) );
  board.setFillColor( Color::None );
  board.setLineWidth( 2.0 );
  board.setLineStyle( Board2D::Shape::SolidStyle );
  //board << Point( 13, 5 );
  draw( board, Point( 13, 5 ), Point( 0, 0 ) );
  board.setPenColor( Color(255,0,0) );
  board.setLineWidth( 1.0 );
  draw( board, Point( 5, 2 ), Point( 0, 0 ) );
  draw( board, Point( 2, 1 ), Point( 0, 0 ) );
  draw( board, Point( 0, 1 ), Point( 0, 0 ) );
  board.setPenColor( Color(0,255,0) );
  draw( board, Point( 3, 1 ), Point( 0, 0 ) );
  draw( board, Point( 1, 0 ), Point( 0, 0 ) );
  
  board.saveEPS("example-2.eps");
  board.saveSVG("example-2.svg");

  //-----------------------------------------------------------------------------
  typedef LighterSternBrocot<int64_t,int32_t> SB;
  typedef SB::Fraction Fraction;
  Fraction z( 5, 12 ); // classical instanciation
  board.clear();
  board << domain;
  board.setPenColor( Color(0,0,255) );
  board.setFillColor( Color::None );
  board.setLineWidth( 2.0 );
  board.setLineStyle( Board2D::Shape::SolidStyle );
  draw( board, Point( z.q(), z.p() ), Point( 0, 0 ) );
  
  
  Fraction z1, z2;
  int32_t n1, n2;
  z.getSplit( z1, z2 );
  board.setLineWidth( 1.0 );
  board.setPenColor( Color(0,255,0) );
  draw( board, Point( z1.q(), z1.p() ), Point( 0, 0 ) );
  board.setPenColor( Color(255,0,0) );
  draw( board, Point( z2.q(), z2.p() ), Point( z1.q(), z1.p() ) );
  board.saveEPS("example-2bis.eps");
  board.saveSVG("example-2bis.svg");

  //-----------------------------------------------------------------------------
  board.clear();
  board << domain;
  board.setPenColor( Color(0,0,255) );
  board.setFillColor( Color::None );
  board.setLineWidth( 2.0 );
  board.setLineStyle( Board2D::Shape::SolidStyle );
  draw( board, Point( z.q(), z.p() ), Point( 0, 0 ) );

  z.getSplitBerstel( z1, n1, z2, n2 );
  board.setPenColor( Color(0,255,0) );
  board.setLineWidth( 1.0 );
  Point p( 0, 0 );
  Point q( 0, 0 );
  for ( unsigned int i = 0; i < n1; ++i )
    {
      q += Vector( z1.q(), z1.p() );
      draw( board, q - p, p );
      p = q;
    }
  board.setPenColor( Color(255,0,0) );
  for ( unsigned int i = 0; i < n2; ++i )
    {
      q += Vector( z2.q(), z2.p() );
      draw( board, q - p, p );
      p = q;
    }
  board.saveEPS("example-2ter.eps");
  board.saveSVG("example-2ter.svg");



  return 0;
}
