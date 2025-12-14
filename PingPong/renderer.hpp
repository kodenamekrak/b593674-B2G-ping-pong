#pragma once

#include <cstdint>

#include "RenderState.hpp"

void render_background(const RenderState& render_state);
void clear_screen(const RenderState& render_state, uint32_t color);
// x position, y position, width, height
void draw_rect_pixels(const RenderState& render_state, int x0, int x1, int y0, int y1, uint32_t color);
void draw_rect(const RenderState& render_state, float x, float y, float half_size_x, float half_size_y, uint32_t color);