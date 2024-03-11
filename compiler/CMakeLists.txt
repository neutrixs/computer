set(
	SOURCES
	"main.cpp"
	"keywords/set.cpp"
	"token.cpp"
)

if(DEFINED BOOST_ROOT)
	include_directories(${BOOST_ROOT})
else()
	message(WARNING "BOOST_ROOT variable is undefined")
endif()

add_executable(compiler ${SOURCES})