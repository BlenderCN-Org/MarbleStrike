import os
import sys

currentDirectory = os.getcwd()
os.chdir("../")
rootDirectory = os.getcwd()
os.chdir( currentDirectory )

sys.path.append( rootDirectory + "\\tools\\scripts" )

import generate_project
generate_project.main( "ogles2.xml", "engineogles2.vcxproj", rootDirectory )