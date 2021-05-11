#pragma once

constexpr unsigned int CHUNK_SIZE = 32U;
constexpr unsigned int CHUNK_KEY_SHIFT = 5U;
constexpr unsigned int TILEMAP_SIZE_X = 32U;
constexpr unsigned int TILEMAP_SIZE_Y = 16U;
constexpr unsigned int TILEMAP_KEY_SHIFT = 6U;
constexpr unsigned int MAX_BLOCK_X = TILEMAP_SIZE_X * CHUNK_SIZE;
constexpr unsigned int MAX_BLOCK_Y = TILEMAP_SIZE_Y * CHUNK_SIZE;


constexpr unsigned int GROUND_LEVEL = 128U;
