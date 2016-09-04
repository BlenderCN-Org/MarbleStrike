import os
import sys
import distutils.dir_util
from stat import *
from shutil import *

#################################################################################
#
#################################################################################

def ChangeSystemSlash( filename ):
    newFilename = filename
    if sys.platform == "darwin":
        newFilename = filename.replace("\\","/")
    return newFilename

#################################################################################
# Walktree Function
#################################################################################

def Walktree( topFolder, sourceFolder, destinationFolder, callback, assetType ):
    for f in os.listdir( topFolder ):
        if f != ".svn":
            pathname = os.path.join( topFolder, f )
            mode = os.stat( pathname )[ST_MODE]
            if S_ISDIR( mode ):
                Walktree( pathname, sourceFolder, destinationFolder, callback, assetType )
            elif S_ISREG( mode ):
                callback( pathname, sourceFolder, destinationFolder, assetType )
            else:
                print( "Skipping %s" % pathname )

#################################################################################
# Copy File Callback
#################################################################################

def CopyFileCallback( pathFilename, sourceFolder, destinationFolder, assetType ):

    destinationFilename = pathFilename
    destinationFilename = destinationFilename.replace( sourceFolder, destinationFolder )    
    destinationPath = os.path.dirname( destinationFilename )

    copyFile = False

    if not os.path.exists( destinationFilename ):
        copyFile = True
    else:
        lastTimeModifySrc = os.stat( pathFilename )[ST_MTIME]
        lastTimeModifyDst = os.stat( destinationFilename )[ST_MTIME]

        if lastTimeModifySrc > lastTimeModifyDst:
            copyFile = True

    if copyFile == True:
        tempPathFilename = pathFilename.replace( ChangeSystemSlash( "..\\" ), "" )
        if tempPathFilename in assetType:
            if not os.path.exists( destinationPath ):
                os.makedirs( destinationPath )
            print( pathFilename + " -> " + destinationFilename )
            copyfile( pathFilename, destinationFilename )

#################################################################################
# Shader Helper Functions
#################################################################################

def ParseInclude( filename, includePath ):
    includeFiles = []
    f = open( ChangeSystemSlash( filename ), 'r')
    for line in f:
        if line.find( "#include" ) >= 0:
            first = line.find( "\"" ) + 1
            end = line.rfind( "\"" )
            includeString = ""
            if end < first:
                includeString = line
            else:
                includeString = line[first:end]
            for path in includePath:
                filePath = path + includeString
                if os.path.exists( ChangeSystemSlash( filePath ) ):
                    includeList = ParseInclude( filePath, includePath )
                    for include in includeList:
                        includeFiles.append( include )
                    includeFiles.append( filePath )
    f.close()
    return includeFiles

#################################################################################
# D3D
#################################################################################

def BuildD3DShaderFile( inputFilename, destinationFilename, includeFiles ):
    fxcExe = '"C:\\Program Files (x86)\\Windows Kits\\8.0\\bin\\x86\\fxc.exe"'

    command = fxcExe

    splitExt = os.path.splitext( inputFilename )

    fileExt = splitExt[1]
    if ( fileExt == ".vsh" or fileExt == ".psh" ):
        if ( fileExt == ".vsh" ):
            command += " " + "/Zpr /Od /Tvs_3_0 /EVSMain /Zi"
        elif ( fileExt == ".psh" ):
            command += " " + "/Zpr /Od /Tps_3_0 /EPSMain /Zi"

        #for include in includeFiles:
        #    command += " " + "/I"
        #    command += include

        command += " " + "/D"
        command += "D3D9_API"

        command += " " + "/Fo"
        command += destinationFilename

        command += " " + inputFilename

        print( command )
        os.system( command )
        
