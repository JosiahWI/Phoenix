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

#include <Phoenix/Graphics/Camera.hpp>
#include <Phoenix/Player.hpp>
#include <iostream>

using namespace phx;
using namespace gfx;

FPSCamera::FPSCamera(Window* window, Actor* actor)
{
	m_window = window;
	m_window->setCursorState(CursorState::DISABLED);

	const math::vec2f windowSize = static_cast<math::vec2f>(window->getSize());
	m_projection = math::mat4::perspective(windowSize.x / windowSize.y, 45.f,
	                                       1000.f, 0.1f);

	m_windowCentre = {std::floor(windowSize.x / 2.f),
	                  std::floor(windowSize.y / 2.f)};
	//TODO: When the actor system has a registry, get player from there instead
	m_actor = actor;

	m_settingSensitivity =
	    Settings::get()->add("Sensitivity", "camera:sensitivity", 5);
	m_settingSensitivity->setMax(100);
	m_settingSensitivity->setMin(1);
}

math::vec3 FPSCamera::getPosition() const { return m_position; }

math::vec3 FPSCamera::getDirection() const { return m_direction; }

void FPSCamera::setProjection(const math::mat4& projection)
{
	m_projection = projection;
}

math::mat4 FPSCamera::getProjection() const { return m_projection; }

void FPSCamera::setActor(Actor actor){m_actor = &actor;};

math::mat4 FPSCamera::calculateViewMatrix() const
{
	const math::vec3 centre = m_position + m_direction;
	return math::mat4::lookAt(m_position, centre, m_up);
}

void FPSCamera::tick(float dt)
{
	if (!m_enabled)
		return;

	const math::vec2 mousePos = m_window->getCursorPosition();

	m_window->setCursorPosition(m_windowCentre);

	const float sensitivity = m_settingSensitivity->value() * 0.00001;

	m_rotation = m_actor->getRotation();
	m_position = m_actor->getPosition();

	m_rotation.x += sensitivity * dt * (m_windowCentre.x - mousePos.x);
	m_rotation.y += sensitivity * dt * (m_windowCentre.y - mousePos.y);

	m_rotation.y = math::clamp(m_rotation.y, -math::PIDIV2, math::PIDIV2);

	m_direction.x = std::cos(m_rotation.y) * std::sin(m_rotation.x);
	m_direction.y = std::sin(m_rotation.y);
	m_direction.z = std::cos(m_rotation.y) * std::cos(m_rotation.x);

	const math::vec3 right = {std::sin(m_rotation.x - math::PIDIV2), 0.f,
	                          std::cos(m_rotation.x - math::PIDIV2)};

	const math::vec3 forward = {std::sin(m_rotation.x), 0.f,
	                          std::cos(m_rotation.x)};

	m_up = math::vec3::cross(right, m_direction);

	const float moveSpeed = m_actor->getMoveSpeed() * 0.001f;

	if (m_window->isKeyDown(events::Keys::KEY_W))
	{
		m_position += forward * dt * moveSpeed;
	}
	else if (m_window->isKeyDown(events::Keys::KEY_S))
	{
		m_position -= forward * dt * moveSpeed;
	}

	if (m_window->isKeyDown(events::Keys::KEY_A))
	{
		m_position -= right * dt * moveSpeed;
	}
	else if (m_window->isKeyDown(events::Keys::KEY_D))
	{
		m_position += right * dt * moveSpeed;
	}

	if (m_window->isKeyDown(events::Keys::KEY_SPACE))
	{
		m_position.y += dt * moveSpeed;
	}
	else if (m_window->isKeyDown(events::Keys::KEY_LEFT_SHIFT))
	{
		m_position.y -= dt * moveSpeed;
	}

	m_actor->setPosition(m_position);
	m_actor->setRotation(m_rotation);
}

void FPSCamera::enable(bool enabled)
{
	if (enabled)
	{
		m_window->setCursorState(gfx::CursorState::DISABLED);
		m_window->setCursorPosition(m_windowCentre);
	}
	else
	{
		m_window->setCursorState(gfx::CursorState::NORMAL);
	}

	m_enabled = enabled;
}

void FPSCamera::onWindowResize(events::Event e)
{
	const math::vec2i windowSize = m_window->getSize();
	m_projection = math::mat4::perspective(static_cast<float>(windowSize.x) /
	                                           static_cast<float>(windowSize.y),
	                                       45.f, 1000.f, 0.1f);

	m_windowCentre = {static_cast<float>(static_cast<int>(windowSize.x / 2)),
	                  static_cast<float>(static_cast<int>(windowSize.y / 2))};
}
