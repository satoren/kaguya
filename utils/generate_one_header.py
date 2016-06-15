
import os
import sys
import re

def inner_include_path(line):
    m = re.search('#include\s\"(?P<headername>.*)\"',line)
    if m:
        return m.group('headername')
    return None;

def parseheader(out,filepath,basedir,onceincludedfiles):

    for line in open(os.path.join(basedir,filepath), 'r'):
        if line.find('#pragma once') != -1:
            onceincludedfiles.append(os.path.join(basedir,filepath))
            continue
        path = inner_include_path(line)
        if path:
            if not os.path.join(basedir,path) in onceincludedfiles:
                parseheader(out,path,basedir,onceincludedfiles)
        else:
            out.write(line)

if __name__ == "__main__":
    onceincludedfiles=[]
    sys.stdout.write('#ifndef KAGUYA_LUABINDING_HPP_INCLUDED\n')
    sys.stdout.write('#define KAGUYA_LUABINDING_HPP_INCLUDED\n')
    parseheader(sys.stdout,'kaguya/kaguya.hpp','../include/',onceincludedfiles)
    sys.stdout.write('#endif// KAGUYA_LUABINDING_HPP_INCLUDED\n')
