import Blender
import xml.dom.minidom
from Blender import Mathutils
from Blender.Mathutils import *

import utilities
reload( utilities )

def ExportPhysics( object ):
	
	rotateMatrix = RotationMatrix( 90, 3, 'x' )

	#
	# Create XML file
	#
	doc = xml.dom.minidom.Document()
	
	#
	# Retrieve physics data
	#
	objectProperty = object.getProperty( "volumetype" )
	volumeType = objectProperty.getData()	
	
	if volumeType == "sphere":
		
		rootElement = doc.createElement("spherevolume")
		doc.appendChild( rootElement )
	
		objectTransform = object.getMatrix( 'localspace' )
		newTranslation = utilities.GetTranslation( objectTransform )
		
		element = doc.createElement("translation")
		element.setAttribute("x", str( newTranslation.x ) )
		element.setAttribute("y", str( newTranslation.y ) )
		element.setAttribute("z", str( newTranslation.z ) )
		rootElement.appendChild( element )
					
		element = doc.createElement("rotation")
		rotation = utilities.GetRotationMatrix( objectTransform ).toQuat()
				
		element.setAttribute("x", str( rotation.x ) )
		element.setAttribute("y", str( rotation.y ) )
		element.setAttribute("z", str( rotation.z ) )
		element.setAttribute("w", str( rotation.w ) )
		rootElement.appendChild( element )
		
		element = doc.createElement("attribute")		
		
		mesh = object.getData( mesh = 1 )

		maxVertex = Vector(0,0,0)
		for vertex in mesh.verts:
			newVertex = rotateMatrix * vertex.co;
			if abs( newVertex.x ) > maxVertex.x:
				maxVertex.x = abs( newVertex.x )
			if abs( newVertex.y ) > maxVertex.y:
				maxVertex.y = abs( newVertex.y )
			if abs( newVertex.z ) > maxVertex.z:
				maxVertex.z = abs( newVertex.z )
		
		radius = maxVertex

		element.setAttribute("radiusX", str( radius.x ) )
		element.setAttribute("radiusY", str( radius.y ) )
		element.setAttribute("radiusZ", str( radius.z ) )		
		rootElement.appendChild( element )
		
		
	elif volumeType == "box":
		
		rootElement = doc.createElement("boxvolume")
		doc.appendChild( rootElement )
	
		objectTransform = object.getMatrix( 'localspace' )
				
		newTranslation = GetTranslation( objectTransform )
		
		element = doc.createElement("translation")
		element.setAttribute("x", str( newTranslation.x ) )
		element.setAttribute("y", str( newTranslation.y ) )
		element.setAttribute("z", str( newTranslation.z ) )
		rootElement.appendChild( element )
		
		element = doc.createElement("rotation")
		
		rotation = utilities.GetRotationMatrix( objectTransform).toQuat()
		
		element.setAttribute("x", str( rotation.x ) )
		element.setAttribute("y", str( rotation.y ) )
		element.setAttribute("z", str( rotation.z ) )
		element.setAttribute("w", str( rotation.w ) )
		rootElement.appendChild( element )
		
		element = doc.createElement("attribute")		
		
		mesh = object.getData( mesh = 1 )
		maxVertex = Vector(-999,-999,-999)
		minVertex = Vector( 999, 999, 999)
		for vertex in mesh.verts:
			newVertex = rotateMatrix * vertex.co
			if newVertex.x > maxVertex.x:
				maxVertex.x = newVertex.x
			if newVertex.y > maxVertex.y:
				maxVertex.y = newVertex.y
			if newVertex.z > maxVertex.z:
				maxVertex.z = newVertex.z
			if newVertex.x < minVertex.x:
				minVertex.x = newVertex.x
			if newVertex.y < minVertex.y:
				minVertex.y = newVertex.y
			if newVertex.z < minVertex.z:
				minVertex.z = newVertex.z
		
		lengthX = maxVertex.x - minVertex.x
		lengthY = maxVertex.y - minVertex.y
		lengthZ = maxVertex.z - minVertex.z
		
		element.setAttribute("lengthX", str( lengthX ) )
		element.setAttribute("lengthY", str( lengthY ) )
		element.setAttribute("lengthZ", str( lengthZ ) )		
		rootElement.appendChild( element )
	
	#
	# Write XML file
	#
	physicsFilename = "../../assets/physics/" + object.getName()
	physicsFilename += ".xml"
	
	file_object = open( physicsFilename, "w")
 	file_object.write( doc.toprettyxml() )
	file_object.close()	