def CopyD3DShaderCallback( pathFilename, sourceFolder, destinationFolder, assetFileList ):
    destinationFilename = pathFilename
    destinationFilename = destinationFilename.replace( sourceFolder, destinationFolder )    
    destinationFilename += ".o"
    destinationFilename = ChangeSystemSlash( destinationFilename )
    destinationPath = os.path.dirname( destinationFilename )

    includeFiles = []
    includePath = []
    includePath.append( sourceFolder )
    includeFiles = ParseInclude( pathFilename, includePath )

    copyFile = False

    if not os.path.exists( ChangeSystemSlash( destinationFilename ) ):
        copyFile = True
    else:
        lastTimeModifySrc = os.stat( ChangeSystemSlash( pathFilename ) )[ST_MTIME]
        lastTimeModifyDst = os.stat( destinationFilename )[ST_MTIME]

        if lastTimeModifySrc > lastTimeModifyDst:
            copyFile = True
        else:
            for include in includeFiles:
                lastTimeModifyInclude = os.stat( ChangeSystemSlash( include ) )[ST_MTIME]
                if lastTimeModifyInclude > lastTimeModifyDst:
                    copyFile = True
                    break

    if copyFile == True:
        tempPathFilename = pathFilename.replace( "..\\", "" )
        if tempPathFilename in assetFileList:
            if not os.path.exists( destinationPath ):
                os.makedirs( destinationPath )
            BuildD3DShaderFile(
                ChangeSystemSlash( pathFilename ),
                destinationFilename,
                includeFiles
                )

def BuildShadersD3D( destinationFolder, assetFileList ):

    sourceFolder = ChangeSystemSlash( "..\\assets\\shaders\\hlsl\\" )
    Walktree( sourceFolder, sourceFolder, destinationFolder, CopyD3DShaderCallback, assetFileList )    
    
#################################################################################
# D3D11
#################################################################################    

def BuildD3D11ShaderFile( inputFilename, destinationFilename, includeFiles ):
    fxcExe = '"C:\\Program Files (x86)\\Windows Kits\\8.0\\bin\\x86\\fxc.exe"'

    command = fxcExe

    splitExt = os.path.splitext( inputFilename )
    fileExt = splitExt[1]
    if ( fileExt == ".vsh" or fileExt == ".psh" ):
        if ( fileExt == ".vsh" ):
            command += " " + "/Zpr /Od /Tvs_4_0_level_9_3 /EVSMain /Zi"
        elif ( fileExt == ".psh" ):
            command += " " + "/Zpr /Od /Tps_4_0_level_9_3 /EPSMain /Zi"

        #for include in includeFiles:
        #    command += " " + "/I"
        #    command += include

        command += " " + "/D"
        command += "D3D11_API"

        command += " " + "/Fo"
        command += destinationFilename

        command += " " + inputFilename

        print( command )
        os.system( command )

def CopyD3D11ShaderCallback( pathFilename, sourceFolder, destinationFolder, assetFileList ):

    destinationFilename = pathFilename
    destinationFilename = destinationFilename.replace( sourceFolder, destinationFolder )    
    destinationFilename += ".o"
    destinationFilename = ChangeSystemSlash( destinationFilename )
    destinationPath = os.path.dirname( destinationFilename )
    
    includeFiles = []
    includePath = []
    includePath.append( sourceFolder )
    includeFiles = ParseInclude( pathFilename, includePath )    
    
    copyFile = False

    if not os.path.exists( destinationFilename ):
        copyFile = True
    else:
        lastTimeModifySrc = os.stat( ChangeSystemSlash( pathFilename ) )[ST_MTIME]
        lastTimeModifyDst = os.stat( destinationFilename )[ST_MTIME]

        if lastTimeModifySrc > lastTimeModifyDst:
            copyFile = True
        else:            
            for include in includeFiles:
                lastTimeModifyInclude = os.stat( ChangeSystemSlash( include ) )[ST_MTIME]
                if lastTimeModifyInclude > lastTimeModifyDst:
                    copyFile = True
                    break

    if copyFile == True:
        tempPathFilename = pathFilename.replace( "..\\", "" )
        if tempPathFilename in assetFileList:
            if not os.path.exists( destinationPath ):
                os.makedirs( destinationPath )            
            BuildD3D11ShaderFile(
                ChangeSystemSlash( pathFilename ),
                destinationFilename,
                includeFiles
                )    
    
