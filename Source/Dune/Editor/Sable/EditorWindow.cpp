#include <Dune/Editor/Sable/EditorWindow.h>

using namespace Dune;

CSableEditorWindow::CSableEditorWindow( ) :
    Sb::CApplicationWindow( )
{
    m_Handle = 0;
}

CSableEditorWindow::~CSableEditorWindow()
{
}

UInt32 CSableEditorWindow::GetHandle() const
{
    return m_Handle;
}

Bool CSableEditorWindow::Initialize( const Sb::CSwapChainInfo& swapChainDesc, UInt32 handle )
{
    m_Handle = handle;
    return Sb::CApplicationWindow::Initialize( swapChainDesc );
}