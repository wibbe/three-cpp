#! /use/bin/ruby


module UniformType
  FLOAT = 1
  VEC3 = 2
  COLOR = 3
  TEXTURE = 4
end


class Uniform
  attr_accessor :name, :type, :default_value

  def initialize(name, type, default_value)
    @name = name
    @type = type
    @default_value = default_value
  end
end


class Material
  attr_reader :name, :vertex_shader, :fragment_shader, :uniforms, :textures

  def initialize(name)
    @name = name
    @vertex_shader = ""
    @fragment_shader = ""
    @uniforms = []
  end

  def float(name, value = 0.0)
    @uniforms << Uniform.new(name, UniformType::FLOAT, [value])
  end

  def vec3(name, x = 0.0, y = 0.0, z = 0.0)
    @uniforms << Uniform.new(name, UniformType::VEC3, [x, y, z])
  end

  def color(name, r = 0.0, g = 0.0, b = 0.0, a = 0.0)
    @uniforms << Uniform.new(name, UniformType::COLOR, [r, g, b, a])
  end

  def texture(name)
    @uniforms << Uniform.new(name, UniformType::TEXTURE, [])
  end

  def vertex_shader(name)
    @vertex_shader = name
  end

  def fragement_shader(name)
    @fragement_shader = name
  end
end


class Shader
  attr_reader :name

  def initialize(name, text_lines, snipets)
    @name = name
    @text_lines = text_lines
    @snipets = snipets
  end
end


class CodeLib
  def initialize(source_path, target_path)
    @shaders = []
    @materials = []
    @snipets = {}
    @source_path = source_path
    @target_path = target_path
  end

  def snipet(name, text_lines)
    @snipets[name] = text_lines
  end

  def shader(name, text_lines)
    shader = Shader.new(name, text_lines, @snipets)
    @shaders << shader
  end

  def material(name, &block)
    material = Material.new(name)
    material.instance_eval(&block)
    @materials << material
  end
end