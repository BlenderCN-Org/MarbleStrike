import imp
import xml.dom.minidom
import bpy
import objectexport
imp.reload( objectexport )
import materialexport
imp.reload( materialexport )

##############################################################
#
# Export Scene
#
##############################################################

def export_scene(operator, context, filepath=""):

    print( "********** START EXPORTING **********" )
     
    #
    # Create XML file
    #    
    doc = xml.dom.minidom.Document()
    rootElement = doc.createElement("blenderasset")
    doc.appendChild( rootElement )
    
    scenes = bpy.data.scenes
    scene = scenes[0]
    objectList = scene.objects
    
    #
    # Must contain at least 1 bounding volume and camera volume
    #
    if scene.name != "lib":
        hasBoundingVolume = False
        hasCameraVolume = False
        for object in bpy.data.objects:
            if "custom_type" in object:
                if object["custom_type"] == "bounding_volume":
                    hasBoundingVolume = True
                if object["custom_type"] == "camera_volume":
                    hasCameraVolume = True
        
        assert hasBoundingVolume == True
        assert hasCameraVolume == True
    
    #
    # Objects
    #
    objectCount = 0
    for object in bpy.data.objects:
        if object.layers[0] == True and object.library == None:            
            objectexport.ExportObject( object, doc, rootElement )
            objectCount += 1
            
    #
    # Materials
    #
    materialCount = 0
    for material in bpy.data.materials:        
        if material.users > 0:
            materialexport.ExportMaterial( material, doc, rootElement )
            materialCount += 1
    
    rootElement.setAttribute( "MaterialCount", str( materialCount ) )
    rootElement.setAttribute( "ObjectCount", str( objectCount ) )
        
    #
    # Write XML
    #
    sceneName = scene.name
    file_object = open(filepath, "w", encoding="utf8", newline="\n")
    file_object.write( doc.toprettyxml() )
    file_object.close()
         
    print( "********** END EXPORTING **********" )