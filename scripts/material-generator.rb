#! /use/bin/ruby

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

require "optparse"

# -- Argument parsing --

def parse_args
  options = {}
  options[:files] = []
  options[:path] = ""
  start = 0

  opts = OptionParser.new do |opts|
    opts.banner = "Usage: material-generator.rb [options] file1 file2 ..."
    opts.separator ""
    opts.separator "Options:"

    opts.on('-p', '--path PATH', 'Where the generated code goes (src/ and include/ paths will be appended to the specified path)') do |v|
      options[:path] = v
      start += 2
    end

    opts.on_tail('-h', '--help', 'Show this message') do
      puts opts
      exit      
    end

    opts.parse(ARGV)

    options[:files] = ARGV[start, ARGV.length - start]
    options
  end

  return options
end

options = parse_args
if options[:path] == ""
  puts "Error: Need to specify a target path"
  exit
end


# -- Generator lib --

module UniformType
  FLOAT = 1
  VEC3 = 2
  VEC4 = 3
  COLOR = 4
end


class Uniform
  attr_accessor :name, :type, :default_value

  def initialize(name, type, default_value)
    @name = name
    @type = type
    @default_value = default_value
  end

  def definition
    str = ""

    str = "float" if @type == UniformType::FLOAT
    str = "three::Vector3" if @type == UniformType::VEC3
    str = "three::Vector4" if @type == UniformType::VEC4
    str = "three::Color" if @type == UniformType::COLOR

    str += " #{@name}"
    return str
  end

  def default_declaration
    str = ""

    str = "#{@default_value[0]}" if @type == UniformType::FLOAT
    str = "three::Vector3(#{@default_value[0]}, #{@default_value[1]}, #{@default_value[2]})" if @type == UniformType::VEC3
    str = "three::Vector4(#{@default_value[0]}, #{@default_value[1]}, #{@default_value[2]}, #{@default_value[3]})" if @type == UniformType::VEC4
    str = "three::Color(#{@default_value[0]}, #{@default_value[1]}, #{@default_value[2]}, #{@default_value[3]})" if @type == UniformType::COLOR
    return str
  end
end


class Material
  attr_reader :name, :vertex_shader, :fragment_shader, :uniforms, :textures

  def initialize(name)
    @name = name
    @vertex_shader = ""
    @fragment_shader = ""
    @uniforms = []
    @defines = []
    @textures = []
  end

  def float(name, value = 0.0)
    @uniforms << Uniform.new(name, UniformType::FLOAT, [value])
  end

  def vec3(name, x = 0.0, y = 0.0, z = 0.0)
    @uniforms << Uniform.new(name, UniformType::VEC3, [x, y, z])
  end

  def vec4(name, x = 0.0, y = 0.0, z = 0.0, w = 0.0)
    @uniforms << Uniform.new(name, UniformType::VEC4, [x, y, z, w])
  end

  def color(name, r = 0.0, g = 0.0, b = 0.0, a = 0.0)
    @uniforms << Uniform.new(name, UniformType::COLOR, [r, g, b, a])
  end

  def texture(name)
    @textures << name
  end

  def define(*names)
    names.each { |x| @defines << x}
  end

  def vertex_shader(name)
    @vertex_shader = name
  end

  def fragment_shader(name)
    @fragment_shader = name
  end

  def generate_definition(file)
    file.puts "  class #{@name} : public three::Material"
    file.puts "  {"
    file.puts "    public:"
    file.puts "      static uint32_t Type;"
    file.puts ""
    file.puts "    public:"
    file.puts "      #{@name}();"
    file.puts ""
    file.puts "      uint32_t type() const { return #{@name}::Type; }"
    file.puts ""
    file.puts "      void apply(Renderer * renderer);"
    file.puts ""
    file.puts "      std::string vertexShaderCode() const;"
    file.puts "      std::string fragmentShaderCode() const;"
    file.puts ""
    file.puts "      const char * textureName(uint32_t slot) const;"
    file.puts "      const char * uniformName(uint32_t slot) const;"
    file.puts "      uint32_t textureCount() const { return #{@textures.length}; }"
    file.puts "      uint32_t uniformCount() const { return #{@uniforms.length}; }"
    file.puts ""
    file.puts "    public:"
    @textures.each { |texture| file.puts "      three::Texture * #{texture};" }
    @uniforms.each { |uniform| file.puts "      #{uniform.definition};"}
    file.puts "  };"
    file.puts ""
  end

  def generate_implementation(file)
    file.puts ""
    file.puts "  uint32_t #{@name}::Type = three::StringHash(\"#{@name}\").hash;"
    file.puts ""
    file.puts "  #{@name}::#{@name}()"
    file.puts "  {"
    @textures.each { |x| file.puts "    #{x} = 0;"}
    @uniforms.each { |x| file.puts "    #{x.name} = #{x.default_declaration};"}
    file.puts "  }"
    file.puts ""
    file.puts "  void #{name}::apply(Renderer * renderer)"
    file.puts "  {"
    file.puts "    assert(__renderMaterial && renderer);"
    @uniforms.each_index do |i|
      file.puts "    __renderMaterial->uniform(#{i}, #{@uniforms[i].name});"
    end
    @textures.each_index do |i|
      file.puts "    renderer->setTexture(#{@textures[i]}, #{i});"
    end
    file.puts "  }"
    file.puts ""
    file.puts "  std::string #{@name}::vertexShaderCode() const"
    file.puts "  {"
    file.puts "    return three::Code::generate(\"#{@vertex_shader}\", #{@defines.collect() { |x| "\"#{x}\"" } .join(', ')}, 0);"
    file.puts "  }"
    file.puts ""
    file.puts "  std::string #{@name}::fragmentShaderCode() const"
    file.puts "  {"
    file.puts "    return three::Code::generate(\"#{@fragment_shader}\", #{@defines.collect() { |x| "\"#{x}\"" } .join(', ')}, 0);"
    file.puts "  }"
    file.puts ""
    file.puts "  const char * #{@name}::textureName(uint32_t slot) const"
    file.puts "  {"
    file.puts "    switch (slot)"
    file.puts "    {"
    @textures.each_index do |i|
      file.puts "      case #{i}:"
      file.puts "        return \"#{@textures[i]}\";"
    end
    file.puts "      default:"
    file.puts "        return 0;"
    file.puts "    }"
    file.puts "  }"
    file.puts ""
    file.puts "  const char * #{@name}::uniformName(uint32_t slot) const"
    file.puts "  {"
    file.puts "    switch (slot)"
    file.puts "    {"
    @uniforms.each_index do |i|
      file.puts "      case #{i}:"
      file.puts "        return \"#{@uniforms[i].name}\";"
    end
    file.puts "      default:"
    file.puts "        return 0;"
    file.puts "    }"
    file.puts "  }"
    file.puts ""
  end
