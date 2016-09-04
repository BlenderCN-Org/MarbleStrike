#
# IMPORTS
#
import os
import bpy

############################################################
#
# GLOBALS
#
############################################################


############################################################
#
# EXPORT MATERIALS
#
############################################################

def ExportMaterial( material, doc, rootElement ):
    materialElement = doc.createElement( "material" )
    materialElement.setAttribute( "name", material.name )
    materialElement.setAttribute( "use_transparency", str( material.use_transparency ) )
    
    assert "effect" in material, "Material has no EFFECT property: %s." % material.name
    
    effect = "NO_EFFECT"
    if "effect" in material:
        effect = material["effect"]    
    materialElement.setAttribute( "effect", effect )
    rootElement.appendChild( materialElement )
    
    if material.library != None:
        library = material.library
        filepath = library.filepath
        splitExt = os.path.splitext( filepath )
        filename = os.path.basename( splitExt[0] )
        dataFilePath = bpy.data.filepath
        findIndex = dataFilePath.find( "assets" )
        findIndex = findIndex + len( "assets" ) + 1
        baseName = bpy.path.basename( dataFilePath )
        baseNameIndex = dataFilePath.find( baseName )
        filename = dataFilePath[findIndex:baseNameIndex] + filename
        materialElement.setAttribute( "lib_filepath", filename )
        
    else:
        
        #
        # Diffuse Color
        #
        colorElement = doc.createElement( "diffusecolor" )
        materialElement.appendChild( colorElement )
        color = material.diffuse_color
        alpha = material.alpha
        diffuseColorString = ( "%f" % color[0] ) + " " + ( "%f" % color[1] ) + " " + ( "%f" % color[2] ) + " " + ( "%f" % alpha )
        textNode = doc.createTextNode( diffuseColorString )    
        colorElement.appendChild( textNode )
        
        #
        # Specular Color
        #
        specElement = doc.createElement( "specularcolor" )
        materialElement.appendChild( specElement )
        specColor = material.specular_color
        specularity = material.specular_hardness
        specColorString = ( "%f" % specColor[0] ) + " " + ( "%f" % specColor[1] ) + " " + ( "%f" % specColor[2] ) + " " + ( "%f" % specularity )
        textNode = doc.createTextNode( specColorString )    
        specElement.appendChild( textNode )
        
        #
        # Reflectivity
        #
        reflectElement = doc.createElement( "reflectivity" )
        materialElement.appendChild( reflectElement )
        reflectFactor = material.raytrace_mirror.reflect_factor
        reflectString = ( "%f" % reflectFactor )
        textNode = doc.createTextNode( reflectString )    
        reflectElement.appendChild( textNode )
        
        #
        # Friction
        #
        frictionElement = doc.createElement( "friction" )
        materialElement.appendChild( frictionElement )
        frictionString = ( "%f" % material.physics.friction )
        textNode = doc.createTextNode( frictionString )    
        frictionElement.appendChild( textNode )
        
        #
        # Textures
        #
        textureSlots = material.texture_slots
        for slot in textureSlots:
            if slot != None:
                texture = slot.texture
                if texture != None and texture.type == 'IMAGE':
                    textureImageElement = doc.createElement( "textureimage" )
                    materialElement.appendChild( textureImageElement )
                    
                    imageFullPath = texture.image.filepath
                    imageFullPath = imageFullPath.replace("//","")
                    pathSplit = os.path.split( imageFullPath )
                    imageName = pathSplit[1]
                    splitText = os.path.splitext(imageName)
                    
                    currentWorkDirectory = os.getcwd()
                    workingDirectorySplit = os.path.split( currentWorkDirectory )
                    
                    assetDirectory = "assets"
                    assetIndex = currentWorkDirectory.find( assetDirectory )
                    textureBeginIndex = assetIndex + len( assetDirectory ) + 1
                    textureFilePath = currentWorkDirectory[textureBeginIndex:] + "\\" + splitText[0]
                    textureFilePath = textureFilePath.replace( "/", "\\" )
                    
                    while textureFilePath.find( "\\\\" ) != -1:
                        textureFilePath = textureFilePath.replace( "\\\\", "\\" )
                    
                    textNode = doc.createTextNode( textureFilePath )
                    textureImageElement.appendChild( textNode )
                    
                    if slot.use_map_color_diffuse:
                        textureImageElement.setAttribute( "mapto", "color" )
                    elif slot.use_map_normal:
                        textureImageElement.setAttribute( "mapto", "normal" )
                    elif slot.use_map_alpha:
                        textureImageElement.setAttribute( "mapto", "alpha" )