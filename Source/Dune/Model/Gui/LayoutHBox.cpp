#include <Dune/Model/Gui/LayoutHBox.h>

#include <Sable/Gui/Layout/HBox.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CLayoutHBox, CLayout )

String CLayoutHBox::PropStretchFactor = "StretchFactor";

CLayoutHBox::CLayoutHBox() :
    CLayout( )
{
	SetName( "LayoutHBox" );
}

CLayoutHBox::~CLayoutHBox()
{
}

Void CLayoutHBox::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
    Sb::CLayoutHBox* layout = NEWOBJ( Sb::CLayoutHBox, ( ) );
    objSet.AddObject( *layout );

    CLayout::PrepareBakeObjectSet( context, objSet );
}

Void CLayoutHBox::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    CLayout::BakeObjectSet( context, objSet );
}
