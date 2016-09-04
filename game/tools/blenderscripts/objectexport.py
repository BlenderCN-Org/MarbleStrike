#
# IMPORTS
#
import bpy
import imp
import mathutils
import os

import curveexport
imp.reload( curveexport )
import cameraexport
imp.reload( cameraexport )
import cameravolumeexport
imp.reload( cameravolumeexport )
import boundingvolumeexport
imp.reload( boundingvolumeexport )
import lightexport
imp.reload( lightexport )
import meshexport
imp.reload( meshexport )
import utilities
imp.reload(utilities)
import triggerexport
imp.reload( triggerexport )

from math import radians

############################################################
#
# GLOBALS
#
############################################################

gCustomTypeAttribName = "custom_type"

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

def ExportPropertyString( property_name, object, doc, rootElement ):
    if property_name in object:
        property_value = object[property_name]
        rootElement.setAttribute( property_name, property_value )
        
def ExportPropertyInt( property_name, object, doc, rootElement ):
    if property_name in object:
        property_value = object[property_name]        
        rootElement.setAttribute( property_name, ("%i" % property_value ) )

def ExportObjectProperties( object, doc, rootElement ):
    ExportPropertyString( "start_animation", object, doc, rootElement )
    ExportPropertyString( "loop_animation", object, doc, rootElement )
    ExportPropertyString( "reset_animation", object, doc, rootElement )
    ExportPropertyString( "on_stop_animation", object, doc, rootElement )
    ExportPropertyInt( "start_frame", object, doc, rootElement )
    ExportPropertyString( "custom_type", object, doc, rootElement )
    ExportPropertyString( "game_type", object, doc, rootElement )
    ExportPropertyString( "script", object, doc, rootElement )
    
