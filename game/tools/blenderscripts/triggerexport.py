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

def ExportTrigger( object, doc, rootElement ):    

    triggerElement = doc.createElement( "trigger" )
    
    items = object.items();
    for item in items:
        if item[0] != "_RNA_UI":
            triggerElement.setAttribute( item[0], item[1] );
        
    rootElement.appendChild( triggerElement )