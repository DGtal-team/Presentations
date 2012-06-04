#include "DGtal/helpers/StdDefs.h"
#include "DGtal/math/arithmetic/LighterSternBrocot.h"

using namespace std;
using namespace DGtal;

int main( int argc, char** argv )
{
  typedef BigInteger Integer;
  typedef LighterSternBrocot<Integer,int64_t> SB;
  typedef SB::Fraction Fraction;
  
  Fraction z( 643, 432 ); // classical instanciation
  SB::display( std::cout, z ); // z=z_3=[1,2,21,10]
  std::cout << std::endl;
  std::cout << "Nb nodes = " << SB::instance().nbFractions 
	    << std::endl; // 6 nodes 
  Fraction z2 = z.previousPartial(); 
  SB::display( std::cout, z2 ); // z_2=[1,2,21]
  std::cout << std::endl;
  std::cout << "Nb nodes = " << SB::instance().nbFractions 
	    << std::endl; // 7 nodes 
  Fraction z1 = z.reduced( 2 );
  SB::display( std::cout, z1 );  // z_1=[1,2]
  std::cout << std::endl;
  z.push_back( make_pair( 12, 4 ) );
  SB::display( std::cout, z ); // z=z_4=[1,2,21,10,12]
  // [Fraction f=7780/5227 u=12 k=4 [1,2,21,10,12] ]
  std::cout << std::endl;
  return 0;
}
