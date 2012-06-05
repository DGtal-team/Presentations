#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"
#include "DGtal/io/boards/Board2D.h"
#include "DGtal/io/Color.h"
#include "DGtal/math/arithmetic/IntegerComputer.h"
#include "DGtal/math/arithmetic/LighterSternBrocot.h"
#include "DGtal/math/arithmetic/StandardDSLQ0.h"

int main()
{
  using namespace Z2i;

  Point plow(-2,-2);
  Point pup (17,8);
  Domain domain( plow, pup );
  Board2D board; // for 2D display
  board << domain;
  typedef LighterSternBrocot<int64_t,int32_t> SB;
  typedef SB::Fraction Fraction;
  typedef StandardDSLQ0<Fraction> DSL;
  typedef StandardDSLQ0<Fraction>::ConstIterator ConstIterator;
  DSL D( 7, 16, 0 ); // (a, b, mu)
  board << SetMode( plow.className(), "Paving" );
  board << CustomStyle( plow.className()+"/Paving", 
			new CustomColors( Color(0,0,255),
					  Color(100,100,255) ) );
  for ( ConstIterator it = D.begin( D.U() ),
	  itend = D.end( D.L() ); it != itend; ++it )
    board << *it;
  board << CustomStyle( plow.className()+"/Paving", 
			new CustomColors( Color(255,0,0),
					  Color(255,100,100) ) );
  for ( ConstIterator it = D.begin( D.L() ),
	  itend = D.end( D.U() + D.v() ); it != itend; ++it )
    board << *it;

  board.saveEPS("example-4.eps");
  board.saveSVG("example-4.svg");

  return 0;
}
