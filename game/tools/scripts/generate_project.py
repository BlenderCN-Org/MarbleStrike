import os
import sys
from stat import *
from shutil import *
from xml.etree import ElementTree
from xml.dom import minidom

#################################################################################
# Main
#################################################################################

def LoadXMLConfig( filename ):
    tree = ElementTree.parse( filename )
    root = tree.getroot()
    
    return root;    

def PrettifyXML( root ):    
    rough_string = ElementTree.tostring( root, 'utf-8' )
    reparsed = minidom.parseString( rough_string )
    return reparsed.toprettyxml( indent = "\t" )
    
def GenerateProjectConfiguration( root, type, configData ):

    for platformType in configData.iter( type ):
        project_configuration = ElementTree.SubElement(root, "ProjectConfiguration")
        project_configuration.attrib["Include"] = type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"]
    
        configuration = ElementTree.SubElement(project_configuration, "Configuration")
        configuration.text = type
        platform = ElementTree.SubElement(project_configuration, "Platform")
        platform.text = platformType.find( "gConfigData.Platform" ).attrib["value"]
    
def GeneratePropertyGroupImportGroup( root, type, configData ):

    for platformType in configData.iter( type ):
        property_group_element = ElementTree.SubElement( root, "PropertyGroup" )
        property_group_element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] + "'"
        property_group_element.attrib["Label"] = "Configuration"    
        
        element = ElementTree.SubElement( property_group_element, "ConfigurationType" )
        element.text = platformType.find( "gConfigData.ConfigurationType" ).attrib["value"]
        element = ElementTree.SubElement( property_group_element, "CharacterSet" )
        element.text = platformType.find( "gConfigData.CharacterSet" ).attrib["value"]
        element = ElementTree.SubElement( property_group_element, "PlatformToolset" )
        element.text = platformType.find( "gConfigData.PlatformToolset" ).attrib["value"]
        element = ElementTree.SubElement( property_group_element, "UseDebugLibraries" )
        element = ElementTree.SubElement( property_group_element, "WholeProgramOptimization" )
        
        import_group_element = ElementTree.SubElement( root, "ImportGroup" )
        import_group_element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        import_group_element.attrib["Label"] = "PropertySheets"
        
        import_element = ElementTree.SubElement( import_group_element, "Import" )
        import_element.attrib["Project"] = "$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props"    
        import_element.attrib["Condition"] = "exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')"
        import_element.attrib["Label"] = "LocalAppDataPlatform"

def GeneratePropertyGroup( root, type, configData ):

    for platformType in configData.iter( type ):
        element = ElementTree.SubElement( root, "OutDir" )
        element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        element.text = platformType.find( "gConfigData.OutputDirectory" ).attrib["value"]
        element = ElementTree.SubElement( root, "IntDir" )
        element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        element.text = platformType.find( "gConfigData.IntermediateDirectory" ).attrib["value"]
        element = ElementTree.SubElement( root, "ExtensionsToDeleteOnClean" )
        element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        element = ElementTree.SubElement( root, "GenerateManifest" )
        element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        element.text = "true"
        element = ElementTree.SubElement( root, "LinkIncremental" )
        element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        element.text = platformType.find( "gConfigData.LinkIncremental" ).attrib["value"]
        element = ElementTree.SubElement( root, "TargetName" )
        element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        element.text = platformType.find( "gConfigData.TargetName" ).attrib["value"]

