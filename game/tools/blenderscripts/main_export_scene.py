#!BPY
"""
Name: 'Export Scene'
Blender: 244
Group: 'Export'
"""
import Blender

import sys
sys.path.append("..\\..\\tools\\blenderscripts")

import main
reload( main )

main.export_scene()