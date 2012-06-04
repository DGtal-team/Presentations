#include "DGtal/helpers/StdDefs.h"
#include "DGtal/math/arithmetic/IntegerComputer.h"

int main( int argc, char** argv )
{
  typedef DGtal::BigInteger Integer;
  IntegerComputer<Integer> ic; // instance for computations
  Integer g = ic.gcd( 192, 128 ); // 64
  std::cout << "gcd(192,128)=" << g << std::endl;
  IntegerComputer<Integer>::Vector2I v
    = ic.extendedEuclid( 5, 12, 2 ); // solution to 5x+12y = 2
  std::cout << "5x+12y=2 <=> " << "x=" << v[0]
	    << " y=" << v[1] << std::endl;
  std::vector<Integer> q; // quotients
  ic.getCFrac( q, 5, 13 );
  std::cout << "5/13=[" << q[0] << ";" << q[1] 
	    << "," << q[2] << "," << q[3]
	    << "," << q[4] << "]" << std::endl;
  q.clear();
  ic.getCFrac( q, Integer("51234567894643563456345635435722900123"), 
	       Integer("345678532087609239457759428901234" ) );
  std::cout << "[" << q[0];
  for ( unsigned int i = 1; i < q.size(); ++i )
    std::cout << "," << q[i];
  std::cout << "]" << std::endl;
  return 0;
}
