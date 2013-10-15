import LibWorkspace
import LibBuildCode

configList = LibWorkspace.GetWorkspace( "Dune" ).GetProject( "Editor" ).GetBuildConfigList()
for config in configList :
    LibBuildCode.CBuilderMSVC( config ).ReBuild( )