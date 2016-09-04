import os
import sys
import distutils.dir_util
from stat import *
from shutil import *

blenderScriptFile = "__init__.py"
destinationScriptFile = "F:\\blender\\2.64\\scripts\\addons\\io_scene_xml\\__init__.py"

copyfile( blenderScriptFile, destinationScriptFile )