def GenerateItemDefinitionGroup( root, type, configData ):

    for platformType in configData.iter( type ):
        itemDefinitionGroup_element = ElementTree.SubElement( root, "ItemDefinitionGroup" )
        itemDefinitionGroup_element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + type + "|" + platformType.find( "gConfigData.Platform" ).attrib["value"] +"'"
        
        element = ElementTree.SubElement( itemDefinitionGroup_element, "PreBuildEvent" )
        element = ElementTree.SubElement( element, "Command" )
        
        compile_element = ElementTree.SubElement( itemDefinitionGroup_element, "ClCompile" )
        element = ElementTree.SubElement( compile_element, "Optimization" )
        element.text = platformType.find( "gConfigData.Optimization" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "AdditionalOptions" )
        element.text = platformType.find( "gConfigData.AdditionalOptionsCompiler" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "InlineFunctionExpansion" )
        element.text = platformType.find( "gConfigData.InlineFunctionExpansion" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "IntrinsicFunctions" )
        element.text = platformType.find( "gConfigData.IntrinsicFunctions" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "FavorSizeOrSpeed" )
        element.text = platformType.find( "gConfigData.FavorSizeOrSpeed" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "OmitFramePointers" )
        element.text = platformType.find( "gConfigData.OmitFramePointers" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "EnableFiberSafeOptimizations" )
        element.text = platformType.find( "gConfigData.EnableFiberSafeOptimizations" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "AdditionalIncludeDirectories" )
        includes = platformType.find( "gConfigData.AdditionalIncludeDirectories" ).attrib["value"]
        element.text = includes
        element = ElementTree.SubElement( compile_element, "CompileAsWinRT" )
        if platformType.find( "gConfigData.CompileAsWinRT" ) != None:
            element.text = platformType.find( "gConfigData.CompileAsWinRT" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "PreprocessorDefinitions" )
        defines = platformType.find( "gConfigData.PreprocessorDefinitions" ).attrib["value"]
        element.text = defines
        element = ElementTree.SubElement( compile_element, "DebugInformationFormat" )
        element.text = "ProgramDatabase"
        element = ElementTree.SubElement( compile_element, "ForcedUsingFiles" )
        element.text = "%(ForcedUsingFiles)"
        element = ElementTree.SubElement( compile_element, "StringPooling" )
        element.text = platformType.find( "gConfigData.StringPooling" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "MinimalRebuild" )
        element.text = platformType.find( "gConfigData.MinimalRebuild" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "BasicRuntimeChecks" )
        element.text = platformType.find( "gConfigData.BasicRuntimeChecks" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "RuntimeLibrary" )
        element.text = platformType.find( "gConfigData.RuntimeLibrary" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "FunctionLevelLinking" )
        element.text = platformType.find( "gConfigData.FunctionLevelLinking" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "RuntimeTypeInfo" )
        element.text = platformType.find( "gConfigData.RuntimeTypeInfo" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "PrecompiledHeader" )
        element.text = "NotUsing"
        element = ElementTree.SubElement( compile_element, "WarningLevel" )
        element.text = platformType.find( "gConfigData.WarningLevel" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "TreatWarningAsError" )
        element.text = platformType.find( "gConfigData.WarningAsErrors" ).attrib["value"]
        element = ElementTree.SubElement( compile_element, "DisableSpecificWarnings" )
        element.text = platformType.find( "gConfigData.DisableSpecificWarnings" ).attrib["value"]
            
        link_element = ElementTree.SubElement( itemDefinitionGroup_element, "Link" )
        element = ElementTree.SubElement( link_element, "AdditionalOptions" )    
        element.text = platformType.find( "gConfigData.LinkAdditionalOptions" ).attrib["value"]    
        element = ElementTree.SubElement( link_element, "RegisterOutput" )
        element.text = platformType.find( "gConfigData.RegisterOutput" ).attrib["value"]    
        element = ElementTree.SubElement( link_element, "AdditionalDependencies" )
        dependencies = platformType.find( "gConfigData.AdditionalDependencies" ).attrib["value"]
        element.text = dependencies
        element = ElementTree.SubElement( link_element, "OutputFile" )
        element.text = platformType.find( "gConfigData.OutputDirectory" ).attrib["value"] + platformType.find( "gConfigData.TargetName" ).attrib["value"] + ".exe"
        element = ElementTree.SubElement( link_element, "AdditionalLibraryDirectories" )
        libraries = platformType.find( "gConfigData.AdditionalLibraryDirectories" ).attrib["value"]
        element.text = libraries
        element = ElementTree.SubElement( link_element, "GenerateDebugInformation" )
        element.text = platformType.find( "gConfigData.GenerateDebugInformation" ).attrib["value"]
        element = ElementTree.SubElement( link_element, "ProgramDatabaseFile" )
        element.text = platformType.find( "gConfigData.ProgramDatabaseFile" ).attrib["value"]
        element = ElementTree.SubElement( link_element, "SubSystem" )
        element.text = platformType.find( "gConfigData.SubSystem" ).attrib["value"]
        element = ElementTree.SubElement( link_element, "OptimizeReferences" )
        element.text = platformType.find( "gConfigData.OptimizeReferences" ).attrib["value"]
        element = ElementTree.SubElement( link_element, "EnableCOMDATFolding" )
        element.text = platformType.find( "gConfigData.EnableCOMDATFolding" ).attrib["value"]
        element = ElementTree.SubElement( link_element, "TargetMachine" )
        element.text = platformType.find( "gConfigData.TargetMachine" ).attrib["value"]
        element = ElementTree.SubElement( link_element, "LinkTimeCodeGeneration" )
        if platformType.find( "gConfigData.LinkTimeCodeGeneration" ) != None:
            element.text = platformType.find( "gConfigData.LinkTimeCodeGeneration" ).attrib["value"]
        
        postBuildEvent_element = ElementTree.SubElement( itemDefinitionGroup_element, "PostBuildEvent" )
        element = ElementTree.SubElement( postBuildEvent_element, "Message" )
        element = ElementTree.SubElement( postBuildEvent_element, "Command" )
    
    
