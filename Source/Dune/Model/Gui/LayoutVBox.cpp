#include <Dune\Model\Gui\LayoutVBox.h>

#include <Sable\Gui\Layout\VBox.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CLayoutVBox, CLayout )

CLayoutVBox::CLayoutVBox() :
CLayout( )
{
	SetName( "LayoutVBox" );
}

CLayoutVBox::~CLayoutVBox()
{
}

Void CLayoutVBox::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
	Sb::CLayoutVBox* layout = NEWOBJ( Sb::CLayoutVBox, ( ) );
	objSet.AddObject( *layout );

	CLayout::PrepareBakeObjectSet( context, objSet );
}

Void CLayoutVBox::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	CLayout::BakeObjectSet( context, objSet );
}
