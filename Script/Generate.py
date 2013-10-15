import LibWorkspace
import LibGenerateCode
import LibUtils

context = LibWorkspace.CContext()

w = context.GetWorkspace( "Dune" )
LibGenerateCode.GenerateQtMocFile( w )
LibGenerateCode.GenerateHeader( w )
LibGenerateCode.GenerateBuildFile( w )

LibUtils.StringifyFile( 'S:/Dune/Source/Dune/Common/Qt/QtStyleSheet.qss', 'S:/Dune/Source/Dune/Common/Qt/QtStyleSheet.inc' )
LibUtils.StringifyFile( 'S:/Dune/Source/Dune/Resources/Mesh/Arrow.obj', 'S:/Dune/Source/Dune/Resources/Mesh/Arrow.inc' )
LibUtils.StringifyFile( 'S:/Dune/Source/Dune/Resources/Mesh/Camera.obj', 'S:/Dune/Source/Dune/Resources/Mesh/Camera.inc' )
