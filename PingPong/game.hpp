#pragma once

struct RenderState;
struct Input;

void simulate_game(const RenderState& render_state, const Input& input, float delta);