def BuildShadersD3D11( destinationFolder, assetFileList ):
    
    sourceFolder = ChangeSystemSlash( "..\\assets\\shaders\\hlsl\\" )
    Walktree( sourceFolder, sourceFolder, destinationFolder, CopyD3D11ShaderCallback, assetFileList )

    d3dShaderToolExe = "..\\tools\\d3dshadertool\\d3dshadertool.exe"
    command = d3dShaderToolExe
    command += " " + destinationFolder

    print( command )
    os.system( command )

#################################################################################
# OpenGL
#################################################################################    

def GenerateShaderCopy( pathFilename, sourceFolder, destinationFilename ):

    includePath = []
    includePath.append( sourceFolder )

    includeFiles = ParseInclude( pathFilename, includePath )

    outputFile = open( ChangeSystemSlash( destinationFilename ), "w+" )
    inputFile = open( ChangeSystemSlash( pathFilename ), "r" )

    includeDict = {}

    for include in includeFiles:
        if include not in includeDict:
            includeFile = open( ChangeSystemSlash( include ), "r" )
            for line in includeFile:
                if line.find( "#include" ) == -1:
                    outputFile.write( line )
            outputFile.write( "\n\n" )
            includeFile.close()

            includeDict[include] = 1

    for line in inputFile:
        if line.find( "#include" ) == -1:
            outputFile.write( line )

    inputFile.close()
    outputFile.close()

def CopyOGLShaderCallback( pathFilename, sourceFolder, destinationFolder, assetFileList ):
    
    destinationFilename = pathFilename
    destinationFilename = destinationFilename.replace( sourceFolder, destinationFolder )    
    destinationFilename = ChangeSystemSlash( destinationFilename )
    destinationPath = os.path.dirname( destinationFilename )

    copyFile = False

    if not os.path.exists( destinationFilename ):
        copyFile = True
    else:
        lastTimeModifySrc = os.stat( ChangeSystemSlash( pathFilename ) )[ST_MTIME]
        lastTimeModifyDst = os.stat( destinationFilename )[ST_MTIME]

        if lastTimeModifySrc > lastTimeModifyDst:
            copyFile = True
        else:
            includePath = []
            includePath.append( sourceFolder )
            includeFiles = ParseInclude( pathFilename, includePath )
            for include in includeFiles:
                lastTimeModifyInclude = os.stat( ChangeSystemSlash( include ) )[ST_MTIME]
                if lastTimeModifyInclude > lastTimeModifyDst:
                    copyFile = True
                    break
                    
    if copyFile == True:
        tempPathFilename = pathFilename.replace( ChangeSystemSlash( "..\\" ), "" )
        if tempPathFilename in assetFileList:
            if not os.path.exists( destinationPath ):
                os.makedirs( destinationPath )
                
            splitExt = os.path.splitext( pathFilename )
            if splitExt[1] == ".vert" or splitExt[1] == ".frag":
                print( pathFilename + " -> " + destinationFilename )
                GenerateShaderCopy(
                    ChangeSystemSlash( pathFilename ),
                    sourceFolder,
                    destinationFilename
                    )
                
def BuildShadersOGL( destinationFolder, assetFileList ):
    
    sourceFolder = ChangeSystemSlash( "..\\assets\\shaders\\glsl\\" )
    Walktree( sourceFolder, sourceFolder, destinationFolder, CopyOGLShaderCallback, assetFileList )

#################################################################################
# Main
#################################################################################

