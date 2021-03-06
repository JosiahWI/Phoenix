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

#include <Common/Logger.hpp>
#include <Common/Math/Math.hpp>
#include <Common/Metadata.hpp>

using namespace phx;

bool Metadata::set(const std::string& key, const std::any& data)
{
	auto existing = m_data.find(key);
	if (existing != m_data.end())
	{
		if (existing->second.type() != data.type())
		{
			return false;
		}
	}
	if (data.type() == typeid(int) || data.type() == typeid(float) ||
	    data.type() == typeid(phx::math::vec3))
	{
		m_data[key] = data;
		return true;
	}
	LOG_DEBUG("Metadata") << "Attempted to store unsupported datatype"
	                      << data.type().name() << "in metadata container";
	return false;
}

const std::any* Metadata::get(const std::string& key) const
{
	auto existing = m_data.find(key);
	if (existing != m_data.end())
	{
		return &existing->second;
	}
	return nullptr;
}

Serializer& Metadata::operator>>(Serializer& ser) const
{
	int size = m_data.size();
	ser << size;
	for (auto& data : m_data)
	{
		ser << data.first;
		if (data.second.type() == typeid(int))
		{
			ser << 'i' << std::any_cast<int>(data.second);
		}
		else if (data.second.type() == typeid(float))
		{
			ser << 'f' << std::any_cast<float>(data.second);
		}
		else if (data.second.type() == typeid(phx::math::vec3))
		{
			math::vec3 v = std::any_cast<math::vec3>(data.second);
			ser << 'v' << v.x << v.y << v.z;
		}
		else
		{
			LOG_FATAL("Metadata")
			    << "Attempted to serialize unsupported data type";
			exit(EXIT_FAILURE);
		}
	}
	return ser;
}

Serializer& Metadata::operator<<(Serializer& ser)
{
	int size;
	ser >> size;
	for (int i = 0; i < size; i++)
	{
		std::string key;
		ser >> key;
		char type;
		ser >> type;
		std::any val;
		if (type == 'i')
		{
			int v;
			ser >> v;
			set(key, v);
		}
		else if (type == 'f')
		{
			float v;
			ser >> v;
			set(key, v);
		}
		else if (type == 'v')
		{
			float x, y, z;
			ser >> x >> y >> z;
			math::vec3 v(x, y, z);
			set(key, v);
		}
		else
		{
			LOG_FATAL("Metadata")
			    << "Attempted to deserialize unsupported data type";
			exit(EXIT_FAILURE);
		}
	}
	return ser;
}
