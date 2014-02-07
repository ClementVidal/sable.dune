#include <Dune/Bake/Configuration.h>

using namespace Dune;

SBakeRequest& SBakeRequest::operator=( const SBakeRequest& other ) 
{
    Obj = other.Obj;
    Type = other.Type;
    return *this;
}

SBakeRequest::SBakeRequest()
{
    Type.SetBit( nType_Process );
}

SBakeConfiguration::SBakeConfiguration( )
{
    ThreadCount = 1;
	OptimizeModels = FALSE;
}

SBakeConfiguration::~SBakeConfiguration()
{

}