end


class CodeLib
  def initialize(target_name, namespace)
    @materials = []
    @shaders = {}
    @snipets = {}
    @target_name = target_name
    @namespace = namespace
    @level = 0
  end

  def snipet(name, text_lines)
    @snipets[name] = text_lines
  end

  def shader(name, text_lines)
    @shaders[name] = text_lines
  end

  def material(name, &block)
    material = Material.new(name)
    material.instance_eval(&block)
    @materials << material
  end

  def generate_code(base_path)
    header_file = File.join(base_path, 'include', @target_name + ".h")
    source_file = File.join(base_path, 'src', @target_name + ".cpp")

    if not @materials.empty? or not @snipets.empty? or not @shaders.empty?
      File.open(header_file, "w") do |file|
        generate_header(file)
      end
    end

    File.open(source_file, "w") do |file|
      generate_source(header_file, file)
    end
  end

  def generate_header(file)
    file.puts ""
    file.puts "#pragma once"
    file.puts ""

    if not @materials.empty?
      file.puts '#include "Material.h"'
      file.puts ""
    end

    begin_namespace file
    file.puts ""

    if not @snipets.empty? or not @shaders.empty?
      file.puts "  void initialize#{@target_name}();"
      file.puts ""
    end

    @materials.each { |mat| mat.generate_definition(file) }

    end_namespace file
  end

  def generate_source(header_file, file)
    file.puts ""
    file.puts "#include \"#{File.basename(header_file)}\"" if not @materials.empty?
    file.puts '#include "Code.h"'
    file.puts '#include "RenderMaterial.h"'
    file.puts '#include "StringHash.h"'
    file.puts '#include "Renderer.h"'
    file.puts '#include <cassert>'
    file.puts ""
    begin_namespace(file)
    file.puts ""

    generate_shader_code(@snipets, file, true, true) if not @snipets.empty?
    generate_shader_code(@snipets, file, false, true) if not @snipets.empty?

    generate_shader_code(@shaders, file, true, false) if not @shaders.empty?
    generate_shader_code(@shaders, file, false, false) if not @shaders.empty?

    if not @snipets.empty? or not @shaders.empty?
      file.puts "  void initialize#{@target_name}()"
      file.puts "  {"
      file.puts "    // Does nothing, just here so we have something to link agains"
      file.puts "  }"
    end

    @materials.each { |mat| mat.generate_implementation(file) }

    end_namespace(file)
  end

  def generate_shader_code(snipets, file, generate_text, is_snipets)
    snipets.each do |key, value|
      name = key.to_s

      if generate_text
        file.puts "  static const char * _text_#{name}[] = {"

        @level = 0
        value.each do |line|
          if @snipets.member? line
            file.puts "    \"@#{line}\","
          else
            file.puts '    "' + indent(line) + '\n",'
          end
        end

        file.puts "  };"
        file.puts ""
      else
        file.puts "  three::Code _code_#{name}(\"#{is_snipets ? "@" + name : name}\", _text_#{name}, #{value.length});"
      end
    end

    if not generate_text
      file.puts ""
    end
  end

  def begin_namespace(file)
    file.puts(@namespace.collect { |x| "namespace #{x} {"} .join(" "))
  end

  def end_namespace(file)
    file.puts("}" * @namespace.length)
  end

  def indent(line)
    indent = "  " * @level

    if line.start_with? "{" or line.end_with? "{" or line.start_with? "#ifdef"
      @level += 1
    elsif line.start_with? "}" or line.start_with? "#end"
      @level -= 1
      indent = "  " * @level
    elsif line.start_with? "#else"
      indent = "  " * (@level - 1)
    end

    return indent + line
  end
end

# -- Main --

def define(target_name, namespace, &block)
  options = parse_args

  lib = CodeLib.new(target_name, namespace)
  lib.instance_eval(&block)
  lib.generate_code(options[:path])
end

options[:files].each do |file|
  puts "Processing file #{file}"
  require(file)
end