# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8-80 compliant>

bl_info = {
    "name": "Game Engine (XML)",
    "author": "",
    "blender": (2, 5, 8),
    "api": 35622,
    "location": "File > Import-Export",
    "description": "Export XML",
    "warning": "",
    "wiki_url": "http://wiki.blender.org/index.php/Extensions:2.5/Py/",
    "tracker_url": "",
    "support": 'OFFICIAL',
    "category": "Import-Export"}

import bpy
from bpy.props import BoolProperty, FloatProperty, StringProperty, EnumProperty
from bpy_extras.io_utils import ExportHelper, ImportHelper, path_reference_mode, axis_conversion


class ImportXML(bpy.types.Operator, ImportHelper):
    '''Load a Game Engine File'''
    bl_idname = "import_scene.xml"
    bl_label = "Import XML"
    bl_options = {'PRESET'}
    
    filename_ext = ".xml"
    filter_glob = StringProperty(default="*.xml;", options={'HIDDEN'})

    def execute(self, context):
        # print("Selected: " + context.active_object.name)
        return {'FINISHED'}

class ExportXMLPanel(bpy.types.Panel):
    bl_space_type = "VIEW_3D"
    bl_region_type = "TOOLS"
    bl_context = "objectmode"
    bl_label = "Export XML"
 
    def draw(self, context):
        TheCol = self.layout.column(align = True)
        TheCol.operator("export_scene.xml", text = "Export XML")

class ExportXML(bpy.types.Operator, ExportHelper):
    '''Save a Game Engine File'''
    bl_idname = "export_scene.xml"
    bl_label = 'Export XML'
    bl_options = {'PRESET'}
    
    filename_ext = ".xml"
    filter_glob = StringProperty(default="*.xml", options={'HIDDEN'})

    def execute(self, context):
        import sys
        sys.path.append("F:\\depotgame\\game\\tools\\blenderscripts")
        #sys.path.append("/Volumes/SHARED/depotgame/game/tools/blenderscripts")
        import main
        import imp
        imp.reload( main )
        main.export_scene(self, context, **self.as_keywords(ignore=("check_existing", "filter_glob")))
        return {'FINISHED'}


def menu_func_import(self, context):
    self.layout.operator(ImportXML.bl_idname, text="GameEngine (.xml)")


def menu_func_export(self, context):
    self.layout.operator(ExportXML.bl_idname, text="GameEngine (.xml)")


def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_import.append(menu_func_import)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_import.remove(menu_func_import)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


# CONVERSION ISSUES
# - matrix problem
# - duplis - only tested dupliverts
# - all scenes export
# + normals calculation

if __name__ == "__main__":
    register()
