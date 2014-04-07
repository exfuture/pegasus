cmake_minimum_required(VERSION 2.6)
project(pegasus)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -march=native -mtune=native -std=c99 -W -Wall -pedantic -pipe -D_GNU_SOURCE -fPIC")

set(linking_libraries
	"m")

set(sources
	"pegasus.c"
	"pegasus_block.c"
	"pegasus_channel.c"
	"pegasus_error.c"
	"pegasus_fec.c"
	"pegasus_modulation.c"
	"pegasus_panic.c"
	"pegasus_random.c"
	"pegasus_selftests.c"
	"pegasus_source.c"
	"pegasus_tools.c"
	"pegasus_units.c")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
	list(APPEND linking_libraries
		"gomp")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -openmp -static-intel")
endif()

add_executable(pegasus
	${sources})

find_library(TCMALLOC_LIBRARY
	NAMES
		libtcmalloc_minimal.so
	PATHS
		/usr/lib
		/usr/local/lib)

find_library(JEMALLOC_LIBRARY
	NAMES
		libjemalloc.so
	PATHS
		/usr/lib
		/usr/local/lib)

if (TCMALLOC_LIBRARY)
	list(APPEND linking_libraries
		"pthread"
		"tcmalloc_minimal")
elseif (JEMALLOC_LIBRARY)
	list(APPEND linking_libraries
		"jemalloc")
endif (TCMALLOC_LIBRARY)

target_link_libraries(pegasus
	${linking_libraries})

