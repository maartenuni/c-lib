#!/usr/bin/env python3

import sys
import os.path
import datetime

libname = 'c-lib'

message = '''/*
 * This file is part of {}
 *
 * Copyright © {} {}
 *
 * {} is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * {} is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the Lesser General Public License
 * along with {}.  If not, see <http://www.gnu.org/licenses/>
 */'''

author = ""
year = datetime.datetime.now().year

def print_usage():
    bname = os.path.basename(sys.argv[0])
    print("Usage: {} <Author Name>".format(bname), file=sys.stderr)
    

if len(sys.argv) != 2:
    print_usage()
    exit(1)

author = sys.argv[1]
print (message.format(libname, year, author, libname, libname, libname));
