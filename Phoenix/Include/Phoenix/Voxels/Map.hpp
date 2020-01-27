// Copyright 2019 Genten Studios
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

#include <Phoenix/CoreIntrinsics.hpp>
#include <Phoenix/Math/Math.hpp>
#include <Phoenix/Voxels/Chunk.hpp>

#include <vector>

namespace phx
{
	namespace voxels
	{
		/**
		 * @brief A map of chunks
		 *
		 * The map handles knowing which chunks need to be in memory based on if
		 * a client is using the chunk or not. The map also loads and saves
		 * chunks to file.
		 *
		 * Clients can checkout chunks based on their position, if the chunk is
		 * nolonger needed by the client, it should return the chunk to save
		 * memory. A server side client representation will handle this in the
		 * future.
		 */
		class Map
		{
		public:
			Map(std::string save, std::string mapName);
			~Map() = default;

			/**
			 * @brief Checks out a chunk from the map
			 *
			 * @param pos Position of the chunk to checkout
			 * @return Chunk& The address of the checked out chunk
			 */
			Chunk& checkoutChunk(math::vec3 pos);
			/**
			 * @brief Returns a chunk that was previously checked out
			 *
			 * @param pos Position of the chunk to return
			 */
			void returnChunk(math::vec3 pos);

		private:
			std::map<math::vec3 pos, Chunk chunk> m_chunks;
			std::map<math::vec3 pos, int out>     m_chunkLog;
			std::string                           m_save;
			std::string                           m_mapName;

			/**
			 * @brief Loads a chunk from save or creates a new one if needed
			 *
			 * @param pos Position of chunk to load
			 */
			void loadChunk(math::vec3 pos);
			/**
			 * @brief Saves a chunk to file then clears memory
			 *
			 * @param pos Position of chunk to unload
			 */
			void unloadChunk(math::vec3 pos);
		};
	}; // namespace voxels
};     // namespace phx