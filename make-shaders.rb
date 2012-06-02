#! /use/bin/ruby

in_file = "src/GLShaders.glsl"
out_file = "src/GLShaders.h"

has_variable = false

current_fragment = ""
fragments = {}

File.open(out_file, "w") do |out|
  out.puts "#pragma once"
  out.puts ""
  out.puts "namespace three {"
  out.puts ""
  File.open(in_file, "r") do |shader|
    shader.each do |line|
      # Start of a new code fragment
      if (line.start_with? "@")
        if current_fragment != ""
          out.puts "    \"\";"
          out.puts ""
        end
        name = line[1..-1].delete "\n"
        current_fragment = name

        fragments[name] = []

        out.puts "  static const char * #{name} = "
      elsif line.lstrip().start_with? "="
        name = line.lstrip()[1..-1].delete "\n"

        indent = "    \"" + " " * line.rindex("=")

        if fragments.include? name
          fragments[name].each do |fragment|
            out.puts indent + fragment[1..-1]
            if current_fragment != ""
              fragments[current_fragment] << indent + fragment
            end
          end
        end
      elsif line.start_with? "//"
        # Ignore comments
      elsif current_fragment != "" and line != "\n"
        code_line = "\"#{line.delete "\n"}\\n\"" 
        fragments[current_fragment] << code_line
        out.puts "    " + code_line
      end
    end
  end

  if current_fragment != ""
    out.puts "    \"\";"
  end

  out.puts ""
  out.puts "}"
  out.puts ""
end

