#include <Quartz2/Quartz.hpp>

#include <memory>

using namespace q2;

class PhoenixGame : public Game
{
private:
	std::unique_ptr<ChunkRenderer> m_chunkRenderer;
	std::unique_ptr<Camera> m_camera;

public:
	PhoenixGame()
		: Game(800, 600, "Phoenix Game!")
	{}

protected:
	virtual bool onEvent(SDL_Event e)
	{
		if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				m_camera->toggleEnabled();
				SDL_ShowCursor(!m_camera->isEnabled());
				return true;
			}
		}

		return false;
	}

	virtual void onStart()
	{
		m_chunkRenderer = std::make_unique<ChunkRenderer>();
		m_chunkRenderer->setup();
	
		if (!m_chunkRenderer->isReady())
		{
			std::fprintf(stderr, "Renderer setup failed: Status = %s\n",
				enums::toString(m_chunkRenderer->status()));

			exitGame();
		}

		m_camera = std::make_unique<Camera>();
		SDL_ShowCursor(0);
	}
	
	virtual void onExit()
	{
		m_chunkRenderer->teardown();
	}

	virtual void onFrame(float dt)
	{
		OpenGL32::clearScreen(0.f, 0.f, 0.f, 0.f);

		m_camera->update(dt, getSDLWindow());

		m_chunkRenderer->render(m_camera.get());
	}
};

int main(int argc, char** argv)
{
	Game* game = new PhoenixGame();
	game->start();

	return 0;
}