assetEffects = [
    "assets\\effects\\bowler_0.xml",
    "assets\\effects\\bowler_1.xml",
    "assets\\effects\\bowler_2.xml",
    "assets\\effects\\bowler_3.xml",
    "assets\\effects\\shadow.xml",
    "assets\\effects\\skydome.xml",
    "assets\\effects\\simple.xml",
    "assets\\effects\\post_pass.xml",
    "assets\\effects\\particle.xml",
    "assets\\effects\\states\\blend\\alphablend.xml",
    "assets\\effects\\states\\blend\\additiveblend.xml",
    "assets\\effects\\states\\blend\\noblend.xml",
    "assets\\effects\\states\\depthstencil\\no_ztest_no_zwrite.xml",
    "assets\\effects\\states\\depthstencil\\no_zwrite.xml",
    "assets\\effects\\states\\depthstencil\\zlessequal.xml",
    "assets\\effects\\states\\depthstencil\\zlessequalteststencil.xml",
    "assets\\effects\\states\\depthstencil\\zlessequalwritestencil.xml",
    "assets\\effects\\states\\rasterizer\\backcull.xml",
    "assets\\effects\\states\\rasterizer\\frontcull.xml",
    "assets\\effects\\states\\rasterizer\\nocull.xml",
    "assets\\effects\\states\\rasterizer\\simple.xml",
    "assets\\effects\\states\\sampler\\post.xml",
    "assets\\effects\\states\\sampler\\default.xml",
    "assets\\effects\\states\\sampler\\linear.xml",
    "assets\\effects\\states\\sampler\\shadow.xml",
    ]

assetAudioEffects = [
    "assets\\game\\bowler\\audio\\effects\\button_click.ogg",
    "assets\\game\\bowler\\audio\\effects\\sound1.ogg",
    ]

assetMusic = [
    "assets\\game\\bowler\\audio\\music\\OldeTimey.ogg",
    "assets\\game\\bowler\\audio\\music\\PluckyDaisy.ogg",
    "assets\\game\\bowler\\audio\\music\\BrightWish.ogg",
    "assets\\game\\bowler\\audio\\music\\CircusWaltzFX.ogg",
    "assets\\game\\bowler\\audio\\music\\FriendlyDay.ogg",
    "assets\\game\\bowler\\audio\\music\\GoldRush.ogg",
    "assets\\game\\bowler\\audio\\music\\HammockFight.ogg",
    "assets\\game\\bowler\\audio\\music\\MerryGo.ogg",
    "assets\\game\\bowler\\audio\\music\\Wallpaper.ogg",
    ]

assetGUI_480x320 = [
    "assets\\game\\bowler\\gui\\bowleroptionsgui.xml",
    "assets\\game\\bowler\\gui\\bowlerrungui.xml",
    "assets\\game\\bowler\\gui\\bowlerloadgui.xml",
    "assets\\game\\bowler\\gui\\bowlerselectgui.xml",
    "assets\\game\\bowler\\gui\\bowlerstartgui.xml",
    "assets\\game\\bowler\\gui\\bowlerstatsgui.xml",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_1.dds",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_2.dds",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_3.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_1.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_2.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_3.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_4.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_5.dds",
    "assets\\game\\bowler\\gui\\textures\\loading.dds",
    "assets\\game\\bowler\\gui\\textures\\loading_animated.dds",
    "assets\\game\\bowler\\gui\\textures\\loading_animated.txt",
    "assets\\game\\bowler\\gui\\textures\\run_layout_1.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_2.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_3.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_4.dds",
    "assets\\game\\bowler\\gui\\textures\\title.dds",
    "assets\\game\\bowler\\textures\\joystickcircle.dds",
    "assets\\game\\bowler\\textures\\joystickthumb.dds",
    "assets\\game\\bowler\\textures\\jump_icon.dds",
    "assets\\game\\bowler\\settings.json",
    ]

