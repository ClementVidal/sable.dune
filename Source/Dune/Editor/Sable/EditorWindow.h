#ifndef _DUNE_EDITOR_EDITOR_SABLE_EDITORWINDOW_
#define _DUNE_EDITOR_EDITOR_SABLE_EDITORWINDOW_

#include <Sable/Core/Common/DataTypes.h>

#include <Sable/Application/Window.h>

namespace Dune
{

class CSableEditorRenderer;

class CSableEditorWindow : public Sb::CApplicationWindow
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CSableEditorWindow();
    ~CSableEditorWindow();
    //@}
    
    /** @name Operator*/
    //@{
    //@}
    
    /** @name Accessors*/
    //@{
    UInt32    GetHandle() const;
    //@}
    
    /** @name Manipulator*/
    //@{
    Bool    Initialize( const Sb::CSwapChainInfo& swapChainDesc, UInt32 handle );
    //@}
    
private:

    // Methods
    
    // Attributes
    UInt32 m_Handle;

};

}

#endif
