// Copyright 2019-20 Genten Studios
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <Client/Graphics/Layer.hpp>
#include <Client/Graphics/Window.hpp>

#include <Common/Settings.hpp>

#include <entt/entt.hpp>

namespace phx::client
{
	/**
	 * @brief The Heads Up Display
	 *
	 * @see Layer
	 * @see LayerStack
	 */
	class HUD : public gfx::Overlay
	{
	public:
		HUD(gfx::Window* window, entt::registry* registry, entt::entity player);
		~HUD() override = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(events::Event& e) override;

		void tick(float dt) override;
		// (10 slots * btn width) + (11 margins * margin width)
		static constexpr float WIDTH  = (10 * 50) + (11 * 8);
		static constexpr float HEIGHT = 80;
		static constexpr float POSY   = 100;

	private:
		entt::registry* m_registry;

		gfx::Window* m_window;

		entt::entity m_player;
	};
} // namespace phx::client