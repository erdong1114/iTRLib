project(itrbase) #project name
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

set(LIBRARY_OUTPUT_PATH "${itrbase_SOURCE_DIR}/bin/debug")#force change build directory
set(CMAKE_BUILD_TYPE DEBUG)	 #debug mode

AUX_SOURCE_DIRECTORY(./platform platform)
AUX_SOURCE_DIRECTORY(./encode encode)
AUX_SOURCE_DIRECTORY(./math math)
AUX_SOURCE_DIRECTORY(./protocol protocol)
AUX_SOURCE_DIRECTORY(./container container)
#build all lib into a single one
add_library(itrbase ${platform} ${encode} ${math} ${protocol} ${container})
