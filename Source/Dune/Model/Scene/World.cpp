#include <Dune/Model/Scene/World.h>

#include <Sable/Graphics/Scene/World.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CSceneWorld, CBankObject )

Dune::CSceneWorld::CSceneWorld( ) :
    CBankObject( )
{
    SetName("World");
}

Dune::CSceneWorld::~CSceneWorld()
{
}

Void Dune::CSceneWorld::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
    Sb::CSceneWorld* world = NEWOBJ( Sb::CSceneWorld, ( ) );
    objSet.AddObject( *world );

    CBankObject::PrepareBakeObjectSet( context, objSet );
}

Void Dune::CSceneWorld::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CSceneWorld* world = objSet.GetObj<Sb::CSceneWorld>();
    DebugAssert( world );

    // Attach childs
    UInt32 i;
    for( i=0;i<GetChildCount();i++ )
    {
        CBakeObjectSet* set = context.GetObjectSet( GetChildAtIndex( i ) );
        DebugAssert( set );
        Sb::CNodeSpatial* node = set->GetObj<Sb::CNodeSpatial>(0);
        if( node )
        {
            node->SetParent( &world->GetRootNode() );
        }
    }


    CBankObject::BakeObjectSet( context, objSet );
}