set(
	SOURCES
	"main.cpp"
	"alu.cpp"
	"memory.cpp"
	"processor.cpp"
	"program.cpp"
)

if(DEFINED BOOST_ROOT)
	include_directories(${BOOST_ROOT})
else()
	message(WARNING "BOOST_ROOT variable is undefined")
endif()

add_executable(computer ${SOURCES})