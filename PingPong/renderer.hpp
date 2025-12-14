#pragma once

#include <cstdint>

#include "RenderState.hpp"

RenderState& get_render_state();

void render_background();
void clear_screen(uint32_t color);
// x position, y position, width, height
void draw_rect_pixels(int x0, int x1, int y0, int y1, uint32_t color);
void draw_rect(float x, float y, float half_size_x, float half_size_y, uint32_t color);
void draw_number(int number, float x, float y, float size, uint32_t color);