set(
	SOURCES
	"main.cpp"
	"keywords/set.cpp"
	"token.cpp"
	"compile.cpp"
 "keywords/mov.cpp")

if(DEFINED BOOST_ROOT)
	include_directories(${BOOST_ROOT})
else()
	message(WARNING "BOOST_ROOT variable is undefined")
endif()

link_directories("${BOOST_ROOT}/stage/lib")
add_executable(compiler ${SOURCES})