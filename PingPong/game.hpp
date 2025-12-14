#pragma once

struct RenderState;
struct Input;

enum GameMode
{
	GM_MENU,
	GM_GAMEPLAY,
};

void simulate_game(const Input& input, float delta);