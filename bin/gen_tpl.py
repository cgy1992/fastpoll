import sys
import re
import argparse
import collections
from enum import IntEnum

# TODO: more arguments (render function prefix, source-file, header, ..)

class Flags(IntEnum):
  InDef = 1
  InString = 2
  InNodeDecl = 4

def generate_header(tof):
  tof.write('#pragma once\n\n')

  tof.write('#include "fsp.h"\n\n')

def generate_macro(tof, const):
  max_len = 80 - 2 # SPACE '\'
  spacing = len("#define " + const['name'].upper()) + 1
  len_per_line = max_len - spacing
  first_line = True

  if(len(const['name']) > 0 and len(const['definition']) > 0):
    tof.write("#define " + const['name'].upper() + " ")

    while len(const['definition']) > 0:
      if(first_line == False):
        tof.write(spacing * ' ')

      tof.write('"' + const['definition'][0:len_per_line] + '"')

      if(len(const['definition']) >= len_per_line):
        tof.write(' \\')

      tof.write('\n')

      const['definition'] = const['definition'][len_per_line:]
      first_line = False

def generate_function(tof, const):
  fn_head = 'void fsp_render_' + const['name'].lower()
  fn_args = collections.OrderedDict()

  for arg in const['args']:
    fn_args[arg[1]] = {'d': 'int', 's': 'char*'}[arg[5]]

  #fn_args.keys().sort()
  tof.write('\n' + fn_head + '(' + ', '.join([(fn_args[k] + ' ' + k) for k in fn_args]) + ')\n{\n')
  
  tof.write('  fsp_printf((void*)0, "' + const['definition'] + '", ')
  tof.write(', '.join([(const['args'][i][1]) for i in range(len(const['args']))]))
  tof.write(');\n')

  tof.write('}\n\n')

if __name__ == '__main__':
  consts = [{'name': '', 'args': [], 'definition': ''}]

  parser = argparse.ArgumentParser()
  parser.add_argument('-i', '--input', metavar="tdf", type=str, nargs='+', required=True)
  parser.add_argument('-o', '--output', metavar="tof", type=str, nargs=1, required=True)

  args = parser.parse_args()

  for tdf_path in args.input:
    with open(tdf_path, "r", encoding="utf-8") as tdf:
      lines = tdf.readlines()

      if len(lines) < 1:
        sys.exit(1)

      nl_cnt = 0
      flags = 0

      # parse lines 
      for line in lines:
        for ch in line:
          if(flags & Flags.InDef == False):
            if(nl_cnt > 0):
              nl_cnt = 0
              consts.append({'name': '', 'definition': ''})

            if(ch != ':'):
              if(ch not in [' ', '\t', '\n']):
                consts[-1]['name'] += ch 
            else:
              flags |= Flags.InDef

          else:
            if(ch == '\n'):
              nl_cnt += 1

              if(nl_cnt == 2):
                flags ^= Flags.InDef

            else:
              if(ch in ['\'', '"']):
                flags ^= Flags.InString

              elif(ch in ['<', '>']):
                flags ^= Flags.InNodeDecl

              if(ch not in ['\t', '\n']):
                nl_cnt = 0

                if(ch == ' '):
                  if(Flags.InNodeDecl & flags != True and consts[-1]['definition'][-1:] is ' '):
                    continue

                consts[-1]['definition'] += ch

  # write template makros
  with open(args.output[0], "w", encoding="utf-8") as tof:
    generate_header(tof)

    for const in consts:
      const['definition'] = re.sub('"', '\\"', const['definition'])
      const['definition'] = re.sub('>(\s+)<', '><', const['definition'])

      # 1. whole placeholder, 2. name, 3. whole format, 4. sign, 5. padding, 6. type
      const['args'] = re.findall('(\{([^:]+):(%([-+]?)([0-9]{0,})(\w))\})', const['definition'])

      for arg in const['args']:
        const['definition'] = re.sub(arg[0], arg[2], const['definition'])

      if(len(const['args']) > 0):
        generate_function(tof, const)
      else:
        generate_macro(tof, const)

  sys.exit(0)
