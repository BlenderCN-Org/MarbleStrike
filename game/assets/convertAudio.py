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
				print "Skipping %s" % pathname

#################################################################################
# Build Audio
#################################################################################
	
def ConvertFile( inputFile, outputFile ):
	
	executable = "afconvert"
	command = executable
	command += " " + "-f m4af"
	command += " " + "-d aac"
	command += " " + inputFile
	command += " " + outputFile

	print command
	
	os.system( command )

#################################################################################
# Copy File Callback
#################################################################################

def ProcessAudioCallback( inputFile ):

	assetType = [
	".mp3"
	]
	
	splitExt = os.path.splitext( inputFile )
	if splitExt[1].lower() in assetType:	
			
		outputFile = inputFile.replace( "mp3", "mp4" )
		
		shouldProcessFile = False

		if not os.path.exists( outputFile ):
			shouldProcessFile = True
		else:
			lastTimeModifySrc = os.stat( inputFile )[ST_MTIME]
			lastTimeModifyDst = os.stat( outputFile )[ST_MTIME]
		
			if lastTimeModifySrc > lastTimeModifyDst:
				shouldProcessFile = True
			
		if shouldProcessFile == True:
			ConvertFile( inputFile, outputFile )

#################################################################################
# Main
#################################################################################

print sys.platform

folderList = [ 
	"..\\assets\\"
	]
	
for fileList in folderList:
	newFilename = ChangeSystemSlash( fileList )
	Walktree( newFilename, ProcessAudioCallback )