#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLESYSTEM_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLESYSTEM_

#include <Dune/Editor/View/Editor/Editor.h>

#include <Sable/Graphics/Particle/Operator/Operator.h>
#include <Sable/Input/Common/CameraController.h>

#include <QtGui/QToolBox>
#include <QtGui/QTreeWidget>

namespace Dune
{

class CEditorDisplayWindow;
class CCtrlEditorParticleSystem;

class CPSEmitterPointPanel;
class CPSEmitterRectanglePanel;
class CPSGravityPanel;
class CPSKillOldPanel;
class CPSTargetSizePanel;
class CPSTargetColorPanel;
class CWxFloatInputCtrl;

/**
*/
class CViewParticleSystem : public CViewEditor
{

	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	enum ECtrlID
	{
		nCtrlID_AddOperator = 0,
		nCtrlID_Remove,
		nCtrlID_Rename,
		nCtrlID_SaveAs,
		nCtrlID_Save,
		nCtrlID_Load,
		nCtrlID_MaxParticleCount
	};
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CViewParticleSystem( CApplication& app, const CHandle& ps );
    virtual ~CViewParticleSystem();
    //@}

    /** @name Accessor*/
    //@{
    CEditorDisplayWindow*   GetDisplayWindow() const ;
	CCtrlEditor&	GetCtrl() const;
	Void			SetParticleSystem( const CHandle& h );
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

	// DataType

    // Methods
	QWidget*		SetupEditorWidget( );
	Void		    SetupMenu( );
	Void		    FillOperatorChoices( const Sb::CTypeInfo& type, QStringList& arr );
	Void		    OnAddOperator( const Sb::CTypeInfo& type, Sb::CParticleOperator* pop );

    Void            OnMouseLMBUp( const Sb::CVector2f& pos );
    Void            OnMouseLMBDown( const Sb::CVector2f& pos );
    Void            OnMouseMove( const Sb::CVector2f& pos );

	void			showEvent( QShowEvent * event );

	Q_SLOT		Bool OnUIAddOperator();
	Q_SLOT		Bool OnUIRemoveOperator();
	Q_SLOT		Bool OnUIRenameOperator();
	Q_SLOT		Bool OnUILoad();
	Q_SLOT		Bool OnUISaveAs();
	Q_SLOT		Bool OnUISave();
	Q_SLOT		Bool OnUIMaxParticleCountChanged( int value );

    // Attributes
	Sb::CRef<CCtrlEditorParticleSystem>	m_Ctrl;
	Sb::CFilePath					    m_CurrentFilePath;

	QWidget*			        m_EditorWidget;
	QTreeWidget*		        m_OperatorTree;
	QMenuBar*			        m_MenuBar;
	QSpinBox*			        m_MaxParticleCount;

	CHandle						m_RootHandle;

    CEditorDisplayWindow*	    m_DisplayWindow;

    Sb::CCameraController			m_CamCtrl;
    Sb::CMouseInterface::Signal         m_SignalMouseEventCamCtrl;
    Sb::CKeyboardInterface::Signal      m_SignalKeyboardEventCamCtrl;
};

};

#endif