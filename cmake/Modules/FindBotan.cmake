# - Find Botan
# Botan is a BSD-licensed crypto library written in C++
#
# The following are defined by this module:
#  Botan_FOUND - TRUE if Botan was found
#  Botan_INCLUDE_DIR - Directory containing the Botan headers

#  Botan_LIBRARIES - If invoked via FIND_PACKAGE(Botan COMPONENTS ...),
#                      will only contain the libraries matching each component.
#                      otherwise, it will contain all Botan libraries found.

find_path(BOTAN_INCLUDE_DIR botan/botan.h
	/usr/include
	/usr/local/include
)

find_library(BOTAN_LIBRARY NAMES botan)

if (BOTAN_LIBRARY AND BOTAN_INCLUDE_DIR)
	SET(BOTAN_INCLUDE_DIR ${BOTAN_INCLUDE_DIR})
	SET(BOTAN_LIBRARY ${BOTAN_LIBRARY})
endif (BOTAN_LIBRARY AND BOTAN_INCLUDE_DIR)

# handle the QUIETLY and REQUIRED arguments and set PNG_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BOTAN  DEFAULT_MSG  BOTAN_LIBRARY BOTAN_INCLUDE_DIR)

mark_as_advanced(BOTAN_INCLUDE_DIR BOTAN_LIBRARY)