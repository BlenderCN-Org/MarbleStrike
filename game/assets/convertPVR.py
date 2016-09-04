import os
import sys
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

def Walktree( top, callback ):
	for f in os.listdir( top ):
		if f != ".svn":
			pathname = os.path.join( top, f )
			mode = os.stat( pathname )[ST_MODE]
			if S_ISDIR( mode ):
				Walktree( pathname, callback )
			elif S_ISREG( mode ):
				callback( pathname )
			else:
				print( "Skipping %s" % pathname )

#################################################################################
# Build Texture PVR
#################################################################################
	
def BuildTexturePVR( inputFile, outputFile ):
	
	noFlipTexture = [
	"..\\assets\\game\\common\\textures\\envmaps\\inclouds_nx.png",
	"..\\assets\\game\\common\\textures\\envmaps\\inclouds_ny.png",
	"..\\assets\\game\\common\\textures\\envmaps\\inclouds_nz.png",
	"..\\assets\\game\\common\\textures\\envmaps\\inclouds_px.png",
	"..\\assets\\game\\common\\textures\\envmaps\\inclouds_py.png",
	"..\\assets\\game\\common\\textures\\envmaps\\inclouds_pz.png"
	]
	
	platform = "Windows_x86_32"
	executable = "PVRTexTool.exe"
	if sys.platform == "darwin":	
		platform = "MacOS_x86"
		executable = "PVRTexTool"
		
	executable = "..\\tools\\common\\PVRTexTool\\PVRTexToolCL\\" + platform + "\\" + executable
	command = ChangeSystemSlash( executable )
	command += " " + "-i" + ChangeSystemSlash( inputFile )
	command += " " + "-o" + ChangeSystemSlash( outputFile )
	
	if inputFile not in noFlipTexture:	
		command += " " + "-yflip1"
	command += " " + "-m -fOGL8888"

	print( command )
	
	os.system( command )

#################################################################################
# Copy File Callback
#################################################################################

def ProcessTextureCallback( inputFile ):

	assetType = [
	".png"
	]
	
	splitExt = os.path.splitext( inputFile )
	if splitExt[1].lower() in assetType:	
			
		outputFile = inputFile.replace( "png", "pvr" )
		
		shouldProcessTexture = False

		if not os.path.exists( outputFile ):
			shouldProcessTexture = True
		else:
			lastTimeModifySrc = os.stat( inputFile )[ST_MTIME]
			lastTimeModifyDst = os.stat( outputFile )[ST_MTIME]
		
			if lastTimeModifySrc > lastTimeModifyDst:
				shouldProcessTexture = True
			
		if shouldProcessTexture == True:
			BuildTexturePVR( inputFile, outputFile )

#################################################################################
# Main
#################################################################################

print( sys.platform )

folderList = [ 
	"..\\assets\\"
	]
	
for fileList in folderList:
	newFilename = ChangeSystemSlash( fileList )
	Walktree( newFilename, ProcessTextureCallback )