assetGUI_960x640 = [
    "assets\\game\\bowler\\gui\\bowleroptionsgui_960x640.xml",
    "assets\\game\\bowler\\gui\\bowlerrungui_960x640.xml",
    "assets\\game\\bowler\\gui\\bowlerloadgui_960x640.xml",
    "assets\\game\\bowler\\gui\\bowlerselectgui_960x640.xml",
    "assets\\game\\bowler\\gui\\bowlerstartgui_960x640.xml",
    "assets\\game\\bowler\\gui\\bowlerstatsgui_960x640.xml",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_1_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_2_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_3_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_1_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_2_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_3_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_4_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_5_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\loading_animated_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\loading_animated_960x640.txt",
    "assets\\game\\bowler\\gui\\textures\\run_layout_1_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_2_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_3_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_4_960x640.dds",
    "assets\\game\\bowler\\gui\\textures\\start_layout_960x640.dds",
    "assets\\game\\bowler\\textures\\joystickcircle_960x640.dds",
    "assets\\game\\bowler\\textures\\joystickthumb_960x640.dds",
    "assets\\game\\bowler\\textures\\jump_icon_960x640.dds",
    "assets\\game\\bowler\\settings_960x640.json",
    ]

assetGUI_1136x640 = [
    "assets\\game\\bowler\\gui\\bowleroptionsgui_1136x640.xml",
    "assets\\game\\bowler\\gui\\bowlerrungui_1136x640.xml",
    "assets\\game\\bowler\\gui\\bowlerloadgui_1136x640.xml",
    "assets\\game\\bowler\\gui\\bowlerselectgui_1136x640.xml",
    "assets\\game\\bowler\\gui\\bowlerstartgui_1136x640.xml",
    "assets\\game\\bowler\\gui\\bowlerstatsgui_1136x640.xml",
    "assets\\game\\bowler\\settings_1136x640.json",
    ]

assetGUI_1280x720 = [
    "assets\\game\\bowler\\gui\\bowleroptionsgui_1280x720.xml",
    "assets\\game\\bowler\\gui\\bowlerrungui_1280x720.xml",
    "assets\\game\\bowler\\gui\\bowlerloadgui_1280x720.xml",
    "assets\\game\\bowler\\gui\\bowlerselectgui_1280x720.xml",
    "assets\\game\\bowler\\gui\\bowlerstartgui_1280x720.xml",
    "assets\\game\\bowler\\gui\\bowlerstatsgui_1280x720.xml",
    "assets\\game\\bowler\\settings_1280x720.json",
    ]
    
assetGUI_1024x768 = [
    "assets\\game\\bowler\\gui\\bowleroptionsgui_1024x768.xml",
    "assets\\game\\bowler\\gui\\bowlerrungui_1024x768.xml",
    "assets\\game\\bowler\\gui\\bowlerloadgui_1024x768.xml",
    "assets\\game\\bowler\\gui\\bowlerselectgui_1024x768.xml",
    "assets\\game\\bowler\\gui\\bowlerstartgui_1024x768.xml",
    "assets\\game\\bowler\\gui\\bowlerstatsgui_1024x768.xml",
    "assets\\game\\bowler\\settings_1024x768.json",
    ]
    
assetGUI_2048x1536 = [
    "assets\\game\\bowler\\gui\\bowleroptionsgui_2048x1536.xml",
    "assets\\game\\bowler\\gui\\bowlerrungui_2048x1536.xml",
    "assets\\game\\bowler\\gui\\bowlerloadgui_2048x1536.xml",
    "assets\\game\\bowler\\gui\\bowlerselectgui_2048x1536.xml",
    "assets\\game\\bowler\\gui\\bowlerstartgui_2048x1536.xml",
    "assets\\game\\bowler\\gui\\bowlerstatsgui_2048x1536.xml",
    "assets\\game\\bowler\\gui\\textures\\start_layout_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_1_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\selection_layout_2_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_1_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_2_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_3_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_4_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\options_layout_5_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_1_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_2_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_3_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\run_layout_4_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\loading_animated_2048x1536.dds",
    "assets\\game\\bowler\\gui\\textures\\loading_animated_2048x1536.txt",
    "assets\\game\\bowler\\settings_2048x1536.json",    
    ]    

