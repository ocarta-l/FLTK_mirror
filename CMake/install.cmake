#
# "$Id: install.cmake 12282 2017-06-29 18:12:57Z AlbrechtS $"
#
# Main CMakeLists.txt to build the FLTK project using CMake (www.cmake.org)
# Written by Michael Surette
#
# Copyright 1998-2017 by Bill Spitzak and others.
#
# This library is free software. Distribution and use rights are outlined in
# the file "COPYING" which should have been included with this file.  If this
# file is missing or damaged, see the license at:
#
#     http://www.fltk.org/COPYING.php
#
# Please report all bugs and problems on the following page:
#
#     http://www.fltk.org/str.php
#

#######################################################################
# installation
#######################################################################

# generate uninstall target
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  @ONLY
)
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/FL
   DESTINATION ${FLTK_INCLUDEDIR} USE_SOURCE_PERMISSIONS
   PATTERN ".svn" EXCLUDE
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/FL
   DESTINATION ${FLTK_INCLUDEDIR} USE_SOURCE_PERMISSIONS
)

if(OPTION_CREATE_LINKS)
   install(SCRIPT ${CMAKE_BINARY_DIR}/install-symlinks.cmake)
endif(OPTION_CREATE_LINKS)

# generate FLTKConfig.cmake for installed directory use
set(INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)

set(CONFIG_PATH ${CMAKE_INSTALL_PREFIX}/${FLTK_CONFIG_PATH})

install(EXPORT FLTK-Targets
   DESTINATION ${FLTK_CONFIG_PATH}
   FILE FLTK-Targets.cmake
)

configure_file(
   ${CMAKE_SOURCE_DIR}/CMake/FLTKConfig.cmake.in
   ${CMAKE_BINARY_DIR}/etc/FLTKConfig.cmake
   @ONLY
)

install(FILES ${CMAKE_BINARY_DIR}/etc/FLTKConfig.cmake
   DESTINATION ${FLTK_CONFIG_PATH}
)

configure_file(
   ${CMAKE_SOURCE_DIR}/CMake/UseFLTK.cmake.in
   ${CMAKE_BINARY_DIR}/etc/UseFLTK.cmake
   @ONLY
)

install(FILES ${CMAKE_BINARY_DIR}/etc/UseFLTK.cmake
   DESTINATION ${FLTK_CONFIG_PATH}
)

# generate fltk-config
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix "\${prefix}")
set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
set(BINARY_DIR)
set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
set(srcdir ".")

set(LIBNAME "${libdir}/libfltk.a")

configure_file(
   "${CMAKE_SOURCE_DIR}/fltk-config.in"
   "${CMAKE_BINARY_DIR}/bin/fltk-config"
   @ONLY
)
if(UNIX)
   execute_process(COMMAND chmod 755 fltk-config
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
   )
endif(UNIX)

install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/fltk-config
   DESTINATION ${FLTK_BINDIR}
)

if(UNIX OR MSYS OR (MINGW AND CMAKE_CROSSCOMPILING))
   macro(INSTALL_MAN FILE LEVEL)
   install(FILES
      ${CMAKE_SOURCE_DIR}/documentation/src/${FILE}.man
      DESTINATION ${FLTK_MANDIR}/man${LEVEL}
      RENAME ${FILE}.${LEVEL}
   )
   endmacro(INSTALL_MAN FILE LEVEL)

   INSTALL_MAN(fluid 1)
   INSTALL_MAN(fltk-config 1)
   INSTALL_MAN(fltk 3)
   INSTALL_MAN(blocks 6)
   INSTALL_MAN(checkers 6)
   INSTALL_MAN(sudoku 6)

endif(UNIX OR MSYS OR (MINGW AND CMAKE_CROSSCOMPILING))
