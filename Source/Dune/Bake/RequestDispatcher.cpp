#include <Dune/Bake/RequestDispatcher.h>

#include <Dune/Model/Bank/Bank.h>
#include <Dune/Model/Bank/Object.h>
#include <Dune/Model/Gui/Header.h>
#include <Dune/Model/Gui/Header.h>
#include <Dune/Model/Asset/Header.h>
#include <Dune/Bake/Context/Context.h>
#include <Dune/Bake/DependencySolver.h>
#include <Dune/Model/Project/Project.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CBakeRequestDispatcher, Sb::CManagedObject )

CBakeRequestDispatcher::CBakeRequestDispatcher( CProject& prj )
{
	m_Project = &prj;
}

CBakeRequestDispatcher::~CBakeRequestDispatcher()
{

}

Void CBakeRequestDispatcher::SetContext( CBakeContext& context )
{
    m_Context = &context;
}

Void CBakeRequestDispatcher::StartIteration( Iterator& iterator ) const 
{
	m_RequestList.StartIteration( iterator );
}

CBakeContext& CBakeRequestDispatcher::GetContext()
{
    return *m_Context;
}

/**
Main entry point for request dispatching.

This method is directly called by CBakeContext()
*/
Void CBakeRequestDispatcher::Dispatch( )
{
    DispatchObjects( "CBankObject" );
}

Void CBakeRequestDispatcher::DispatchObjects( String objectType )
{
    Sb::CList<CHandle> list;
    Sb::CList<CHandle>::Iterator it;

    Sb::CString query;
    query.WriteFormattedText( "IsKindOf( %s )", objectType );

    // Grab every textures
    GetProject().IssueQuery( query, list );
	CBakeDependencySolver depsSolver( GetProject() );
	depsSolver.Resolve( list );

	ForEachItem( it, list )
	{
		SBakeRequest r;
		r.Obj = GetProject().GetObj(*it);

		AddRequest( r );
	}
}

CProject& CBakeRequestDispatcher::GetProject() const
{
	return *m_Project;
}

Void CBakeRequestDispatcher::AddRequest( const SBakeRequest& r )
{
    DebugAssert( r.Obj );

	if( IsRequestAlreadyAdded( r.Obj->GetHandle() ) )
		return;

    m_RequestList.AddItemAtEnd( r );
}

/**
Check if a request for a given object already exist in the current request table
*/
Bool CBakeRequestDispatcher::IsRequestAlreadyAdded( const CHandle& r ) const
{
    Iterator it;

    ForEachItem( it, (*this) )
    {   
        if( (*it).Obj->GetHandle() == r )
            return TRUE;
    }

    return FALSE;
}