add_subdirectory(GUI)

set(currentDir ${CMAKE_CURRENT_LIST_DIR})
set(Headers
	${Headers}

	${currentDir}/OpenGLTools.hpp
	${currentDir}/ImGuiExtensions.hpp

	${currentDir}/Window.hpp
	${currentDir}/Layer.hpp
	${currentDir}/LayerStack.hpp

	${currentDir}/ShaderPipeline.hpp
	${currentDir}/Camera.hpp

	${currentDir}/ChatBox.hpp

	${currentDir}/BlockModel.hpp
	${currentDir}/BaseBlockModels.hpp
	${currentDir}/TexturePacker.hpp
	${currentDir}/ChunkMesher.hpp
	${currentDir}/ChunkRenderer.hpp
	${currentDir}/WorldRenderer.hpp

	PARENT_SCOPE
)
