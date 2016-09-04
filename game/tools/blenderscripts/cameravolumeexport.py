#
# IMPORTS
#
import bpy
import imp
import mathutils

import utilities
imp.reload( utilities )

from math import radians

############################################################
#
# GLOBALS
#
############################################################


############################################################
#
# EXPORT CAMERAS
#
############################################################

def ExportCameraVolume( object, doc, rootElement ):    

    cameraVolumeElement = doc.createElement( "camera_volume" )
    
    cameraTypeAttribute = "camera_type"
    cameraVolumeElement.setAttribute( cameraTypeAttribute, "camera_follow" )    
    if cameraTypeAttribute in object:
        cameraVolumeElement.setAttribute( cameraTypeAttribute, object[cameraTypeAttribute] )
        
    cameraNameAttribute = "camera_name"
    cameraVolumeElement.setAttribute( cameraNameAttribute, object[cameraNameAttribute] );
    
    attributeOffsetArray = { "angle", "position" }
    offsetArray = { "x", "y", "z" }
    
    for attributeOffset in attributeOffsetArray:
        for offset in offsetArray:
            attribute = offset + "_" + attributeOffset + "_offset"    
            cameraVolumeElement.setAttribute( attribute, "0" )
            if attribute in object:
                cameraVolumeElement.setAttribute( attribute, str( object[attribute] ) )
        
    rootElement.appendChild( cameraVolumeElement )