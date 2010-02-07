include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${PYTHON_INCLUDE_DIRS} ${BOTAN_INCLUDE_DIR})

set(library_SRCS
	Process.cpp
	Process_Download.cpp
	Process_Upload.cpp
	Chunker.cpp
	Crypto.cpp
	MetaHandler.cpp
	Stegano.cpp
	Transmission.cpp
)

set(gui_SRCS
	MainWindow.cpp
	main.cpp
	DialogNewDownload.cpp
	DialogNewUpload.cpp
	DialogAbout.cpp
	DialogSettings.cpp
	DialogResult.cpp
	ProcessListDelegate.cpp
	ProcessListModel.cpp
	ProcessListView.cpp
)

set(UI_FILES
	MainWindow.ui
	DialogNewDownload.ui
	DialogNewUpload.ui
	DialogAbout.ui
	DialogResult.ui
	DialogSettings.ui
)

set(translation_files
	translations/hidemydata_de.ts
)

set(qt4_rcc
	hidemydata.qrc
)

qt4_add_resources(qt4_resources ${qt4_rcc})
qt4_automoc(${library_SRCS})
qt4_automoc(${gui_SRCS})

qt4_wrap_ui(UIFILES ${UI_FILES})

add_library(hidemydata SHARED ${library_SRCS})
target_link_libraries(hidemydata ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
	${QT_QTXML_LIBRARY} ${PYTHON_LIBRARY} ${EXIV2_LIBRARIES}
	${QT_QTNETWORK_LIBRARY} ${BOTAN_LIBRARY})

qt4_add_translation(TRANSLATION ${translation_files})
add_executable(hidemydata_qt4 ${gui_SRCS} ${UIFILES} ${TRANSLATION} ${qt4_resources})

SET_TARGET_PROPERTIES(hidemydata_qt4 PROPERTIES ENABLE_EXPORTS ON) #um die unittests zu bauen

target_link_libraries(hidemydata_qt4 ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
${QT_QTXML_LIBRARY} hidemydata ${PYTHON_LIBRARY} ${EXIV2_LIBRARIES} ${QT_QTNETWORK_LIBRARY} ${BOTAN_LIBRARY})

add_subdirectory(tests)
