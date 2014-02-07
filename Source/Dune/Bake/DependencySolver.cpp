#include <Dune/Bake/DependencySolver.h>

#include <Dune/Model/Bank/Header.h>
#include <Dune/Model/Project/Project.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CBakeDependencySolver, Sb::CManagedObject )

CBakeDependencySolver::CBakeDependencySolver( CBankObjectAccessor& objAccessor )
{
	m_ObjAccessor = &objAccessor;
	m_EdgeCount = 0; 
	m_NodeCount = 0; 
	m_Nodes.SetItemCount( 1024 ); 
	m_Edges.SetItemCount(1024);
}

CBakeDependencySolver::~CBakeDependencySolver()
{

}

Void CBakeDependencySolver::Visit( Sb::CList<CHandle>& list, CBakeDependencySolver::SDepNode& n )
{
	SDepEdge* e = n.OutEdges;
	if( n.Visited )
		return;

	// Mark the node as visited
	n.Visited = TRUE;
	while( e )
	{
		Visit( list, *e->To );
		e = e->NextEdge;
	}

	list.AddItemAtEnd( n.Obj->GetHandle() );
}

Void CBakeDependencySolver::BuildGraph( const CHandle& srcObjHandle, NodeTable& mapTable )
{
	Sb::CList<CHandle>::Iterator it;
	CBankObject::DependencyList::Iterator itDep;

	if( mapTable.HasItem( srcObjHandle ) || !srcObjHandle.IsValid() )
		return;

	SDepNode* from = AddNode();
	CBankObject* ssrcObj = m_ObjAccessor->GetObj( srcObjHandle );
	from->Obj = ssrcObj;
	mapTable.AddItem( srcObjHandle, from );

	const CBankObject::DependencyList& depList = ssrcObj->GetOutputDependencyList();

	ForEachItem( itDep, depList )
	{
		CHandle& h = (*itDep).Handle;
        CBankObject* obj = m_ObjAccessor->GetObj( h );
		if( obj )
		{
			// If dependency does not exist in the table, add it
			if( mapTable.HasItem( h ) == FALSE )
			{
				BuildGraph( h, mapTable );
			}

            // It's possible that h still does not exist in mapTable
            // for exemple if it's an invalid item
            if( mapTable.HasItem( h ) )
            {
			    SDepNode* to = mapTable.GetItem( h );
			    SDepEdge* e = AddEdge();
			    e->From = from;
			    e->To = to;

			    // If the from edge already has an output, add the new one to the linked list
			    // of output edges
			    if( from->OutEdges )
			    {
				    SDepEdge* tmp = from->OutEdges;
				    from->OutEdges = e;
				    from->OutEdges->NextEdge = tmp;
			    }
			    else
			    {
				    from->OutEdges = e;
			    }
            }
		}
	}
}

Void CBakeDependencySolver::Resolve( Sb::CList<CHandle>& list )
{
	Sb::CList<CHandle>::Iterator it;
	NodeTable mapTable;

	// Add every nodes
	ForEachItem( it, list )
	{
		BuildGraph( (*it), mapTable );
	}

	// Clear the input list before processing
	list.ClearAllItems();

	// Visit all node and fill the output list
	UInt32 i;
	for(i=0;i<m_NodeCount;i++)
	{
		Visit( list, m_Nodes[i] );
	}
}

CBakeDependencySolver::SDepNode* CBakeDependencySolver::AddNode()
{
	DebugAssert( m_NodeCount + 1 < m_Nodes.GetItemCount() );
	return &m_Nodes[m_NodeCount++];
}

CBakeDependencySolver::SDepEdge* CBakeDependencySolver::AddEdge()
{
	DebugAssert( m_NodeCount + 1 < m_Nodes.GetItemCount() );
	return &m_Edges[m_EdgeCount++];
}

