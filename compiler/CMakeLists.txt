set(
	SOURCES
	"main.cpp"
	"keywords/set.cpp"
	"token.cpp"
)

if(DEFINED include)
	include_directories(${include})
else()
	message(WARNING "include variable is undefined")
endif()

add_executable(compiler ${SOURCES})