def GenerateFilters( fileList, outputFilename, configData ):

    project_element = ElementTree.Element('Project')    
    if configData.find( "gProjectData.ToolsVersion" ) != None:
        project_element.attrib["ToolsVersion"] = configData.find( "gProjectData.ToolsVersion" ).attrib["value"]    
    project_element.attrib["xmlns"] = "http://schemas.microsoft.com/developer/msbuild/2003"
    
    item_group = ElementTree.SubElement( project_element, "ItemGroup" )    
    filterMap = []
    
    for file in fileList:
        isDocument = False
        splitFile = os.path.splitext( file )        
        if splitFile[1] == ".cpp" or splitFile[1] == ".c":
            file_element = ElementTree.SubElement( item_group, "ClCompile" )
        elif splitFile[1] == ".hpp" or splitFile[1] == ".h":
            file_element = ElementTree.SubElement( item_group, "ClInclude" )
        elif splitFile[1] == ".xml":
            file_element = ElementTree.SubElement( item_group, "Xml" )
            isDocument = True
        elif splitFile[1] == ".dds" or splitFile[1] == ".png" or splitFile[1] == ".jpg":
            file_element = ElementTree.SubElement( item_group, "Image" )
            isDocument = True;
        elif splitFile[1] == ".mp4" or splitFile[1] == ".ogg" or splitFile[1] == ".o" or splitFile[1] == ".desc" or splitFile[1] == ".script" or splitFile[1] == ".txt":
            file_element = ElementTree.SubElement( item_group, "Media" )                    
            isDocument = True;
        else:
            file_element = ElementTree.SubElement( item_group, "None" )

        file_element.attrib["Include"] = file   

        splitFile = os.path.split( file )        
        splitDrive = os.path.splitdrive( splitFile[0] )    
        
        newPath = splitDrive[1]
        element = ElementTree.SubElement( file_element, "Filter" )
        element.text = newPath[1:]               
        
        tokens = newPath[1:].split( "\\" )
        filterPath = ""        
        for token in tokens:            
            filterPath += token
            if filterPath not in filterMap:
                filterMap.append( filterPath )
                element = ElementTree.SubElement( item_group, "Filter" )
                element.attrib["Include"] = filterPath
                element = ElementTree.SubElement( element, "UniqueIdentifier" )
                element.text = "{692bb02a-08f1-4528-8281-d6e86b94a713}"
            filterPath += "\\"
    
    pretty_xml = PrettifyXML( project_element )
    text_file = open( outputFilename + ".filters", "w")
    text_file.write( pretty_xml )
    text_file.close()

def Walktree( topFolder, fileList, rootDirectory, dirFilterList, extFilterList ):
    for f in os.listdir( topFolder ):
        if f != ".svn":
            pathname = os.path.join( topFolder, f )
            mode = os.stat( pathname )[ST_MODE]
            if S_ISDIR( mode ):                
                findPath = pathname.replace( rootDirectory, "" )
                findPath = findPath + "\\"
                
                addDirectory = False
                for path in dirFilterList:
                    if path.find( '*' ) != -1:                        
                        if findPath.find( path[:len( path ) - 1] ) != -1:
                            addDirectory = True                            
                
                if findPath in dirFilterList or addDirectory:
                    Walktree( pathname, fileList, rootDirectory, dirFilterList, extFilterList )
            elif S_ISREG( mode ):
                fileList.append( pathname )
            else:
                print( "Skipping %s" % pathname )
                
