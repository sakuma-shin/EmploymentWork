#pragma once

namespace GameConstants {
	// Rail camera / player initial positions
	constexpr float RAIL_CAMERA_DISTANCE_X = 0.0f;
	constexpr float RAIL_CAMERA_DISTANCE_Y = -7.0f;
	constexpr float RAIL_CAMERA_DISTANCE_Z = 28.0f;

	constexpr float PLAYER_INIT_X = 0.0f;
	constexpr float PLAYER_INIT_Y = -4.0f;
	constexpr float PLAYER_INIT_Z = 28.0f;

	constexpr float RAIL_CAMERA_START_POS_Z = 200.0f;
	constexpr float RAIL_CAMERA_ROT_X = 0.1f;

	// Sky dome
	constexpr float SKYDOME_DISTANCE = 30.0f;

	// Player movement / shooting
	constexpr float PLAYER_MOVE_SPEED = 0.5f;
	constexpr float PLAYER_MAX_TILT = 0.3f;
	constexpr float PLAYER_TILT_SPEED = 0.2f;
	constexpr float PLAYER_BULLET_SPEED = 2.0f;

	// Rail camera
	constexpr float RAIL_CAMERA_MOVE_SPEED = 0.5f;

	// Enemy defaults
	constexpr float ENEMY_INIT_ROT_X = 3.0f;
	constexpr float ENEMY_INIT_ROT_Y = 1.6f;
	constexpr float ENEMY_SCALE_FACTOR = 0.7f;
	constexpr float ENEMY_SIZE_X = 4.0f;
	constexpr float ENEMY_SIZE_Y = 4.0f;
	constexpr float ENEMY_SIZE_Z = 1.0f;
	constexpr float ENEMY_APPROACH_SPEED_Z = -0.05f;
	constexpr float ENEMY_LEAVE_ROT_Z = -1.5f;
	constexpr float ENEMY_LEAVE_DOWN_VELOCITY = -0.4f;
	constexpr float ENEMY_BULLET_SPEED_Z = -0.5f;
}
