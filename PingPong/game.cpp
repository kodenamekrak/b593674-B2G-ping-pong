#include "game.hpp"
#include "renderer.hpp"
#include "platform_comon.hpp"

#include <cstdio>
#include <algorithm>

#include "color.hpp"

// Why does windows define this ????
#undef min

#define is_down(b) input.buttons[b].is_down
#define is_pressed(b) (input.buttons[b].is_down && input.buttons[b].changed)
#define is_released(b) (!input.buttons[b].is_down && input.buttons[b].changed)

static GameMode game_mode = GM_MENU;
static bool hot_button = false;
bool enemy_is_ai;

static float player_1_pos = 0, player_2_pos = 0;
static float player_1_dp = 0, player_2_dp = 0;
static float player_half_size_x = 2.5f, player_half_size_y = 12.f;

static int player_1_score = 0, player_2_score;

static float arena_half_size_x = 85.f, arena_half_size_y = 45.f;

static float ball_pos_x = 0, ball_pos_y = 0;
static float ball_dp_x = 100.f, ball_dp_y = 0;
static float ball_half_size = 1;

static const float PLAYER_OFFSET = 80.f;

void update_player_position(float& player_pos, float& player_dp, float player_ddp, float delta)
{
	player_ddp -= player_dp * 10.f;

	player_pos = player_pos + player_dp * delta + player_ddp * delta * delta * .5f;
	player_dp = player_dp + player_ddp * delta;

	if (player_pos + player_half_size_y > arena_half_size_y)
	{
		player_pos = arena_half_size_y - player_half_size_y;
		player_dp = 0;
	}

	if (player_pos - player_half_size_y < -arena_half_size_y)
	{
		player_pos = -arena_half_size_y + player_half_size_y;
		player_dp = 0;
	}
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
	const Color BACKGROUND_COLOR(220, 180, 100);
	clear_screen(BACKGROUND_COLOR);

	if (game_mode == GM_MENU)
	{
		if (is_pressed(BUTTON_LEFT) || is_pressed(BUTTON_RIGHT))
		{
			hot_button = !hot_button;
		}

		if (is_pressed(BUTTON_ENTER))
		{
			game_mode = GM_GAMEPLAY;
			enemy_is_ai = !hot_button;
		}

		if (hot_button == false)
		{ 
			draw_rect(-20, 0, 10, 10, Color(200, 0, 0));
			draw_rect(20, 0, 10, 10, Color(200, 200, 200));
		}
		else
		{ 
			draw_rect(20, 0, 10, 10, Color(200, 0, 0));
			draw_rect(-20, 0, 10, 10, Color(200, 200, 200));
		}

		draw_number(1, -20, 0, 2, Color(255, 255, 255));
		draw_number(2, 20, 0, 2, Color(255, 255, 255));
	}
	else if (game_mode == GM_GAMEPLAY)
	{
		const Color BALL_COLOR(255, 255, 255);
		const Color PADDLE_COLOR(255, 0, 0);

		draw_rect(0, 0, arena_half_size_x, arena_half_size_y, Color(255, 200, 40));

		float player_1_ddp = 0;
		if (is_down(BUTTON_UP)) player_1_ddp += 2000;
		if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;

		float player_2_ddp = 0;
		if(enemy_is_ai)
		{
			if (ball_pos_y > player_2_pos + 2.f) player_2_ddp += 1200;
			if (ball_pos_y < player_2_pos - 2.f) player_2_ddp -= 1200;
		}
		else
		{
			if (is_down(BUTTON_W)) player_2_ddp += 2000;
			if (is_down(BUTTON_S)) player_2_ddp -= 2000;
		}

		update_player_position(player_1_pos, player_1_dp, player_1_ddp, delta);
		update_player_position(player_2_pos, player_2_dp, player_2_ddp, delta);

		ball_pos_x += ball_dp_x * delta;
		ball_pos_y += ball_dp_y * delta;

		if (ball_collides_with_player(PLAYER_OFFSET, player_1_pos))
		{
			ball_pos_x = PLAYER_OFFSET - ball_half_size - player_half_size_x;
			ball_dp_x *= -1;
			ball_dp_y = std::min(ball_dp_y + player_1_dp, 300.f);
		}
		else if (ball_collides_with_player(-PLAYER_OFFSET, player_2_pos))
		{
			ball_pos_x = -PLAYER_OFFSET + ball_half_size + player_half_size_x;
			ball_dp_x *= -1;
			ball_dp_y = std::min(ball_dp_y + player_2_dp, 300.f);
		}

		// Top collision
		if (ball_pos_y + ball_half_size > arena_half_size_y)
		{
			ball_pos_y = arena_half_size_y - ball_half_size;
			ball_dp_y *= -1;
		}
		// Bottom collision
		else if (ball_pos_y - ball_half_size < -arena_half_size_y)
		{
			ball_pos_y = -arena_half_size_y + ball_half_size;
			ball_dp_y *= -1;
		}

		{
			auto reset_ball = [](float dx)
				{
					ball_pos_x = 0;
					ball_pos_y = 0;
					ball_dp_x = dx;
					ball_dp_y = 0;
				};

			// Player 1 (right)
			if (ball_pos_x + ball_half_size > arena_half_size_x)
			{
				player_2_score += 1;
				reset_ball(100);
			}
			// Player 2 (left)
			else if (ball_pos_x - ball_half_size < -arena_half_size_x)
			{
				player_1_score += 1;
				reset_ball(-100);
			}
		}

		draw_number(player_1_score, 10, 40, 1.f, Color(255, 255, 255));
		draw_number(player_2_score, -10, 40, 1.f, Color(255, 255, 255));

		draw_rect(ball_pos_x, ball_pos_y, ball_half_size, ball_half_size, BALL_COLOR);

		draw_rect(PLAYER_OFFSET, player_1_pos, 2.5, 12, PADDLE_COLOR);
		draw_rect(-PLAYER_OFFSET, player_2_pos, 2.5, 12, PADDLE_COLOR);
	}
}