def ExportObject( object, doc, rootElement ):    
    
    print( object.name )
    
    elementObject = doc.createElement("object")
    rootElement.appendChild( elementObject )       
    
    elementObject.setAttribute( "name", object.name )
    elementObject.setAttribute( "custom_type", object.type )
    if object.parent != None:
        elementObject.setAttribute( "parent", object.parent.name )
    
    isVisible = object.is_visible( bpy.data.scenes[0] )
    elementObject.setAttribute( "visible", str( isVisible ) )
    
    ExportObjectProperties( object, doc, elementObject );
    
    elementObject.setAttribute( "proxy", str( object.proxy != None ) )
    
    elementBB = doc.createElement("boundingbox")
    elementObject.appendChild( elementBB )
    #boundingBox = object.dimensions    
    minBB = mathutils.Vector((999.0, 999.0, 999.0))
    maxBB = mathutils.Vector((-999.0, -999.0, -999.0))
    for vertex in object.bound_box:
        if vertex[0] < minBB[0]:
            minBB[0] = vertex[0]
        if vertex[1] < minBB[1]:
            minBB[1] = vertex[1]
        if vertex[2] < minBB[2]:
            minBB[2] = vertex[2]            
        if vertex[0] > maxBB[0]:
            maxBB[0] = vertex[0]
        if vertex[1] > maxBB[1]:
            maxBB[1] = vertex[1]
        if vertex[2] > maxBB[2]:
            maxBB[2] = vertex[2]
            
    boundingBox = maxBB - minBB
        
    boundingboxString = ( "%f" % boundingBox[0] ) + " " + ( "%f" % boundingBox[2] ) + " " + ( "%f" % boundingBox[1] )
    textNode = doc.createTextNode( boundingboxString )
    elementBB.appendChild( textNode )
    
    if object.proxy != None:
        proxyObject = object.proxy
        library = proxyObject.library
        filepath = library.filepath
        splitExt = os.path.splitext( filepath )
        filename = os.path.basename( splitExt[0] )
        dataFilePath = bpy.data.filepath
        findIndex = dataFilePath.find( "assets" )
        findIndex = findIndex + len( "assets" ) + 1
        baseName = bpy.path.basename( dataFilePath )
        baseNameIndex = dataFilePath.find( baseName )
        filename = dataFilePath[findIndex:baseNameIndex] + filename
        elementObject.setAttribute( "proxy_obj_name", proxyObject.name )        
        elementObject.setAttribute( "proxy_filepath", filename )
    
    #
    # Transform
    #
    elementTransform = doc.createElement("xform")
    elementObject.appendChild( elementTransform )    
    objectTransform = object.matrix_local

    if object.type == "CAMERA" or object.type == "LAMP" :
        rotate90Matrix = mathutils.Matrix.Rotation( radians(-90), 4, 'X' )
        objectTransform = objectTransform * rotate90Matrix    
    
    fixTransform = utilities.FixTransform( objectTransform )
    transformStringData = ""
    transformStringData = transformStringData + ( "%f" % fixTransform[0][0] ) + " " + ( "%f" % fixTransform[1][0] ) + " " + ( "%f" % fixTransform[2][0] ) + " " + ( "%f" % fixTransform[3][0] ) + " "
    transformStringData = transformStringData + ( "%f" % fixTransform[0][1] ) + " " + ( "%f" % fixTransform[1][1] ) + " " + ( "%f" % fixTransform[2][1] ) + " " + ( "%f" % fixTransform[3][1] ) + " "
    transformStringData = transformStringData + ( "%f" % fixTransform[0][2] ) + " " + ( "%f" % fixTransform[1][2] ) + " " + ( "%f" % fixTransform[2][2] ) + " " + ( "%f" % fixTransform[3][2] ) + " "
    transformStringData = transformStringData + ( "%f" % fixTransform[0][3] ) + " " + ( "%f" % fixTransform[1][3] ) + " " + ( "%f" % fixTransform[2][3] ) + " " + ( "%f" % fixTransform[3][3] ) + " "
    textNode = doc.createTextNode( transformStringData )
    elementTransform.appendChild( textNode )
    
    scale = utilities.GetScale( objectTransform )
    elementScale = doc.createElement("scale")
    elementObject.appendChild( elementScale )
    elementScale.setAttribute( "x", ( "%f" % scale[0] ) )
    elementScale.setAttribute( "y", ( "%f" % scale[1] ) )
    elementScale.setAttribute( "z", ( "%f" % scale[2] ) )
    
    #
    # Physics
    #
    elementPhysics = doc.createElement("physics")
    elementObject.appendChild( elementPhysics )       
    elementPhysics.setAttribute( "boundstype", object.game.collision_bounds_type )
    elementPhysics.setAttribute( "radius", ( "%f" % object.game.radius ) )
    elementPhysics.setAttribute( "mass", ( "%f" % object.game.mass ) )    
    elementPhysics.setAttribute( "physicstype", object.game.physics_type )
    elementPhysics.setAttribute( "is_compound", str( object.game.use_collision_compound ) )
    
    #
    # Animation
    #
    animationObject = object
    utilities.WriteAnimationData( elementObject, doc, animationObject.animation_data );
        
    #
    # Constraints
    #    
    for constraint in object.constraints:
        elementConstraint = doc.createElement( "constraint" )
        elementObject.appendChild( elementConstraint )
        constraintType = constraint.type
        elementConstraint.setAttribute( "type", constraintType )
        if constraintType == "RIGID_BODY_JOINT":
            pivotType = constraint.pivot_type
            elementConstraint.setAttribute( "pivot", pivotType )
            elementConstraint.setAttribute( "axis_x", ( "%f" % constraint.axis_x ) )
            elementConstraint.setAttribute( "axis_y", ( "%f" % constraint.axis_y ) )
            elementConstraint.setAttribute( "axis_z", ( "%f" % constraint.axis_z ) )
            pivotVector = mathutils.Vector( ( constraint.pivot_x, constraint.pivot_y, constraint.pivot_z ) )
            fixPivot = utilities.FixVector( pivotVector )
            elementConstraint.setAttribute( "pivot_x", ( "%f" % fixPivot[0] ) )
            elementConstraint.setAttribute( "pivot_y", ( "%f" % fixPivot[1] ) )
            elementConstraint.setAttribute( "pivot_z", ( "%f" % fixPivot[2] ) )
            if pivotType == "GENERIC_6_DOF":
                elementConstraint.setAttribute( "limitmaxx", ( "%f" % constraint.limit_max_x ) )
                elementConstraint.setAttribute( "limitminx", ( "%f" % constraint.limit_min_x ) )
                elementConstraint.setAttribute( "limitmaxy", ( "%f" % constraint.limit_max_y ) )
                elementConstraint.setAttribute( "limitminy", ( "%f" % constraint.limit_min_y ) )
                elementConstraint.setAttribute( "limitmaxz", ( "%f" % constraint.limit_max_z ) )
                elementConstraint.setAttribute( "limitminz", ( "%f" % constraint.limit_min_z ) )
                elementConstraint.setAttribute( "target", constraint.target.name )
            elif pivotType == "HINGE":
                elementConstraint.setAttribute( "limit_angle_max_x", ( "%f" % constraint.limit_angle_max_x ) )
                elementConstraint.setAttribute( "limit_angle_min_x", ( "%f" % constraint.limit_angle_min_x ) )
        elif constraintType == "FOLLOW_PATH":
            elementConstraint.setAttribute( "target", str( constraint.target.name ) )
    
    #
    # Type
    #
    if object.proxy == None:
        if gCustomTypeAttribName in object:
            customType = object[gCustomTypeAttribName]
            if customType == "camera_volume":
                cameravolumeexport.ExportCameraVolume( object, doc, elementObject )
            elif customType == "bounding_volume":
                boundingvolumeexport.ExportBoundingVolume( object, doc, elementObject )
            elif customType == "trigger":
                triggerexport.ExportTrigger( object, doc, elementObject )
        else:
            if object.type == "CAMERA":        
                cameraexport.ExportCamera( object, doc, elementObject )
            elif object.type == "LAMP": 
                lightexport.ExportLight( object, doc, elementObject )
            elif object.type == "MESH": 
                meshexport.ExportMesh( object, doc, elementObject )
            elif object.type == "CURVE":
                curveexport.ExportCurve( object, doc, elementObject )
