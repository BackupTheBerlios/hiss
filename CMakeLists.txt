# HIDEMYDATA
###############################################################################
# -DDebug=1			# enable debug
## -DCMAKE_INSTALL_PREFIX=/bla/	# install prefix
## -DCMAKE_DATA_PATH=/other/bla/	# install path for data
## -DCMAKE_CONF_PATH=/etc	# install path for conf
## -DLIB_INSTALL_DIR=/other/bla	# install path for lib
## -DCMAKE_DOC_PATH=/other/bla	# install path for doc
## -DCMAKE_LOCALE_PATH=/other/bla	# install path for locale
## -DCMAKE_FONT=/bla/font.ttf	# install path for locale
###############################################################################
# IMPORTANT DEFINITIONS [PREFIX,...]
###############################################################################
SET ( HIDEMYDATA_VERSION "0.1" CACHE STRING "Version" )
# SET ( WORKDIR ${CMAKE_SOURCE_DIR}/src ) kein plan ob man das braucht
# data because some distributions want different data placement
SET ( CMAKE_CONF_PATH "/etc" CACHE PATH "Config path prefix" )
SET ( CMAKE_INSTALL_PREFIX "/usr/local/" CACHE PATH "Install path prefix" )
SET ( CMAKE_DOC_PATH "${CMAKE_INSTALL_PREFIX}/share/doc" CACHE PATH "Data path prefix" )
SET ( CMAKE_LOCALE_PATH "${CMAKE_INSTALL_PREFIX}/share/locale" CACHE PATH "Locale path prefix" )
SET ( CMAKE_DATA_PATH "${CMAKE_INSTALL_PREFIX}/share/" CACHE PATH "Data path prefix" )
SET ( CMAKE_LIB_PATH "${CMAKE_INSTALL_PREFIX}/lib/" CACHE PATH "Library path prefix" )
SET ( LIB_INSTALL_DIR "${CMAKE_LIB_PATH}" )
###############################################################################
# BASIC PROJECT VALUES
###############################################################################
PROJECT ( hidemydata ) #

CMAKE_MINIMUM_REQUIRED ( VERSION 2.8.0 FATAL_ERROR )

SET ( CMAKE_C_FLAGS $ENV{CFLAGS} )
SET ( CMAKE_CXX_FLAGS $ENV{CXXFLAGS} )
# this specify location of additional cmake sources
SET ( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" )
#various variables
SET ( CMAKE_INSTALL_ETCDIR ${CMAKE_CONF_PATH} )
SET ( CMAKE_INSTALL_BINDIR bin )
SET ( CMAKE_INSTALL_LOCALEDIR ${CMAKE_LOCALE_PATH} )
SET ( CMAKE_INSTALL_DATADIR ${CMAKE_DATA_PATH} )
SET ( CMAKE_INSTALL_LIBDIR ${LIB_INSTALL_DIR} )
SET ( CMAKE_INSTALL_DOCDIR ${CMAKE_DOC_PATH} )

SET ( BUILD_SHARED_LIBRARIES ON)

add_definitions(-DVERSION="${HIDEMYDATA_VERSION}")

# DEBUG STUFF

IF ( Debug )
	SET( CMAKE_C_FLAGS "-g -O0 -Wall -pipe")
	SET( CMAKE_CXX_FLAGS "-g -O0 -Wall -pipe")
	ADD_DEFINITIONS ( -DDebug )
	SET ( CMAKE_VERBOSE_MAKEFILE on )
ENDIF ( Debug )

FIND_PACKAGE ( Qt4 REQUIRED ) #dann nochma gucken mit qt-gui
FIND_PACKAGE ( PythonLibs REQUIRED )
FIND_PACKAGE ( Exiv2 REQUIRED )
FIND_PACKAGE ( Botan REQUIRED )
#IF ( NOT Server )
#	FIND_PACKAGE ( SDL REQUIRED )
#	FIND_PACKAGE ( SDL_image REQUIRED )
#	FIND_PACKAGE ( SDL_ttf REQUIRED )
#	IF ( NOT NO_Audio )
#		FIND_PACKAGE ( SDL_mixer REQUIRED )
#	ENDIF ( NOT NO_Audio )
#ENDIF ( NOT Server )


add_subdirectory ( src )
# path which user send us
INSTALL ( FILES ${CMAKE_SOURCE_DIR}/src/hidemydata_qt4
	DESTINATION ${CMAKE_BIN_PATH}/${APPNAME}/ )
#INSTALL ( FILES ${CMAKE_SOURCE_DIR}/AUTHORS
#	DESTINATION ${CMAKE_INSTALL_DOCDIR}/${APPNAME}/ )
#INSTALL ( FILES ${CMAKE_SOURCE_DIR}/README
#	DESTINATION ${CMAKE_INSTALL_DOCDIR}/${APPNAME}/ )
