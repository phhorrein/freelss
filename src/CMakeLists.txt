cmake_minimum_required(VERSION 2.8)

#if(POLICY CMP0020)
#	cmake_policy(SET CMP0020 NEW)
#endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

PROJECT(FreeLSS-NG)

find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system)
find_package(MHD REQUIRED)
find_package(OpenCV REQUIRED)
find_package(LibIW REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

if (CURL_FOUND AND OpenSSL_FOUND AND MHD_FOUND AND LIBIW_FOUND) 
	set(FREELSS_WEBSERVER, 1)
	message(STATUS "Building webserver")
else()
	set(FREELSS_WEBSERVER, 1)
	message(WARNING "Webserver will not be built")
endif()


# Check which VTK and QT versions is used. We only support VTK6 and QT5
if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
	# Instruct CMake to run moc automatically when needed.
	set(CMAKE_AUTOMOC ON)
	find_package(Qt5Core REQUIRED QUIET)
	find_package(Qt5Widgets REQUIRED QUIET)
else()
	message(FATAL_ERROR "Unsupported VTK or Qt version")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CURL_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${MHD_INCLUDE_DIRS}
	gui core webapp hardware contrib)

file(GLOB UI_FILES gui/*.ui)
file(GLOB QT_WRAP geninc/*.h)
file(GLOB CXX_CORE_FILES core/*.cpp hardware/*.cpp ) 
file(GLOB CXX_WEBAPP_FILES webapp/*.cpp)
set(CMAKE_BUILD_TYPE Debug)

# CMAKE_AUTOMOC in ON so the MocHdrs will be automatically wrapped.
add_executable(FreeLSS-webapp
	${CXX_CORE_FILES} ${CXX_WEBAPP_FILES})

target_link_libraries(FreeLSS-webapp
	${CMAKE_THREAD_LIBS_INIT}
	${JPEG_LIBRARIES} 
	${PNG_LIBRARIES} 
	${Boost_LIBRARIES}
	${OpenCV_LIBRARIES}
	${CURL_LIBRARIES}
	${MHD_LIBRARIES}
	${LIBIW_LIBRARIES}
	${OPENSSL_LIBRARIES})
#qt5_wrap_ui(UISrcs ${UI_FILES} )
#qt5_use_modules(FreeLSS-NG Core Gui)

#target_link_libraries(FreeLSS-NG
#	${VTK_LIBRARIES}
#	${CMAKE_THREAD_LIBS_INIT}
#	${Boost_LIBRARIES}
#	${OpenCV_LIBRARIES}
#	${CURL_LIBRARIES}
#	${MHD_LIBRARIES}
#	${LIBIW_LIBRARIES}
#	${OPENSSL_LIBRARIES})
#target_link_libraries(FreeLSS-NG vtkHybrid QVTK vtkViews ${QtSerialPort_LIBRARIES} ${QT_LIBRARIES})
