set(
	SOURCES
	"main.cpp"
	"components/alu.cpp"
	"components/memory.cpp"
	"components/processor.cpp"
	"loader.cpp"
)

if(DEFINED BOOST_ROOT)
	include_directories(${BOOST_ROOT})
else()
	message(WARNING "BOOST_ROOT variable is undefined")
endif()

add_executable(computer ${SOURCES})