#!/usr/bin/env ruby
require 'json'

CC = '/usr/bin/cc'
SYSROOT = '/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk'

def list_csources(dir)
  return Dir.glob("#{dir}/**/*.c", File::FNM_DOTMATCH)
end

def to_compile_commands(clist)
  return clist.map { |cfile|
    {
      directory: '.',
      file: cfile,
      arguments: [
        CC,
        cfile,
        '-std=c11',
        '-I',
        'src/',
        '-c',
        '-o',
        cfile.gsub(/\.c$/, '.o'),
        'isysroot',
        SYSROOT
      ]
    }
  }
end

src = to_compile_commands(list_csources('src'))
File.open('compile_commands.json', 'w') do |f|
  f.puts(JSON.pretty_generate(src))
end