assetFileList = [
    "assets\\startup.xml",
    "assets\\fx\\sparkle.xml",
    "assets\\fx\\goalsplash.xml",
    "assets\\fx\\particletexture.dds",
    "assets\\font\\borisblackbloxx.xml",
    "assets\\font\\kgthelasttime.xml",
    "assets\\font\\textures\\borisblackbloxx.dds",
    "assets\\font\\textures\\kgthelasttime.dds",
    "assets\\game\\bowler\\database_user.xml",
    "assets\\game\\bowler\\database_readonly.xml",
    "assets\\game\\bowler\\common.xml",
    "assets\\game\\bowler\\level01.xml",
    "assets\\game\\bowler\\level02.xml",
    "assets\\game\\bowler\\level03.xml",
    "assets\\game\\bowler\\level04.xml",
    "assets\\game\\bowler\\level05.xml",
    "assets\\game\\bowler\\level06.xml",
    "assets\\game\\bowler\\level07.xml",
    "assets\\game\\bowler\\level08.xml",
    "assets\\game\\bowler\\level09.xml",
    "assets\\game\\bowler\\level10.xml",
    "assets\\game\\bowler\\level11.xml",
    "assets\\game\\bowler\\level12.xml",
    "assets\\game\\bowler\\level13.xml",
    "assets\\game\\bowler\\level14.xml",
    "assets\\game\\bowler\\level15.xml",
    "assets\\game\\bowler\\skydome.xml",
    "assets\\game\\bowler\\scripts\\in-out-01.script",
    "assets\\game\\bowler\\scripts\\in-out-02.script",
    "assets\\game\\bowler\\scripts\\in-out-03.script",
    "assets\\game\\bowler\\textures\\skydome.dds",
    "assets\\game\\bowler\\textures\\clouds.dds",
    "assets\\game\\bowler\\textures\\spheretexture.dds",
    "assets\\game\\bowler\\textures\\toon.dds",
    ]

shaderFileList = [
    "assets\\shaders\\hlsl\\bowler_0.psh",
    "assets\\shaders\\hlsl\\bowler_0.vsh",
    "assets\\shaders\\hlsl\\bowler_1.psh",
    "assets\\shaders\\hlsl\\bowler_1.vsh",
    "assets\\shaders\\hlsl\\bowler_2.psh",
    "assets\\shaders\\hlsl\\bowler_2.vsh",
    "assets\\shaders\\hlsl\\bowler_3.psh",
    "assets\\shaders\\hlsl\\bowler_3.vsh",
    "assets\\shaders\\hlsl\\bowler_2_hidden.psh",
    "assets\\shaders\\hlsl\\bowler_2_hidden.vsh",
    "assets\\shaders\\hlsl\\post_pass.psh",
    "assets\\shaders\\hlsl\\post_vertex.vsh",
    "assets\\shaders\\hlsl\\simpleparticle.psh",
    "assets\\shaders\\hlsl\\simpleparticle.vsh",
    "assets\\shaders\\hlsl\\skydome.psh",
    "assets\\shaders\\hlsl\\skydome.vsh",
    "assets\\shaders\\hlsl\\simplenolight.psh",
    "assets\\shaders\\hlsl\\simplenolight.vsh",
    "assets\\shaders\\hlsl\\simplelight.psh",
    "assets\\shaders\\hlsl\\simplelight.vsh",
    "assets\\shaders\\hlsl\\shadow_visualizer.psh",
    "assets\\shaders\\hlsl\\shadow_visualizer.vsh",
    "assets\\shaders\\hlsl\\shadowdepth.psh",
    "assets\\shaders\\hlsl\\shadowdepth.vsh",
    "assets\\shaders\\hlsl\\shadowdepthcolor.psh",
    "assets\\shaders\\hlsl\\shadowdepthcolor.vsh",
    "assets\\shaders\\hlsl\\shadow.h",
]

#################################################################################
# Deploy WindowsPhone8
#################################################################################

