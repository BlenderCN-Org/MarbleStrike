import os
import sys
from stat import *
from shutil import *
from struct import *
import json
import Image

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
				print "Skipping %s" % pathname

#################################################################################
# Build Texture DDS
#################################################################################
	
def BuildData( inputFile, outputFile ):
	
	with open( inputFile, "rb") as f:	

		jsonData = {}

		values = unpack( "iiii", f.read(16) )

		jsonData['width'] = values[0]
		jsonData['height'] = values[1]
		jsonData['cellwidth'] = values[2]
		jsonData['cellheight'] = values[3]

		base = unpack( "B", f.read(1) )
		jsonData['base'] = base[0]
		
		widthList = []
		byte = f.read(1)
		while byte != "":
			byte = unpack( "B", byte )
			widthList.append( byte[0] )
			byte = f.read(1)
		
		jsonData['widthschar'] = widthList
		
		jsonFile = json.dumps( jsonData, sort_keys=True, indent=4 )
		
		writeFile = open( outputFile, "w+" )
		writeFile.write( jsonFile )

#################################################################################
# Copy File Callback
#################################################################################

def ProcessCallback( inputFile ):

	assetType = [
	".dat"
	]
		
	splitExt = os.path.splitext( inputFile )
	if splitExt[1].lower() in assetType:	
			
		outputFile = inputFile.replace( "dat", "json" )
		
		shouldProcessTexture = False

		if not os.path.exists( outputFile ):
			shouldProcessTexture = True
		else:
			lastTimeModifySrc = os.stat( inputFile )[ST_MTIME]
			lastTimeModifyDst = os.stat( outputFile )[ST_MTIME]
		
			if lastTimeModifySrc > lastTimeModifyDst:
				shouldProcessTexture = True
			
		if shouldProcessTexture == True:
			BuildData( inputFile, outputFile )
	
#################################################################################
# Main
#################################################################################

print sys.platform

folderList = [ 
	"..\\assets\\font\\"
	]
	

for fileList in folderList:
	newFilename = ChangeSystemSlash( fileList )
	Walktree( newFilename, ProcessCallback )