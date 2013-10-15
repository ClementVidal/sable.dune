import LibWorkspace
import LibBuildCode

configList = LibWorkspace.GetWorkspace( "Dune" ).GetProject( "CmdBake" ).GetBuildConfigList()
for config in configList :
    LibBuildCode.CBuilderMSVC( config ).ReBuild( )