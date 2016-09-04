import bpy
import os

textureList = bpy.data.textures
for texture in textureList:
	image = texture.getImage()
	imageFilename = image.getFilename()
		
	pathSplit = os.path.split( imageFilename )
	
	newFilename = "textures\\truck\\" + pathSplit[1]
	print newFilename
	
	image.setFilename( newFilename )