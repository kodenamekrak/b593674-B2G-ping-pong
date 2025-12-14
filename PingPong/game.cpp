#include "game.hpp"
#include "renderer.hpp"
#include "platform_comon.hpp"

#include <cstdio>
#include <algorithm>

#include "color.hpp"

#define is_down(b) input.buttons[b].is_down
#define is_pressed(b) (input.buttons[b].is_down && input.buttons[b].changed)
#define is_released(b) (!input.buttons[b].is_down && input.buttons[b].changed)

static float player_1_pos = 0, player_2_pos = 0;
static float player_1_dp = 0, player_2_dp = 0;
static float player_half_size_x = 2.5f, float player_half_size_y = 12.f;
static float arena_half_size_x = 85.f, arena_half_size_y = 45.f;

static const float PLAYER_OFFSET = 80.f;

void update_position(bool upkey, bool downkey, float& player_pos, float& player_dp, float delta)
{
	float player_ddp = 0;
	if (upkey) player_ddp += 2000;
	if (downkey) player_ddp -= 2000;

	player_ddp -= player_dp * 10.f;

	player_pos = player_pos + player_dp * delta + player_ddp * delta * delta * .5f;
	player_dp = player_dp + player_ddp * delta;
	player_pos = std::clamp(player_pos, -33.0f, 33.0f);
}

static bool rect_collides(float x1, float y1, float half_size_x_1, float half_size_y_1, float x2, float y2, float half_size_x_2, float half_size_y_2)
{
	return x1 + half_size_x_1 > x2 - half_size_x_2 && // Left border
		   x1 - half_size_x_1 < x2 + half_size_x_2 && // Right Border
		   y1 + half_size_y_1 > y2 - half_size_y_2 && // Bottom Border
		   y1 - half_size_y_1 < y2 + half_size_y_2;   // Top border
}

static bool ball_collides_with_player(float player_pos_x, float player_pos_y)
{
	return rect_collides(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, player_pos_x, player_pos_y, player_half_size_x, player_half_size_y);
}

void simulate_game(const Input& input, float delta)
{
	const Color BACKGROUND_COLOR(255, 100, 40);
	const Color BALL_COLOR(255, 255, 255);
	const Color PADDLE_COLOR(255, 0, 0);

	clear_screen(BACKGROUND_COLOR);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, Color(255, 200, 40));

	update_position(is_down(BUTTON_UP), is_down(BUTTON_DOWN), player_1_pos, player_1_dp, delta);
	update_position(is_down(BUTTON_W), is_down(BUTTON_S), player_2_pos, player_2_dp, delta);

	if(player_1_pos + player_half_size_y > arena_half_size_y)

	draw_rect(0, 0, 1, 1, BALL_COLOR);
	draw_rect(PLAYER_OFFSET, player_1_pos, 2.5, 12, PADDLE_COLOR);
	draw_rect(-PLAYER_OFFSET, player_2_pos, 2.5, 12, PADDLE_COLOR);
}