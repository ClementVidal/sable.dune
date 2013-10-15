#include <Dune\Bake\Context\Editor.h>

#include <QtCore\QDir>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CBakeContextEditor, Sb::CManagedObject )

CBakeContextEditor::CBakeContextEditor( CProject& project, const SBakeConfiguration& config ) :
	CBakeContext( project, config )
{
	QString output( config.OutputPath.GetPath() );

	QDir dir( output );
	if( !dir.exists( ) )
	{
		QDir().mkpath( output );
	}
}

CBakeContextEditor::~CBakeContextEditor()
{

}

Void CBakeContextEditor::Clear()
{
	m_ObjectSetTable.ClearAllItems();
}

CBakeObjectSet* CBakeContextEditor::BakeObject( const CHandle& h, Bool bakeDependency, Bool forcePrepare )
{
	if( h.IsValid() == FALSE )
		return NULL;

	CBankObject* obj = GetProject().GetObj( h );
	if( !obj )
		return NULL;

	CBakeObjectSet* objectSet = NULL;

	Bool isNewDependency = FALSE;
	Sb::CList<CHandle>	objectToProcess;
	Sb::CList<CHandle>::Iterator it;

	// Grab a list (objectToProcess) of all the dependent object of h
	// and sort the in order of dependency 
	objectToProcess.AddItemAtEnd( h );

	if( bakeDependency )
	{
		CBakeDependencySolver depsSolver( GetProject() );
		depsSolver.Resolve( objectToProcess );
	}

	// Prepare each object of the list
	ForEachItem( it, objectToProcess )
	{
		CBankObject* obj = GetProject().GetObj(*it);

		isNewDependency = !HasObjectSet( (*it ) );
		if( isNewDependency )
		{
			CBakeObjectSet tmpObjectSet( obj->GetName() );
			obj->PrepareBakeObjectSet( *this, tmpObjectSet );
			m_ObjectSetTable.AddItem( (*it), tmpObjectSet );
		}

		objectSet = GetObjectSet( (*it) );

		if( forcePrepare && !isNewDependency )
		{
			objectSet->Clear();
			obj->PrepareBakeObjectSet( *this, *objectSet );
		}
		DebugAssert( objectSet );
	}

	// Bake objects
	ForEachItem( it, objectToProcess )
	{
		CBankObject* obj = GetProject().GetObj(*it);
		objectSet = GetObjectSet( (*it) );
		obj->BakeObjectSet( *this, *objectSet );
	}

	// Initialize objects
	ForEachItem( it, objectToProcess )
	{
		CBankObject* obj = GetProject().GetObj(*it);
		objectSet = GetObjectSet( (*it) );

		if( objectSet->NeedInit() )
		{
			obj->InitializeObjectSet( *this, *objectSet );
			objectSet->NeedInit( FALSE );
		}
	}

	objectSet = GetObjectSet( h );


	return objectSet;
}
