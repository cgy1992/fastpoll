import sys
import re
import argparse

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

      in_def = False
      nl_cnt = 0

      # parse lines 
      for line in lines:
        for ch in line:
          if(in_def == False):
            if(nl_cnt > 0):
              nl_cnt = 0
              consts.append({'name': '', 'definition': ''})

            if(ch != ':'):
              if(ch not in [' ', '\t', '\n']):
                consts[-1]['name'] += ch 
            else:
              in_def = True

          else:
            if(ch == '\n'):
              nl_cnt += 1

              if(nl_cnt == 2):
                in_def = False
            else:
              if(ch not in ['\t', '\n']):
                consts[-1]['definition'] += ch

  # write template makros
  with open(args.output[0], "w", encoding="utf-8") as tof:
    print (consts)
    for const in consts:
      const['definition'] = re.sub('"', '\\"', const['definition'])

      if(len(const['name']) > 0 and len(const['definition']) > 0):
        tof.write("#define " + const['name'].upper() + " \"" + const['definition'] + "\"" + '\n' * 2);

  sys.exit(0)