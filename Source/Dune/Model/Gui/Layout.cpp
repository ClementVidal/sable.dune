#include <Dune/Model/Gui/Layout.h>

#include <Sable/Gui/Layout/Layout.h>

using namespace Dune;

CPropertyEnum::SEntry CLayout::EnumSizePolicy[] = { 
	{ "Stretchable", nSizePolicy_Stretchable}, 
	{ "Fixed", nSizePolicy_Fixed }, 
	{ NULL, NULL } };

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CLayout, CBankObject )

String CLayout::PropStretchFactor = "StretchFactor";
String CLayout::PropHSizePolicy = "HSizePolicy";
String CLayout::PropVSizePolicy = "VSizePolicy";
String CLayout::PropSize = "Size";

CLayout::CLayout() :
    CBankObject( )
{
	SetName( "Layout" );

	CPropertyInterface* p;

	p = new CPropertyVector2i( PropStretchFactor );
	((CPropertyVector2i*)p)->SetValue( Sb::CVector2i( 1, 1 ) );
	AddProperty( *p );

	p = new CPropertyVector2i( PropSize );
	((CPropertyVector2i*)p)->SetValue( Sb::CVector2i( 0, 0 ) );
	AddProperty( *p );

	p = new CPropertyEnum( PropVSizePolicy, CPropertyEnum::CDef( EnumSizePolicy )  );
	AddProperty( *p );

	p = new CPropertyEnum( PropHSizePolicy, CPropertyEnum::CDef( EnumSizePolicy )  );
	AddProperty( *p );
}

CLayout::~CLayout()
{
}

CPropertyEnum& CLayout::GetHSizePolicy() const
{
	CPropertyEnum* p;
	p = (CPropertyEnum*)GetProperty( PropHSizePolicy );
	DebugAssert( p );
	return *p;
}

CPropertyVector2i& CLayout::GetSize() const
{
	CPropertyVector2i* p;
	p = (CPropertyVector2i*)GetProperty( PropSize );
	DebugAssert( p );
	return *p;
}

CPropertyEnum& CLayout::GetVSizePolicy() const
{
	CPropertyEnum* p;
	p = (CPropertyEnum*)GetProperty( PropVSizePolicy );
	DebugAssert( p );
	return *p;
}

CPropertyVector2i& CLayout::GetStretchFactor() const
{
	CPropertyVector2i* p;
	p = (CPropertyVector2i*)GetProperty( PropStretchFactor );
	DebugAssert( p );
	return *p;
}

Void CLayout::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CLayout* layout = objSet.GetObj<Sb::CLayout>();

    layout->RemoveAllChild();
	layout->SetStretchFactor( GetStretchFactor().GetValue() );
	layout->SetHSizePolicy( (ESizePolicy) GetHSizePolicy().GetValue() );
	layout->SetVSizePolicy( (ESizePolicy) GetVSizePolicy().GetValue() );

    UInt32 i = 0;
    for(i=0;i<GetChildCount();i++)
    {
        CHandle child = GetChildAtIndex( i );
        CBakeObjectSet* childObjSet = context.GetObjectSet( child );
        DebugAssert( childObjSet );

        Sb::CWidget* childWidget = childObjSet->GetObj<Sb::CWidget>();
        if( childWidget )
        {
            layout->AddWidget( *childWidget );
        }
        else 
        {
            Sb::CLayout* childLayout = childObjSet->GetObj<Sb::CLayout>();
            layout->AddLayout( *childLayout );
        }
        
    }

    CBankObject::BakeObjectSet( context, objSet );
}
