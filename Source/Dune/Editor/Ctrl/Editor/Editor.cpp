#include <Dune/Editor/Ctrl/Editor/Editor.h>

#include <Sable/Graphics/Scene/View.h>

using namespace Dune;

CCtrlEditor::CCtrlEditor( CViewEditor& view ) :
    CCtrl( view.GetApp() ),
    m_View( view )
{
	m_BakeConfiguration.OptimizeModels = FALSE;
	m_BakeConfiguration.OutputPath = Sb::CFilePath( GetApp().GetSettings().GetInternalBakePath() );
	m_BakeContext = NEWOBJ( CBakeContextEditor, ( *GetProject(), m_BakeConfiguration ) );
}

CCtrlEditor::~CCtrlEditor()
{
}

CViewEditor& CCtrlEditor::GetView()
{
    return m_View;
}

CBakeContextEditor&	CCtrlEditor::GetBakeContext()
{
	return *m_BakeContext;
}

Void CCtrlEditor::OnElementSelected( const CHandle& elem )
{

}

Void CCtrlEditor::OnElementDeSelected( const CHandle& elem )
{

}

CBakeObjectSet* CCtrlEditor::GetBakeObjectSet( CHandle h ) const
{
	return m_BakeContext->GetObjectSet( h );
}

Void CCtrlEditor::OnAddObject( const CHandle& parent, const CHandle& h )
{
	GetBakeContext().BakeObject( h, FALSE, FALSE );

	CCtrl::OnAddObject( parent, h );
}

Void CCtrlEditor::OnRemoveObject( const CHandle& h )
{
	GetBakeContext().BakeObject( GetRootObject(), TRUE, FALSE );

	CCtrl::OnRemoveObject( h );
}

Void CCtrlEditor::OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue )
{
	if( prop.GetTypeInfo().IsKindOf( CPropertyHandle::GetStaticTypeInfo() ) )
	{
		const CPropertyHandle& pHandle = (const CPropertyHandle&) prop;
		m_BakeContext->BakeObject( pHandle.GetValue(), TRUE, FALSE );
	}
	m_BakeContext->BakeObject( prop.GetObj()->GetHandle(), FALSE, FALSE );
}

Void CCtrlEditor::OnChildIndexChanged( const CHandle& objtect, Int32 oldPos, Int32 newPos )
{
	m_BakeContext->BakeObject( objtect, FALSE, FALSE );
}