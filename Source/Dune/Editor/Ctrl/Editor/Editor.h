#ifndef _DUNE_EDITOR_CTRL_EDITOR_EDITOR_
#define _DUNE_EDITOR_CTRL_EDITOR_EDITOR_

#include <Sable\Core\Event\Header.h>

#include <Dune\Bake\Context\Editor.h>
#include <Dune\Bake\Configuration.h>

#include <Dune\Editor\Ctrl\Ctrl.h>


namespace Sable
{
}

namespace Dune
{

class CViewEditor;
class CBakeObjectSet;
class CSableEditorRenderer;

/** 
Base class for editor controler
*/
class CCtrlEditor : public CCtrl
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CCtrlEditor( CViewEditor& view );
    ~CCtrlEditor();
    //@}

    /** @name Accessor*/
    //@{
	virtual const CHandle&  			GetRootObject() const = 0;
    //@}

    /** @name Manipulator*/
    //@{
	CBakeContextEditor&			GetBakeContext();
    CViewEditor&                GetView();
    CBakeObjectSet*             GetBakeObjectSet( CHandle h ) const;
	//@}

    /** @name Signal*/
    //@{
    //@}
 
protected:

	// Methods
	virtual Void		OnElementSelected( const CHandle& elem );
	virtual Void		OnElementDeSelected( const CHandle& elem );
	virtual Void		OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue );
	virtual Void		OnRemoveObject( const CHandle& h );
	virtual Void		OnAddObject( const CHandle& parent, const CHandle& h );
	virtual Void		OnChildIndexChanged( const CHandle& objtect, Int32 oldPos, Int32 newPos );

	// Attributes

private:

    // Methods

    // Attributes
    CViewEditor&									m_View;
	Sb::CRef<CBakeContextEditor>					m_BakeContext;
	SBakeConfiguration								m_BakeConfiguration;

	Sb::CEventSlot<CCtrlEditor, Int32>                  m_FrameChangeSlot;
	Sb::CEventSlot<CCtrlEditor, const CHandle&>			m_SlotElementSelected;
	Sb::CEventSlot<CCtrlEditor, const CHandle&>			m_SlotElementDeSelected;
};

}

#endif
