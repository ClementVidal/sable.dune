#ifndef _DUNE_BAKE_DEPENDENCYSOLVER_
#define _DUNE_BAKE_DEPENDENCYSOLVER_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Collection/List.h>
#include <Sable/Core/Collection/Array.h>
#include <Sable/Core/Collection/Map.h>

#include <Dune/Model/Bank/ObjectAccessor.h>

namespace Dune
{

class CHandle;
class CProject;

/**
\ingroup Bake
Dependency solver
*/
class CBakeDependencySolver
{

    DEFINE_MANAGED_CLASS( CBakeDependencySolver )

public:

    /** @name DataTypes */
    //@{
	struct SDepNode;

	struct SDepEdge
	{
		SDepEdge() { From = To = NULL; NextEdge = NULL;}
		SDepNode*	From;
		SDepNode*	To;
		SDepEdge*   NextEdge;
	};

	struct SDepNode
	{
		SDepNode() { Obj = NULL; Visited = FALSE; OutEdges = NULL; }
		Bool				Visited;
		CBankObject*		Obj;
		SDepEdge*			OutEdges;
	};

    //@}

    /** @name Constructor/Destructor*/
    //@{
    CBakeDependencySolver( CBankObjectAccessor& objAccessor );
    virtual ~CBakeDependencySolver();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
	Void Resolve( Sb::CList<CHandle>& list );
    //@}

private:

	// DataTypes
	typedef Sb::CMap<CHandle, SDepNode*> NodeTable;

    // Methods 
	Void		Visit( Sb::CList<CHandle>& list, CBakeDependencySolver::SDepNode& n );
	Void		BuildGraph( const CHandle& sourceObj, NodeTable& mapTable );
	SDepNode*	AddNode();
	SDepEdge*	AddEdge();

	// Attributes
	Sb::CArray<SDepNode>	m_Nodes;
	Sb::CArray<SDepEdge>	m_Edges;
	UInt32					m_EdgeCount;
	UInt32					m_NodeCount;
	CBankObjectAccessor*	m_ObjAccessor;

};


};

#endif
