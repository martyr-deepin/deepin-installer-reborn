message(STATUS "check job syntax")
file(GLOB_RECURSE files "*.job")
foreach(file ${files})
	execute_process(
		COMMAND /bin/bash -n
		INPUT_FILE ${file}
		RESULT_VARIABLE res_var
		)
	if(NOT "${res_var}" STREQUAL "0")
		message(FATAL_ERROR "process script ${file} error")
	endif()
endforeach(file)
message(STATUS "check job syntax - OK")
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/deepin-installer
    FILES_MATCHING PATTERN "*"
    PATTERN "CMakeLists.txt" EXCLUDE
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
