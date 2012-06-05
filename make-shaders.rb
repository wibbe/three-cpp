#! /use/bin/ruby


class CodeLib
  def initialize(target)
    @target = target
    @snipets = {}
    @level = 0
  end

  def snipet(name, text)
    @snipets[name] = text
  end

  # Used to generate a shader definition
  def shader(name, text)
    @target.puts "  static const char * #{name} =" 

    text.each do |line|
      if @snipets.member? line
        snipet = @snipets[line]
        snipet.each { |l| @target.puts '    "' + __indent(l) + l + '\n"' }
      else
        @target.puts '    "' + __indent(line) + line + '\n"'
      end
    end

    @target.puts "    \"\";"
    @target.puts ""
  end

  # A really simple "pretty-printer" for indentation
  def __indent(line)
    indent = "  " * @level

    if line.start_with? "{" or line.start_with? "#ifdef"
      @level += 1
    elsif line.start_with? "}" or line.start_with? "#end"
      @level -= 1
      indent = "  " * @level
    elsif line.start_with? "#else"
      indent = "  " * (@level - 1)
    end

    return indent
  end
end

def define(out_file, &block)
  File.open(out_file, "w") do |file|
    lib = CodeLib.new(file)

    puts "Generating #{out_file}"

    file.puts "#pragma once"
    file.puts ""
    file.puts "namespace three {"
    file.puts ""

    lib.instance_eval(&block)

    file.puts ""
    file.puts "}"
    file.puts ""
  end
end

#Evaluate the file
require 'src/GLShaders.rb'

puts "Done"

