import sys
import re
import argparse
from enum import IntEnum

class Flags(IntEnum):
  InDef = 1
  InString = 2,
  InNodeDecl = 4,

if __name__ == '__main__':
  consts = [{'name': '', 'definition': ''}]

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

                if(Flags.InNodeDecl & flags == False and ch == ' '): 
                  pass

                else:
                  consts[-1]['definition'] += ch

  # write template makros
  with open(args.output[0], "w", encoding="utf-8") as tof:
    for const in consts:
      const['definition'] = re.sub('"', '\\"', const['definition'])

      max_len = 80 - 2 # SPACE '/'
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
            tof.write(' /')

          tof.write('\n')

          const['definition'] = const['definition'][len_per_line:]
          first_line = False

  sys.exit(0)