def GenerateItemGroup( root, configData, outputFilename, rootDirectory ):
    item_group = ElementTree.SubElement( root, "ItemGroup" )    

    dirFilterList = []
    for filter in configData.iter('gProjectData.CodeDirFilters'):
        dirFilterList.append( filter.attrib["value"] )
        
    extFilterList = []
    for filter in configData.iter('gProjectData.CodeFilter'):
        extFilterList.append( filter.attrib["value"] )        
        
    fileList = []
    Walktree( rootDirectory, fileList, rootDirectory, dirFilterList, extFilterList )
     
    excludeFilterList = []    
    for filter in configData.iter('gProjectData.ExcludeFile'):
        excludeFilterList.append( filter.attrib["value"] )        
        
    projectFileList = []
    for file in fileList:
        splitExt = os.path.splitext( file )
        if splitExt[1] in extFilterList:
            splitFile = os.path.split( file )
            findPath = splitFile[0]
            findPath = findPath.replace( rootDirectory, "" )        
            findPath = findPath + "\\"
            excludeFile = findPath + splitFile[1]
            
            if excludeFile not in excludeFilterList:
                projectFileList.append( file )
            
    for file in projectFileList:
        isDocument = False
        splitFile = os.path.splitext( file )        
        if splitFile[1] == ".cpp" or splitFile[1] == ".c":
            file_element = ElementTree.SubElement( item_group, "ClCompile" )
        elif splitFile[1] == ".hpp" or splitFile[1] == ".h":
            file_element = ElementTree.SubElement( item_group, "ClInclude" )
        elif splitFile[1] == ".xml":
            file_element = ElementTree.SubElement( item_group, "Xml" )
            isDocument = True
        elif splitFile[1] == ".dds" or splitFile[1] == ".png" or splitFile[1] == ".jpg":
            file_element = ElementTree.SubElement( item_group, "Image" )
            isDocument = True;
        elif splitFile[1] == ".mp4" or splitFile[1] == ".ogg" or splitFile[1] == ".o" or splitFile[1] == ".desc" or splitFile[1] == ".script" or splitFile[1] == ".txt":
            file_element = ElementTree.SubElement( item_group, "Media" )                    
            isDocument = True;
        else:
            file_element = ElementTree.SubElement( item_group, "None" )

        file_element.attrib["Include"] = file   

        if isDocument:        
        
            platformList = []            
            for filter in configData.iter('Debug'):
                platformList.append( ( "Debug", filter.find( "gConfigData.Platform" ).attrib["value"] ) )
            for filter in configData.iter('Release'):                
                platformList.append( ( "Release", filter.find( "gConfigData.Platform" ).attrib["value"] ) )
        
            for platform in platformList:
                element = ElementTree.SubElement( file_element, "DeploymentContent" )    
                element.attrib["Condition"] = "'$(Configuration)|$(Platform)'=='" + platform[0] + "|" + platform[1] + "'"                
                element.text = "true"                
            
            element = ElementTree.SubElement( file_element, "FileType" )    
            element.text = "Document"            
            
    GenerateFilters( projectFileList, outputFilename, configData )

