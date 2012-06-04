#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/Color.h"
#include "DGtal/math/arithmetic/IntegerComputer.h"

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

  return 0;
}
