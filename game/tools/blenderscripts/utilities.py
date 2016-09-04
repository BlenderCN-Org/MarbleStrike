import mathutils
from math import radians

def HasProperty( object, name ):
    
    allProperties = object.getAllProperties()
    for property in allProperties:
        if property.getName() == name:
            return True
    return False

#def GetTranslation( transform ):

#    rotate90Matrix = mathutils.Matrix.Rotation( radians(-90), 4, 'X' )
    
#    newTranslation = transform.to_translation() * rotate90Matrix
    
#    return newTranslation
        
#def GetRotationMatrix( transform ):

#    rotate90Matrix = mathutils.Matrix.Rotation( radians(-90), 3, 'X' )
    
#    rotationMatrix = rotate90Matrix * transform.to_3x3()
    
#    return rotationMatrix

def FixVector( vector ):
    rotate90Matrix = mathutils.Matrix.Rotation( radians(-90), 4, 'X' )

    newVector = rotate90Matrix * vector

    return newVector
    
def FixTransform( transform  ):

    rotate90Matrix = mathutils.Matrix.Rotation( radians(-90), 4, 'X' )
    
    newTransform = rotate90Matrix * transform;

    decompose = transform.decompose()
    location = decompose[0]
    rotation = decompose[1]
    scale = decompose[2]

    location = rotate90Matrix * location
    euler = rotation.to_euler()

    mat_loc = mathutils.Matrix.Translation(location)
    x_rot = mathutils.Matrix.Rotation( euler.x, 4, 'X')
    z_rot = mathutils.Matrix.Rotation( euler.z, 4, 'Y')
    y_rot = mathutils.Matrix.Rotation( euler.y, 4, 'Z')

    rotation = z_rot * y_rot * x_rot
    
    newTransform = mat_loc * rotation
    
    return newTransform

def GetScale( transform ):
            
    scaleBlender = transform.to_scale()
    scale = transform.to_scale()
    scale[0] = scaleBlender[0]
    scale[1] = scaleBlender[1]
    scale[2] = scaleBlender[2]    
    
    return scale
    
def WriteAnimationData( elementObject, doc, data ):
    animationData = data
    if animationData != None:
        animationAction = animationData.action
        if animationAction != None:
            animationElement = doc.createElement( "animation" )
            elementObject.appendChild( animationElement )
            animationElement.setAttribute( "start", str( animationAction.frame_range[0] ) );
            animationElement.setAttribute( "end", str( animationAction.frame_range[1] ) );
            curves = animationAction.fcurves
            for curve in curves:
                curveElement = doc.createElement( "curve" )
                animationElement.appendChild( curveElement )
                curveElement.setAttribute( "name", curve.data_path )
                
                dataIndex = curve.array_index
                if dataIndex == 1:
                    dataIndex = 2
                elif dataIndex == 2:
                    dataIndex = 1
                   
                curveElement.setAttribute( "index", str( dataIndex ) )
                curveElement.setAttribute( "keys", str( len( curve.keyframe_points ) ) )
                keyFrames = curve.keyframe_points
                
                stringData = ""                
                for keyFrame in keyFrames:
                    centerKeyFrameData = keyFrame.co.y
                    leftKeyFrameData = keyFrame.handle_left.y
                    rightKeyFrameData = keyFrame.handle_right.y
                    if dataIndex == 2:
                        centerKeyFrameData *= -1
                        leftKeyFrameData *= -1
                        rightKeyFrameData *= -1
                    stringData += ( "%f" % keyFrame.co.x ) + " " + ( "%f" % centerKeyFrameData ) + " " + ( "%f" % keyFrame.handle_left.x ) + " " + ( "%f" % leftKeyFrameData ) + " " + ( "%f" % keyFrame.handle_right.x ) + " " + ( "%f" % rightKeyFrameData ) + " "
                textNode = doc.createTextNode( stringData )
                curveElement.appendChild( textNode );