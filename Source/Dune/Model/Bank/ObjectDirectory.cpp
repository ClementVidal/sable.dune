#include <Dune/Model/Bank/ObjectDirectory.h>

#include <Dune/Model/Project/Project.h>

#include <Sable/Audio/Buffer/Header.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CBankObjectDirectory, CBankObject )

CBankObjectDirectory::CBankObjectDirectory( ) :
    CBankObject()
{
    SetName("Directory");
}

CBankObjectDirectory::~CBankObjectDirectory()
{
}
