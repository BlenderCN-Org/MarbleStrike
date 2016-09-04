import Blender

for object in Blender.Object.GetSelected():
	objProperty = object.getProperty("physics")
	print object
	print objProperty
	print objProperty.getData()