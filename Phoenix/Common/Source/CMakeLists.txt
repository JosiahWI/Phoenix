add_subdirectory(Math)
add_subdirectory(Voxels)
add_subdirectory(CMS)
add_subdirectory(Network)

set(currentDir ${CMAKE_CURRENT_LIST_DIR})
set(Sources
	${Sources}

	${currentDir}/CLIParser.cpp

	${currentDir}/Actor.cpp
	${currentDir}/Settings.cpp
	${currentDir}/Metadata.cpp
	${currentDir}/Logger.cpp
	${currentDir}/Commander.cpp
	${currentDir}/Input.cpp
	${currentDir}/Save.cpp
	${currentDir}/PlayerView.cpp

	PARENT_SCOPE
)
