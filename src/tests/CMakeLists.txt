
#include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(CryptoTest_SRCS CryptoTest.cpp)

qt4_automoc(${CryptoTest_SRCS})
add_executable(CryptoTest ${CryptoTest_SRCS})

add_test(CryptoTest CryptoTest)

target_link_libraries(CryptoTest hidemydata ${QT_QTTEST_LIBRARY}
#${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
	${QT_QTXML_LIBRARY} ${PYTHON_LIBRARY} ${QT_QTGUI_LIBRARY}
	${EXIV2_LIBRARIES} ${QT_QTNETWORK_LIBRARY} ${BOTAN_LIBRARY})

