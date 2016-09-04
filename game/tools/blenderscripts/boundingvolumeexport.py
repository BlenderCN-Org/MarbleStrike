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
# EXPORT BOUNDING VOLUMES
#
############################################################

def ExportBoundingVolume( object, doc, rootElement ):    

    boundingVolumeElement = doc.createElement( "bounding_volume" )
    
    boundingVolumeMoveXAttribute = "is_moveable_x"            
    if boundingVolumeMoveXAttribute in object:
        boundingVolumeElement.setAttribute( boundingVolumeMoveXAttribute, object[boundingVolumeMoveXAttribute] )    
        
    boundingVolumeMoveZAttribute = "is_moveable_z"
    if boundingVolumeMoveZAttribute in object:
        boundingVolumeElement.setAttribute( boundingVolumeMoveZAttribute, object[boundingVolumeMoveZAttribute] )    
        
    rootElement.appendChild( boundingVolumeElement )