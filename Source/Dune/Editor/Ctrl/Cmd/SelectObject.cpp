#include <Dune/Editor/Ctrl/Cmd/SelectObject.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdSelectObject, CCmd );

using namespace Dune;


CCmdSelectObject::CCmdSelectObject( CCtrlProject& ctrl,  EAction action )
{
    m_Ctrl = &ctrl;

    const CCtrlProject::ObjectList& objList = ctrl.GetSelectedObjects();
    m_SelectionSet.SetItemCount(objList.GetItemCount() );

    CCtrlProject::ObjectList::Iterator iterator;
    Index index = 0;
    ForEachItem( iterator, objList )
    {
        m_SelectionSet[ index ] = iterator.GetData();
        index++;
    }

    m_Action = action;
}

CCmdSelectObject::CCmdSelectObject( CCtrlProject& ctrl, const CHandle& newObj, EAction action )
{
	m_Ctrl = &ctrl;
	m_SelectionSet.SetItemCount(1);
	m_SelectionSet[0] = newObj;
	m_Action = action;
}

CCmdSelectObject::CCmdSelectObject( CCtrlProject& ctrl, const CHandle* newObjs, UInt32 count, EAction action )
{
	UInt32 i;
	m_Ctrl = &ctrl;
	m_SelectionSet.SetItemCount(count);
	for(i=0;i<count;i++)
		m_SelectionSet[i] = newObjs[i];
	m_Action = action;
}

CCmdSelectObject::~CCmdSelectObject()
{
}

Bool CCmdSelectObject::Undo()
{
	if( m_SelectionSet.GetItemCount() )
	{
		if( m_Action == nAction_Select )
        {
			m_Ctrl->SelectObjects( m_SelectionSet, FALSE );
        }
		else if( m_Action == nAction_DeSelect )
        {
			m_Ctrl->SelectObjects( m_SelectionSet, TRUE );
        }
		else if( m_Action == nAction_Toggle )
		{
			UInt32 i;
			for( i=0;i<m_SelectionSet.GetItemCount();i++ )
			{
				m_Ctrl->SelectObjects( m_SelectionSet[i], !m_Ctrl->IsObjectSelected( m_SelectionSet[i] ) );
			}
        }        
        else
        {
            return FALSE;
        }

	    return TRUE;
	}

    return FALSE;
}

Bool CCmdSelectObject::Do()
{
	if( m_SelectionSet.GetItemCount() )
	{
		if( m_Action == nAction_Select )
		{
			m_Ctrl->SelectObjects( m_SelectionSet, TRUE );
		}
		else if( m_Action == nAction_DeSelect )
		{
			m_Ctrl->SelectObjects( m_SelectionSet, FALSE );
		}
		else if( m_Action == nAction_Toggle )
		{
			UInt32 i;
			for( i=0;i<m_SelectionSet.GetItemCount();i++ )
			{
				m_Ctrl->SelectObjects( m_SelectionSet[i], !m_Ctrl->IsObjectSelected( m_SelectionSet[i] ) );
			}
		}
        else
        {
            return FALSE;
        }

	    return TRUE;
	}

	return TRUE;
}

