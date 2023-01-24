

# Mom, can we get CMAKE?
# No, sweetie, we have CMAKE at home.
# The cmake at home:

import os
import subprocess
from typing import List

def get_all_cpp_files(directory: str) -> List[str]:
    out = []

    for cont in os.listdir(directory):
        path = os.path.join(directory, cont)

        if os.path.isfile(path):
            if path.endswith(('.cpp', '.c')):
                out.append(path)
        elif os.path.isdir(path):
            out += get_all_cpp_files(path)
    
    return out


def build_parser():
    _cwd = os.getcwd()
    os.chdir(os.path.join(os.path.dirname(__file__), 'flex'))

    subprocess.run([ 'flex', os.path.join(os.path.dirname(__file__), 'flex', 'parser.lex') ])
    subprocess.run([ 'bison', '-dy', os.path.join(os.path.dirname(__file__), 'flex', 'parser.yacc') ]) # If errors, run with the -v flag

    os.chdir(_cwd)


def build_server():
    # Find all files
    base_dir = os.path.dirname(__file__)
    cppfiles = get_all_cpp_files(base_dir)

    # Required libraries
    libs = [
        "ws2_32"
    ]

    proc = [ 'g++', *cppfiles, '-I', base_dir, *[ f'-l{lib}' for lib in libs ], '-o', os.path.join(base_dir, 'server.exe') ]
    subprocess.run(proc)

def build():
    build_parser()

    # build_server()

if __name__ == "__main__":
    build()