def GenerateProject( configFilename, outputFilename, rootDirectory ):

    configData = LoadXMLConfig( configFilename )
    
    project_element = ElementTree.Element('Project')    
    project_element.attrib["DefaultTargets"] = "Build"
    if configData.find( "gProjectData.ToolsVersion" ) != None:
        project_element.attrib["ToolsVersion"] = configData.find( "gProjectData.ToolsVersion" ).attrib["value"]
    project_element.attrib["xmlns"] = "http://schemas.microsoft.com/developer/msbuild/2003"
    
    item_group_element = ElementTree.SubElement(project_element, "ItemGroup")
    item_group_element.attrib["Label"] = "ProjectConfigurations"
    
    GenerateProjectConfiguration( item_group_element, "Debug", configData )
    GenerateProjectConfiguration( item_group_element, "Release", configData )    
    
    property_group_element = ElementTree.SubElement( project_element, "PropertyGroup" )
    property_group_element.attrib["Label"] = "Globals"
    
    element = ElementTree.SubElement( property_group_element, "ProjectGuid" )
    element.text = configData.find( "gProjectData.ProjectID" ).attrib["value"]
    element = ElementTree.SubElement( property_group_element, "RootNamespace" )
    element.text = configData.find( "gProjectData.ProjectName" ).attrib["value"]
    element = ElementTree.SubElement( property_group_element, "MinimumVisualStudioVersion" )
    element = ElementTree.SubElement( property_group_element, "DefaultLanguage" )
    element = ElementTree.SubElement( property_group_element, "AppContainerApplication" )
    element = ElementTree.SubElement( property_group_element, "XapOutputs" )
    if configData.find( "gProjectData.XapOutputs" ) != None:
        element.text = configData.find( "gProjectData.XapOutputs" ).attrib["value"]
    element = ElementTree.SubElement( property_group_element, "XapFilename" )
    if configData.find( "gProjectData.XapFilename" ) != None:
        element.text = configData.find( "gProjectData.XapFilename" ).attrib["value"]
    element = ElementTree.SubElement( property_group_element, "WinMDAssembly" )
    if configData.find( "gProjectData.WinMDAssembly" ) != None:
        element.text = configData.find( "gProjectData.WinMDAssembly" ).attrib["value"]
    
    import_element = ElementTree.SubElement( project_element, "Import" )
    import_element.attrib["Project"] = "$(VCTargetsPath)\Microsoft.Cpp.Default.props"
    
    GeneratePropertyGroupImportGroup( project_element, "Debug", configData )
    GeneratePropertyGroupImportGroup( project_element, "Release", configData )
    
    import_element = ElementTree.SubElement( project_element, "Import" )
    import_element.attrib["Project"] = "$(VCTargetsPath)\Microsoft.Cpp.props"
    
    import_group_element = ElementTree.SubElement( project_element, "ImportGroup" )
    import_group_element.attrib["Label"] = "ExtensionSettings"
    
    property_group_element = ElementTree.SubElement( project_element, "PropertyGroup" )
    property_group_element.attrib["Label"] = "UserMacros"
    
    property_group_element = ElementTree.SubElement( project_element, "PropertyGroup" )
    
    element = ElementTree.SubElement( property_group_element, "_ProjectFileVersion" )
    element.text ="10.0.30319.1"
    GeneratePropertyGroup( property_group_element, "Debug", configData )
    GeneratePropertyGroup( property_group_element, "Release", configData )
    
    GenerateItemDefinitionGroup( project_element, "Debug", configData )
    GenerateItemDefinitionGroup( project_element, "Release", configData )
    
    GenerateItemGroup( project_element, configData, outputFilename, rootDirectory )
    
    if configData.find( "gProjectData.WinMDAssembly" ) != None:
        if configData.find( "gProjectData.WinMDAssembly" ).attrib["value"] == "true":
            item_group_element = ElementTree.SubElement( project_element, "ItemGroup" )
            reference_element = ElementTree.SubElement( item_group_element, "Reference" )
            reference_element.attrib["Include"] = "platform.winmd"
            element = ElementTree.SubElement( reference_element, "IsWinMDFile" )
            element.text = "true"
            element = ElementTree.SubElement( reference_element, "Private" )
            element.text = "false"    
    
    import_element = ElementTree.SubElement( project_element, "Import" )
    import_element.attrib["Project"] = "$(VCTargetsPath)\Microsoft.Cpp.targets"
    
    import_group_element = ElementTree.SubElement( project_element, "ImportGroup" )
    import_group_element.attrib["Label"] = "ExtensionTargets"
                    
    pretty_xml = PrettifyXML( project_element )
    text_file = open( outputFilename, "w")
    text_file.write( pretty_xml )
    text_file.close()
    
#################################################################################
# Main
#################################################################################

def main( configFilename, outputFilename, rootDirectory ):    
    GenerateProject( configFilename, outputFilename, rootDirectory )    

if __name__=='__main__':
    sys.exit( main( sys.argv[1], sys.argv[2], sys.argv[3] ) )