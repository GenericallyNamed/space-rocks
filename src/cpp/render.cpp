#include <random>
#include <stdio.h>
#include <string>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include "render.hpp"


using emscripten::val;

// GameCanvas game_canvas = GameCanvas();
thread_local const val document = val::global("document");

void render_test() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(100, 200);
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.set("fillStyle", val("green"));
    ctx.call<void>("fillRect", distrib(gen), distrib(gen), 150, 100);
}

void clear_canvas() {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    // Reset the canvas
    ctx.call<void>("reset");
}

void draw_player(float x, float y, float rotation) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.set("fillStyle", val("pink"));
    ctx.call<void>("translate", x, y);
    ctx.call<void>("rotate", rotation);
    ctx.call<void>("translate", -x, -y);
    // set stroke width
    ctx.set("lineWidth", val(1.5));
    // set stroke to white color
    ctx.set("strokeStyle", val("white"));
    ctx.call<void>("beginPath");
    ctx.call<void>("moveTo", x, y - 10);
    ctx.call<void>("lineTo", x + 10, y + 10);
    ctx.call<void>("lineTo", x, y + 7);
    ctx.call<void>("lineTo", x - 10, y + 10);
    ctx.call<void>("closePath");
    ctx.call<void>("stroke");

    // Reset the rotation
    ctx.call<void>("resetTransform");
}

void draw_asteroid(float x, float y, float rotation) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.set("fillStyle", val("red"));

    ctx.call<void>("translate", x, y);
    ctx.call<void>("rotate", rotation);
    ctx.call<void>("translate", -x, -y);

    // For now, just draw a circle
    ctx.call<void>("beginPath");
    ctx.call<void>("arc", x, y, 10, 0, 2 * M_PI);
    ctx.call<void>("closePath");
    ctx.call<void>("fill");

    // Reset the rotation
    ctx.call<void>("resetTransform");
}

void draw_bullet(float x, float y) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.set("fillStyle", val("white"));
    ctx.call<void>("beginPath");
    ctx.call<void>("arc", x, y, 2, 0, 2 * M_PI);
    ctx.call<void>("closePath");
    ctx.call<void>("fill");

    // Reset the rotation
    ctx.call<void>("resetTransform");
}