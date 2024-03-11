set(
	SOURCES
	"main.cpp"
	"alu.cpp"
	"memory.cpp"
	"processor.cpp"
	"program.cpp"
)

if(DEFINED include)
	include_directories(${include})
else()
	message(WARNING "include variable is undefined")
endif()

add_executable(computer ${SOURCES})