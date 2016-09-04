import os
import sys

currentDirectory = os.getcwd()
os.chdir("../")
rootDirectory = os.getcwd()
os.chdir( currentDirectory )

sys.path.append( rootDirectory + "\\tools\\scripts" )

import generate_project
generate_project.main( "ogles1.xml", "engineogles1.vcxproj", rootDirectory )