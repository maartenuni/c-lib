#
# This file is part of c-lib
# 
# c-lib is free software: you can redistribute it and/or modify
# it under the terms of the Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
# 
# c-lib is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# Lesser General Public License for more details.
# 
# You should have received a copy of the Lesser General Public License
# along with c-lib.  If not, see <http://www.gnu.org/licenses/>
#

cmake_minimum_required(VERSION 2.8)
project (c_lib C CXX)


include (CheckIncludeFiles)
include (GenerateExportHeader)

#set versioning variables
set (C-LIB_VERSION_MAJOR 0)
set (C-LIB_VERSION_MINOR 0)
set (C-LIB_VERSION_MICRO 0)
set (C-LIB_VERSION_STRING
    "\"c-lib-${C-LIB_VERSION_MAJOR}.\""
    "\"${C-LIB_VERSION_MAJOR}.${C-LIB_VERSION_MICRO}\""
    )

#set names for the targets
set (CLIB_SHARED_LIB c-lib-${C-LIB_VERSION_MAJOR}_${C-LIB_VERSION_MINOR})
set (CLIB_STATIC_LIB c-lib-${C-LIB_VERSION_MAJOR}_${C-LIB_VERSION_MINOR}-static)

#Check for expected headers
include(CheckIncludeFiles)
CHECK_INCLUDE_FILES(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES(assert.h HAVE_ASSERT_H)

#Add compilation with warnigs
if(MSVC)
    if (CMAKE_C_FLAGS MATCHES "/w[0-4]")
        string(REGEX REPLACE "/w[0-4]]" "/W4")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
    endif()
else()
    #enable C99
    #this assumes the compiler know about -Wall -pedantic
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -std=c99")
endif()

#add subdirectories
add_subdirectory(src)
add_subdirectory(test)

