#include <Dune\Bake\ObjectSet.h>

using namespace Dune;

CBakeObjectSet::CBakeObjectSet( )
{
	m_NeedInit = FALSE;
    m_Count = 0;
}

CBakeObjectSet::CBakeObjectSet( String name )
{
	m_NeedInit = FALSE;
	m_Name = name;
    m_Table.SetItemCount( 32 );
    m_Count = 0;
}

CBakeObjectSet::CBakeObjectSet( const CBakeObjectSet& other )
{
    *this = other;
}

CBakeObjectSet::~CBakeObjectSet()
{
	Clear();
}

Void CBakeObjectSet::Clear()
{
	m_Count = 0;
	m_Name.SetEmpty();
	m_NeedInit = FALSE;
}

CBakeObjectSet& CBakeObjectSet::operator=( const CBakeObjectSet& other )
{
    m_Table = other.m_Table;
    m_Count = other.m_Count;
    m_Name = other.m_Name;
    return *this;
}

Bool CBakeObjectSet::NeedInit() const
{
	return m_NeedInit;
}

Void CBakeObjectSet::NeedInit( Bool onOff )
{
	m_NeedInit = onOff;
}

Void CBakeObjectSet::AddObject( Sb::CManagedObject& obj )
{
    m_Table[m_Count] = &obj;

    m_Count++;
}

UInt32 CBakeObjectSet::GetObjCount() const
{
    return m_Count;
}

String CBakeObjectSet::GetName() const
{
	return m_Name;
}