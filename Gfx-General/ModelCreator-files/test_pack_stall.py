from structs import Mesh
from structs import CURRENT_VERSION
from structs import TexXYFilterMode
from structs import TexMipFilterMode
from structs import TexAnisoRatio
from structs import TexWrapMode
from structs import Texture
from structs import RenderFlags
from structs import CompareFunc
from structs import CullingMode
from structs import BlendFactor
from structs import BlendEquation
from structs import StencilOp
from structs import PolygonMode
from structs import Material
from structs import Model

from obj_reader import read_obj
from packer import pack


vertices, indices = read_obj("test/stall.obj")

mesh = Mesh()
mesh.vertices = vertices
mesh.indices = indices
mesh.materialIdx = 0

texture = Texture()
texture.name = "stallTexture"
texture.samplerName = "texture1"
texture.wrapX = TexWrapMode.TEX_WRAP_MODE_REPEAT
texture.wrapY = TexWrapMode.TEX_WRAP_MODE_REPEAT
texture.mipFilter = TexMipFilterMode.TEX_MIP_FILTER_MODE_NONE

material = Material()
material.name = "stallMaterial"
material.shaderName = "test_shader"
material.textures = [texture]

model = Model()
model.meshes = [mesh]
model.materials = [material]

dataLE = pack(model, '<')
with open("test/stall_LE.rmdl", "wb") as outf:
    outf.write(dataLE)

dataBE = pack(model, '>')
with open("test/stall_BE.rmdl", "wb") as outf:
    outf.write(dataBE)
