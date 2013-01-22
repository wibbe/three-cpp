# Copyright (c) 2012 Daniel Wiberg
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

define 'DefaultMaterials', ['three'] do

  material 'MeshBasicMaterial' do
    vertex_shader :basicVertexShader
    fragment_shader :basicFragmentShader

    option :useTextureMap, :USE_MAP, false
    option :useLightMap, :USE_LIGHTMAP, false
    option :useEnvMap, :USE_ENVMAP, false
    option :doubleSided, :DOUBLE_SIDED, false
    option :useVertexColor, :USE_COLOR, false
    option :gammaCorrection, :USE_GAMMA, false
    option :useReflectivity, :USE_REFLECTIVITY, false

    texture :map
    texture :lightMap
    texture :envMap

    vec4 :offsetRepeat, 0, 0, 1, 1
    color :diffuse, 1, 1, 1, 1
    float :opacity, 1
    float :flipEnvMap, -1
    int :useRefract, 0
    float :reflectivity, 1.0
    float :refractionRatio, 0.98
  end
end