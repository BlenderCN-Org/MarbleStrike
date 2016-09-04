#
# IMPORTS
#
import bpy
import imp
import mathutils

import utilities
imp.reload(utilities)

from math import radians

############################################################
#
# GLOBALS
#
############################################################

############################################################
#
# EXPORT ARMATURE POSE
#
############################################################

############################################################
#
# EXPORT MESH
#
############################################################

def ExportCurve( object, doc, rootElement ):
    
    curve = object.data
    
    curveElement = doc.createElement( "curve" )
    curveElement.setAttribute( "name", curve.name )
    rootElement.appendChild( curveElement )
    curveElement.setAttribute( "numsplines", str( len( curve.splines ) ) )
    curveElement.setAttribute( "path_duration", ( "%f" % curve.path_duration ) )
        
    #
    # SPLINES
    #
    currentIndex = 0
    for spline in curve.splines:
        splineElement = doc.createElement( "spline" )
        curveElement.appendChild( splineElement )        
        splineElement.setAttribute( "numpoints", str( len( spline.bezier_points ) ) )        
        splineElement.setAttribute( "use_cyclic", str( spline.use_cyclic_u ) )
        for point in spline.bezier_points:
            pointElement = doc.createElement( "point" )
            splineElement.appendChild( pointElement )
            fixPosition = utilities.FixVector( point.co )
            fixHandleLeft = utilities.FixVector( point.handle_left )
            fixHandleRight = utilities.FixVector( point.handle_right )
            position = ( "%f" % fixPosition[0] ) + " " + ( "%f" % fixPosition[1] ) + " " + ( "%f" % fixPosition[2] )
            pointElement.setAttribute( "position", position )
            handle_left = ( "%f" % fixHandleLeft[0] ) + " " + ( "%f" % fixHandleLeft[1] ) + " " + ( "%f" % fixHandleLeft[2] )
            pointElement.setAttribute( "handle_left", handle_left )
            handle_right = ( "%f" % fixHandleRight[0] ) + " " + ( "%f" % fixHandleRight[1] ) + " " + ( "%f" % fixHandleRight[2] )
            pointElement.setAttribute( "handle_right", handle_right )
        currentIndex = currentIndex + 1
        
    #
    # Animation
    #
    
    utilities.WriteAnimationData( curveElement, doc, curve.animation_data );