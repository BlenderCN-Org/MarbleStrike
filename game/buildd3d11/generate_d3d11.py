import os
import sys

currentDirectory = os.getcwd()
os.chdir("../")
rootDirectory = os.getcwd()
os.chdir( currentDirectory )

sys.path.append( rootDirectory + "\\tools\\scripts" )

import generate_project
generate_project.main( "pcd3d11.xml", "engined3d11.vcxproj", rootDirectory )