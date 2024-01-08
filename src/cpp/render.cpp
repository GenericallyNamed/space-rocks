#include <iostream>
#include <list>
#include <random>
#include <stdio.h>
#include <string>
#include <emscripten/emscripten.h>
#include <emscripten/val.h>
#include "render.hpp"


using emscripten::val;

std::list<Particle> particles;

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

void reset_render() {
    clear_canvas();
    particles.clear();
}

void draw_player(float x, float y, float rotation) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.call<void>("translate", x, y);
    ctx.call<void>("rotate", rotation);
    ctx.call<void>("translate", -x, -y);
    // set stroke width
    ctx.set("lineWidth", val(1.5));
    // set stroke to white color
    ctx.set("fillStyle", val("black"));
    ctx.set("strokeStyle", val("white"));
    ctx.call<void>("beginPath");
    ctx.call<void>("moveTo", x, y - 10);
    ctx.call<void>("lineTo", x + 10, y + 10);
    ctx.call<void>("lineTo", x, y + 7);
    ctx.call<void>("lineTo", x - 10, y + 10);
    ctx.call<void>("closePath");
    ctx.call<void>("stroke");
    ctx.call<void>("fill");

    // Reset the rotation
    ctx.call<void>("resetTransform");
}

void draw_asteroid(float x, float y, float rotation) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.set("fillStyle", val("black"));
    ctx.set("strokeStyle", val("white"));
    ctx.call<void>("translate", x, y);
    ctx.call<void>("rotate", rotation);
    ctx.call<void>("translate", -x, -y);

    // Draw the asteroid
    ctx.call<void>("beginPath");
    ctx.call<void>("moveTo", x, y - 10);
    ctx.call<void>("lineTo", x + 6, y - 10);
    ctx.call<void>("lineTo", x + 9, y);
    ctx.call<void>("lineTo", x + 3, y + 2);
    ctx.call<void>("lineTo", x + 8, y + 7);
    ctx.call<void>("lineTo", x - 8, y + 7);
    // Close path
    ctx.call<void>("closePath");
    // Draw stroke
    ctx.call<void>("stroke");
    // Fill the asteroid
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


struct Star {
    float x;
    float y;
    float luminosity;
};

Star stars[100];


void init_stars() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1000);
    for(int i = 0; i < 100; i++) {
        // width = 640, height = 480
        float x = distrib(gen) / 1000.0;
        float y = distrib(gen) / 1000.0;
        stars[i].x = x*640;
        stars[i].y = y*480;

        stars[i].luminosity = distrib(gen) / 1000.0;
    }
}


void draw_stars() {
    for(int i = 0; i < 100; i++) {
        draw_star(stars[i].x, stars[i].y, stars[i].luminosity);
    }
}


void draw_star(float x, float y, float luminosity) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    // calculate rgb fill value based on luminosity (0-1)
    std::string fill = "rgb(" + std::to_string(255 * luminosity) + ", " + std::to_string(255 * luminosity) + ", " + std::to_string(255 * luminosity) + ")";
    ctx.set("fillStyle", val(fill));
    ctx.call<void>("beginPath");
    ctx.call<void>("arc", x, y, 1, 0, 2 * M_PI);
    ctx.call<void>("closePath");
    ctx.call<void>("fill");

    // Reset the rotation
    ctx.call<void>("resetTransform");
}


void draw_rocket_exhaust(float x, float y, float rotation, float dx, float dy) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.set("fillStyle", val("orange"));
    ctx.call<void>("translate", x, y);
    ctx.call<void>("rotate", rotation);
    ctx.call<void>("translate", -x, -y);
    ctx.call<void>("beginPath");
    ctx.call<void>("moveTo", x, y + 7);
    ctx.call<void>("lineTo", x + 3, y + 15);
    ctx.call<void>("lineTo", x - 3, y + 15);
    ctx.call<void>("closePath");
    ctx.call<void>("fill");

    // Reset the rotation
    ctx.call<void>("resetTransform");
}


void generate_rocket_exhaust(float x, float y, float rotation, float dx, float dy) {
    // we're taking the player's velocity and using it to geenrate the particles
    
    for(int i = 0; i < 4; i++) {
        // generate dx and dy using combination of player's dx and dy and rotation
        float inv_rotation = rotation - M_PI / 2;
        float offset_range = M_PI / 3;
        float velocity_scale = 1.5;
        float rotation_offset = float(std::rand() / (RAND_MAX / 1.0)) * offset_range - (offset_range/2);
        float a_dx = -velocity_scale * std::cos(inv_rotation + rotation_offset);
        float a_dy = -velocity_scale * std::sin(inv_rotation + rotation_offset);
        // use rocket rotation to generate x and y starting point at back of rocket
        float start_dist = 8;
        float a_x = x - start_dist * std::cos(inv_rotation);
        float a_y = y - start_dist * std::sin(inv_rotation);
        
        particles.push_back(Particle(a_x, a_y, a_dx, a_dy));
    }
}

void render_particle(float x, float y) {
    val canvas = document.call<val>("getElementById", val("gameCanvas"));
    val ctx = canvas.call<val>("getContext", val("2d"));
    ctx.set("fillStyle", val("white"));
    ctx.call<void>("translate", x, y);
    ctx.call<void>("translate", -x, -y);
    ctx.call<void>("fillRect", x-2.5, y-2.5, 5, 5);
    // printf("Rendering particle at %f, %f\n", x, y);
    // Reset the rotation
    ctx.call<void>("resetTransform");
}

void draw_rocket_exhaust() {
    // We'll render and simulate the particles in the same loop
    for(auto it = particles.begin(); it != particles.end();) {
        it->x += it->dx;
        it->y += it->dy;
        it->lifetime--;
        if(it->lifetime <= 0) {
            it = particles.erase(it);
            continue;
        }
        // printf("Rendering particle at %f, %f\n", it->x, it->y);
        render_particle(it->x, it->y);
        it++;
    }
}