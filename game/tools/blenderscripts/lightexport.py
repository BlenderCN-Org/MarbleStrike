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
# EXPORT LIGHTS
#
############################################################

def ExportLight( object, doc, rootElement ):    

    light = object.data
    
    lightElement = doc.createElement( "light" )
    lightElement.setAttribute( "name", light.name )
    lightElement.setAttribute( "type", light.type )
    if light.type == "SPOT":
        lightElement.setAttribute( "angle", str( light.spot_size ) )
        lightElement.setAttribute( "znear", str( light.shadow_buffer_clip_start ) )
        lightElement.setAttribute( "zfar", str( light.shadow_buffer_clip_end ) )
        lightElement.setAttribute( "fallofftype", light.falloff_type )
    rootElement.appendChild( lightElement )
            
    colorElement = doc.createElement("color")
    lightElement.appendChild( colorElement )
    lightColorString = ""
    lightColorString += str( light.color[0] );
    lightColorString += " "
    lightColorString += str( light.color[1] );
    lightColorString += " "
    lightColorString += str( light.color[2] );    
    textNode = doc.createTextNode( lightColorString )    
    colorElement.appendChild( textNode )