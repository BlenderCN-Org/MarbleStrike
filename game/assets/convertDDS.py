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
# Build Texture DDS
#################################################################################
	
def BuildTextureDDS( inputFile, outputFile ):
	
	executable = "..\\tools\\common\\nvdxt.exe"
	command = executable
	command += " " + "-file " + inputFile
	command += " " + "-output " + outputFile
	command += " " + "-nomipmap -dxt3"

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
			
		outputFile = inputFile.replace( "png", "dds" )
		
		shouldProcessTexture = False

		if not os.path.exists( outputFile ):
			shouldProcessTexture = True
		else:
			lastTimeModifySrc = os.stat( inputFile )[ST_MTIME]
			lastTimeModifyDst = os.stat( outputFile )[ST_MTIME]
		
			if lastTimeModifySrc > lastTimeModifyDst:
				shouldProcessTexture = True
			
		if shouldProcessTexture == True:
			BuildTextureDDS( inputFile, outputFile )

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