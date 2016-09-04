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

def ExportMesh( object, doc, rootElement ):

    rotate90Matrix = mathutils.Matrix.Rotation( radians(-90), 4, 'X' )
    
    mesh = object.data

    meshElement = doc.createElement( "mesh" )
    meshElement.setAttribute( "name", mesh.name )
    meshElement.setAttribute( "draw_type", object.draw_type );
    rootElement.appendChild( meshElement )

    #
    # VERTICES
    #
    smoothVertexList = []
    for vertex in mesh.vertices:
        vertexItem = {}
        vertexItem["position"] = vertex.co
        vertexItem["normal"] = vertex.normal
        smoothVertexList.append( vertexItem )
            
    #
    # FACES
    #
    vertexList = []
    faceList = []        
    faceCount = 0
    for face in mesh.polygons:        
        newFace = []
            
        numFaceVertices = len( face.vertices )
        
        if object.draw_type != "WIRE":
            assert numFaceVertices == 3, "Exporting non triangle mesh: %s" % mesh.name
        
        if face.use_smooth:
            vertexCount = 0
            for vertexIndex in face.vertices:                
                vertex = mesh.vertices[vertexIndex]
                if "texcoord" in smoothVertexList[vertexIndex]:
                    texcoord = mesh.uv_layers[0].data[faceCount*numFaceVertices + vertexCount].uv
                    texcoordVector = mathutils.Vector( (texcoord[0],texcoord[1]) )
                    if smoothVertexList[vertexIndex]["texcoord"] != texcoordVector:
                        vertexItem = {}
                        vertexItem["position"] = vertex.co
                        vertexItem["normal"] = vertex.normal
                        vertexItem["texcoord"] = texcoordVector

                        if len( mesh.vertex_colors ) > 0:
                            if vertexCount == 0:                        
                                vertexItem["color"] = mesh.vertex_colors[0].data[faceCount].color1;
                            elif vertexCount == 1:
                                vertexItem["color"] = mesh.vertex_colors[0].data[faceCount].color2;
                            elif vertexCount == 2:
                                vertexItem["color"] = mesh.vertex_colors[0].data[faceCount].color3;                        
                        smoothVertexList.append( vertexItem )
                        newIndex = len( smoothVertexList ) - 1
                        newFace.append( newIndex )                    
                    else:
                        newFace.append( vertexIndex )                
                else:                
                    if len( mesh.uv_layers ) > 0:
                        texcoord = mesh.uv_layers[0].data[faceCount*numFaceVertices + vertexCount].uv
                        smoothVertexList[vertexIndex]["texcoord"] = mathutils.Vector( (texcoord[0],texcoord[1]) )

                    if len( mesh.vertex_colors ) > 0:
                        vertItem = smoothVertexList[vertexIndex];
                        vertItem["color"] = mesh.vertex_colors[0].data[faceCount].color;

                    newFace.append( vertexIndex )   
                vertexCount += 1
        else:
            vertexCount = 0
            for vertexIndex in face.vertices:
                
                vertex = mesh.vertices[vertexIndex]
                
                vertexItem = {}
                vertexItem["position"] = vertex.co
                vertexItem["normal"] = face.normal
                
                if len( mesh.vertex_colors ) > 0:
                    vertexItem["color"] = mesh.vertex_colors[0].data[faceCount].color;                   

                vertexList.append( vertexItem )
                
                index = len( vertexList ) - 1
                
                if "texcoord" in vertexList[index]:
                    texcoord = mesh.uv_textures[0].data[faceCount*numFaceVertices + vertexCount].uv
                    texcoordVector = mathutils.Vector( (texcoord[0],texcoord[1]) )
                    if vertexList[index]["texcoord"] != texcoordVector:
                        vertexItem = {}
                        vertexItem["position"] = vertex.co
                        vertexItem["normal"] = vertex.normal
                        vertexItem["texcoord"] = texcoordVector
                        
                        if len( mesh.vertex_colors ) > 0:
                            vertexItem["color"] = mesh.vertex_colors[0].data[faceCount].color;

                        vertexList.append( vertexItem )
                        newIndex = len( vertexList ) - 1
                        newFace.append( newIndex )                    
                    else:
                        newFace.append( index )                
                else:                
                    if len( mesh.uv_layers ) > 0:
                        texcoord = mesh.uv_layers[0].data[faceCount*numFaceVertices + vertexCount].uv
                        vertexList[index]["texcoord"] = mathutils.Vector( (texcoord[0],texcoord[1]) )
                    newFace.append( index )   
                vertexCount += 1
            
        faceCount += 1
        faceList.append( newFace )
   
    if len( vertexList ) == 0:
        for vertexItem in smoothVertexList:
            vertexList.append( vertexItem )
   
    #
    # OUTPUT VERTICES
    #
    meshElement.setAttribute( "vertexcount", str( len( vertexList ) ) )
    
    positionElement = doc.createElement( "pos" )
    meshElement.appendChild( positionElement )
    positionElement.setAttribute( "stride", "3" )
        
    normalElement = doc.createElement( "nor" )
    meshElement.appendChild( normalElement )
    normalElement.setAttribute( "stride", "3" )
    
    tangentElement = doc.createElement( "tan" )
    meshElement.appendChild( tangentElement )
    tangentElement.setAttribute( "stride", "3" )
        
    colorElement = doc.createElement( "col" )
    meshElement.appendChild( colorElement )
    colorElement.setAttribute( "stride", "4" )
    
    texCoordElement = doc.createElement( "tex" )
    meshElement.appendChild( texCoordElement )
    texCoordElement.setAttribute( "stride", "2" )
    
    positionString = ""
    normalString = ""
    texCoordString = ""
    colorString = ""
    tangentString = ""
    
    for vertItem in vertexList:
        newPosition = vertItem["position"]
        newPosition = rotate90Matrix * newPosition
        positionString += ( "%f" % newPosition.x )
        positionString += " "
        positionString += ( "%f" % newPosition.y )
        positionString += " "
        positionString += ( "%f" % newPosition.z )
        positionString += " "
        
        newNormal = vertItem["normal"]
        newNormal = rotate90Matrix * newNormal
        normalString += ( "%f" % newNormal.x )
        normalString += " "
        normalString += ( "%f" % newNormal.y )
        normalString += " "
        normalString += ( "%f" % newNormal.z )
        normalString += " "
        
        newTangent = mathutils.Vector((0,0,0))
        newTangent = rotate90Matrix * newTangent
        tangentString += ("%i" %  newTangent.x )
        tangentString += " "
        tangentString += ( "%i" % newTangent.y )
        tangentString += " "
        tangentString += ( "%i" % newTangent.z )
        tangentString += " "

        if "color" in vertItem:
            newColor = mathutils.Vector((1,1,1,1))
            newColor[0] = vertItem["color"][0]
            newColor[1] = vertItem["color"][1]
            newColor[2] = vertItem["color"][2]
        else:
            newColor = mathutils.Vector((1,1,1,1))
        value = 1
        colorString += ( "%f" % newColor[0] )
        colorString += " "
        colorString += ( "%f" % newColor[1] )
        colorString += " "
        colorString += ( "%f" % newColor[2] )
        colorString += " "
        colorString += ( "%i" % newColor[3] )        
        colorString += " "
        
        texCoord = []
        if "texcoord" in vertItem: 
            texCoord.append( vertItem["texcoord"].x )
            texCoord.append( vertItem["texcoord"].y )
        else:
            texCoord.append( 0 )
            texCoord.append( 0 )
        
        texCoordString += ( "%f" % texCoord[0] )
        texCoordString += " "
        texCoordString += ( "%f" % texCoord[1] )
        texCoordString += " "
                        
    textNode = doc.createTextNode( positionString )    
    positionElement.appendChild( textNode )    
    
    textNode = doc.createTextNode( normalString )    
    normalElement.appendChild( textNode )    
    
    textNode = doc.createTextNode( tangentString )    
    tangentElement.appendChild( textNode )    
    
    textNode = doc.createTextNode( texCoordString )
    texCoordElement.appendChild( textNode )
    
    textNode = doc.createTextNode( colorString )
    colorElement.appendChild( textNode )
        
    #
    # Output Faces
    #
    faceIndicesElement = doc.createElement( "faceidx" )
    meshElement.appendChild( faceIndicesElement )
        
    if object.draw_type == "WIRE":    
        faceIndicesElement.setAttribute( "stride", "8" )    
        faceIndicesElement.setAttribute( "count", str( len( faceList ) ) )
        faceIndicesString = ""    
        for face in faceList:        
            faceIndicesString += str( face[0] )
            faceIndicesString += " "
            faceIndicesString += str( face[1] )
            faceIndicesString += " "
            faceIndicesString += str( face[1] )
            faceIndicesString += " "    
            faceIndicesString += str( face[2] )
            faceIndicesString += " "
            faceIndicesString += str( face[2] )
            faceIndicesString += " "
            faceIndicesString += str( face[3] )
            faceIndicesString += " "    
            faceIndicesString += str( face[3] )
            faceIndicesString += " "    
            faceIndicesString += str( face[0] )
            faceIndicesString += " "    
    else:
        faceIndicesElement.setAttribute( "stride", "3" )    
        faceIndicesElement.setAttribute( "count", str( len( faceList ) ) )
        faceIndicesString = ""    
        for face in faceList:        
            faceIndicesString += str( face[0] )
            faceIndicesString += " "
            faceIndicesString += str( face[1] )
            faceIndicesString += " "
            faceIndicesString += str( face[2] )
            faceIndicesString += " "
        
    textNode = doc.createTextNode( faceIndicesString )    
    faceIndicesElement.appendChild( textNode )    

    #
    # Materials
    #    
    materialSize = len( mesh.materials )
    assert materialSize > 0, "No materials for %s" % mesh.name
    
    if materialSize > 0:
        material = mesh.materials[0]        
        meshElement.setAttribute( "material", material.name )