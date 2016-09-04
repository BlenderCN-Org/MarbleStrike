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

def ExportCamera( object, doc, rootElement ):    

    camera = object.data

    cameraElement = doc.createElement( "camera" )
    cameraElement.setAttribute( "name", camera.name )
    cameraElement.setAttribute( "angle", ( "%f" % camera.angle ) )
    cameraElement.setAttribute( "near", ( "%f" % camera.clip_start ) )
    cameraElement.setAttribute( "far", ( "%f" % camera.clip_end ) )
    
    for constraint in object.constraints:
        constraintType = constraint.type
        if constraintType == "TRACK_TO":
            print( constraint.target )
            cameraElement.setAttribute( "trackto", constraint.target.name )
    
    rootElement.appendChild( cameraElement )