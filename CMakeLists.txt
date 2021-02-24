cmake_minimum_required(VERSION 3.8)

project(IPlanetNetworking)


#
#option(IPlanetNetworking_Threaded "Sets various bits ")
#


set(lib_sources

	"include/${PROJECT_NAME}.h" 
	"include/${PROJECT_NAME}Config.h" 

	"source/${PROJECT_NAME}.cpp"

	"source/Error/Error.h"
	"source/Error/Error.cpp"

	"source/Utility/Global.h"
	"source/Utility/Global.cpp"
	
	"source/SockLib/SockLib.h"
	"source/SockLib/SockLib.cpp"
)



set(lib_name ${PROJECT_NAME})



set(lib_cxx 20)

set(lib_public_include 
	"include"
)
set(lib_private_include
	"source"
)

set(lib_public_link
	SAELib
)

set(lib_private_link

)



add_library(${lib_name} STATIC "${lib_sources}")
target_include_directories(${lib_name}
	PUBLIC "${lib_public_include}" 
	PRIVATE "${lib_private_include}")

set_target_properties(${lib_name} PROPERTIES CXX_STANDARD ${lib_cxx} CXX_STANDARD_REQUIRED True)

target_link_libraries(${lib_name} PUBLIC ${lib_public_link} PRIVATE ${lib_private_link})

target_compile_definitions(${lib_name} PUBLIC IPLANET_NETWORKING_CMAKE=true)