def DeployWindowsPhone8():

    wp8AssetFileList = []

    for asset in assetEffects:
        wp8AssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetAudioEffects:
        wp8AssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetMusic:
        asset = asset.replace( ".ogg", ".mp4" )
        wp8AssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_960x640:
        wp8AssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_1280x720:
        wp8AssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetFileList:
        wp8AssetFileList.append( ChangeSystemSlash( asset ) )

    wp8ShaderFileList = []
    for asset in shaderFileList:
        wp8ShaderFileList.append( ChangeSystemSlash( asset ) )

    sourceFolder = ChangeSystemSlash( "..\\assets\\" )
    destinationFolder = ChangeSystemSlash( "..\\buildwp\\resources\\" )
    Walktree( sourceFolder, sourceFolder, destinationFolder, CopyFileCallback, wp8AssetFileList )
    
    destinationFolder = ChangeSystemSlash( "..\\buildwp\\resources\\shaders\\bind3d11\\" )
    BuildShadersD3D11( destinationFolder, wp8ShaderFileList )

#################################################################################
# Deploy Android
#################################################################################

def DeployAndroid():

    androidAssetFileList = []

    for asset in assetEffects:
        androidAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetAudioEffects:
        androidAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetMusic:
        asset = asset.replace( ".ogg", ".mp4" )
        androidAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_960x640:
        androidAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_1280x720:
        androidAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetFileList:
        androidAssetFileList.append( ChangeSystemSlash( asset ) )
        
    for index, asset in enumerate(androidAssetFileList):
        asset = asset.replace( ".dds", ".pvr" )        
        androidAssetFileList[index] = asset;

    androidShaderFileList = []
    for asset in shaderFileList:
        asset = asset.replace( ".vsh", ".vert" )
        asset = asset.replace( ".psh", ".frag" )
        asset = asset.replace( "hlsl", "glsl" )
        androidShaderFileList.append( ChangeSystemSlash( asset ) )

    sourceFolder = ChangeSystemSlash( "..\\assets\\" )
    destinationFolder = ChangeSystemSlash( "..\\buildandroid\\assets\\" )
    Walktree( sourceFolder, sourceFolder, destinationFolder, CopyFileCallback, androidAssetFileList )
    
    destinationFolder = ChangeSystemSlash( "..\\buildandroid\\assets\\shaders\\binogl\\" )
    BuildShadersOGL( destinationFolder, androidShaderFileList )

#################################################################################
# Deploy iOS
#################################################################################

def DeployiOS():

    assetType = []
    iOSAssetFileList = []
    
    for asset in assetEffects:
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetAudioEffects:
        asset = asset.replace( ".ogg", ".mp4" )
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetMusic:
        asset = asset.replace( ".ogg", ".mp4" )
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_480x320:
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )        
    for asset in assetGUI_960x640:
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_1136x640:
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_1024x768:
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_2048x1536:
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )        
    for asset in assetFileList:        
        iOSAssetFileList.append( ChangeSystemSlash( asset ) )    
        
    for index, asset in enumerate(iOSAssetFileList):
        asset = asset.replace( ".dds", ".pvr" )
        iOSAssetFileList[index] = asset;

    iOSShaderFileList = []
    for asset in shaderFileList:
        asset = asset.replace( ".vsh", ".vert" )
        asset = asset.replace( ".psh", ".frag" )
        asset = asset.replace( "hlsl", "glsl" )
        iOSShaderFileList.append( ChangeSystemSlash( asset ) )

    sourceFolder = ChangeSystemSlash( "..\\assets\\" )
    destinationFolder = ChangeSystemSlash( "..\\deploy\\ios\\" )
    Walktree( sourceFolder, sourceFolder, destinationFolder, CopyFileCallback, iOSAssetFileList )

    destinationFolder = ChangeSystemSlash( "..\\deploy\\ios\\shaders\\binogl\\" )
    BuildShadersOGL( destinationFolder, iOSShaderFileList )

#################################################################################
# Deploy PC
#################################################################################

def DeployPC():

    dllFileList = [
        "sdk\\FreeImage\\Dist\\FreeImage.dll",
        "sdk\\glew\\lib\\glew32.dll",
        "sdk\\opengles2\\lib\\libEGL.dll",
        "sdk\\opengles2\\lib\\libGLESv2.dll"
    ]

    assetType = []
    pcAssetFileList = []

    for asset in assetEffects:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetAudioEffects:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
        asset = asset.replace( ".ogg", ".mp4" )
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetMusic:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
        asset = asset.replace( ".ogg", ".mp4" )
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_480x320:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )        
    for asset in assetGUI_960x640:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_1136x640:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_1280x720:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )
    for asset in assetGUI_1024x768:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )    
    for asset in assetGUI_2048x1536:
        pcAssetFileList.append( ChangeSystemSlash( asset ) )        
    for asset in assetFileList:        
        pcAssetFileList.append( ChangeSystemSlash( asset ) )   

    imageAssetFileList = []
    for asset in pcAssetFileList:
        splitExt = os.path.splitext( asset )
        if splitExt[1] == ".dds":
            imageAssetFileList.append( ChangeSystemSlash( asset ) )
            
    for asset in imageAssetFileList:
        asset = asset.replace( ".dds", ".pvr" )
        pcAssetFileList.append( ChangeSystemSlash( asset ) )

    pcShaderFileList = []
    for asset in shaderFileList:
        pcShaderFileList.append( ChangeSystemSlash( asset ) )
        asset = asset.replace( ".vsh", ".vert" )
        asset = asset.replace( ".psh", ".frag" )
        asset = asset.replace( "hlsl", "glsl" )
        pcShaderFileList.append( ChangeSystemSlash( asset ) )        

    sourceFolder = ChangeSystemSlash( "..\\assets\\" )
    destinationFolder = ChangeSystemSlash( "..\\deploy\\pc\\" )
    Walktree( sourceFolder, sourceFolder, destinationFolder, CopyFileCallback, pcAssetFileList )
    
    destinationFolder = ChangeSystemSlash( "..\\deploy\\pc\\shaders\\bind3d\\" )
    BuildShadersD3D( destinationFolder, pcShaderFileList )
    destinationFolder = ChangeSystemSlash( "..\\deploy\\pc\\shaders\\bind3d11\\" )
    BuildShadersD3D11( destinationFolder, pcShaderFileList )
    destinationFolder = ChangeSystemSlash( "..\\deploy\\pc\\shaders\\binogl\\" )
    BuildShadersOGL( destinationFolder, pcShaderFileList )
    
    destinationFolder = ChangeSystemSlash( "..\\deploy\\pc\\" )
    sourceFolder = ChangeSystemSlash( "..\\sdk\\FreeImage\\Dist\\" )
    CopyFileCallback( ChangeSystemSlash( "..\\sdk\\FreeImage\\Dist\\FreeImage.dll" ), sourceFolder, destinationFolder, dllFileList )
    sourceFolder = ChangeSystemSlash( "..\\sdk\\glew\\lib\\" )
    CopyFileCallback( ChangeSystemSlash( "..\\sdk\\glew\\lib\\glew32.dll" ), sourceFolder, destinationFolder, dllFileList )
    sourceFolder = ChangeSystemSlash( "..\\sdk\\opengles2\\lib\\" )
    CopyFileCallback( ChangeSystemSlash( "..\\sdk\\opengles2\\lib\\libEGL.dll" ), sourceFolder, destinationFolder, dllFileList )
    sourceFolder = ChangeSystemSlash( "..\\sdk\\opengles2\\lib\\" )
    CopyFileCallback( ChangeSystemSlash( "..\\sdk\\opengles2\\lib\\libGLESv2.dll" ), sourceFolder, destinationFolder, dllFileList )

#################################################################################
# Main
#################################################################################

def main( arg ):
    if arg == "WindowsPhone8":
        DeployWindowsPhone8()
    elif arg == "iOS":
        DeployiOS()
    elif arg == "Android":
        DeployAndroid()
    elif arg == "PC":
        DeployPC()

if __name__=='__main__':
    sys.exit( main( sys.argv[1] ) )