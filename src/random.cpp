/*
    Copyright (C) 2012-2013 by Michael Kristofik <kristo605@gmail.com>
    Part of the libsdl-demos project.
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    or at your option any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY.
 
    See the COPYING.txt file for more details.
*/
#include "HexGrid.h"
#include "Minimap.h"
#include "RandomMap.h"
#include "hex_utils.h"
#include "sdl_helper.h"
#include "terrain.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>

extern "C" int SDL_main(int, char **)  // 2-arg form is required by SDL
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        std::cerr << "Error initializing SDL: " << SDL_GetError();
        return EXIT_FAILURE;
    }
    atexit(SDL_Quit);

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cerr << "Error initializing SDL_image: " << IMG_GetError();
        return EXIT_FAILURE;
    }
    atexit(IMG_Quit);

    // Have to do this prior to SetVideoMode.
    auto icon = make_surface(IMG_Load("../img/icon.png"));
    if (icon != nullptr) {
        SDL_WM_SetIcon(icon.get(), nullptr);
    }
    else {
        std::cerr << "Warning: error loading icon file: " << IMG_GetError();
    }

    screen = SDL_SetVideoMode(1112, 704, 0, SDL_SWSURFACE);
    if (screen == nullptr) {
        std::cerr << "Error setting video mode: " << SDL_GetError();
        return EXIT_FAILURE;    
    }
    SDL_WM_SetCaption("Random Map Test", "");

    // Display area sized to hold 16x9 hexes.
    SDL_Rect mapArea = {10, 10, 882, 684};
    RandomMap m(18, 11, mapArea);
    Minimap mini(200, m);

    // FIXME: unit tests for this would require an SDL main.  These assume the
    // map is drawn in the upper left corner of the screen.
    /*
    assert(str(m.getHexAtS(-1, -1)) == str(hInvalid));
    assert(str(m.getHexAtS(0, 0)) == str({-1, -1}));
    assert(str(m.getHexAtS(36, 36)) == str({0, 0}));
    assert(str(m.getHexAtS(36, 108)) == str({0, 1}));
    assert(str(m.getHexAtS(90, 144)) == str({1, 1}));
    */

    auto white = SDL_MapRGB(screen->format, 255, 255, 255);
    for (Sint16 x = 0, y = 0; x <= 108; x += 18, y += 24) {
        m.draw(x, y);

        // FIXME: the minimap and bounding box can be turned into
        // RandomMap::drawMinimap().
        Sint16 sx = 902;
        Sint16 sy = 10;
        mini.draw(sx, sy);
        Sint16 box_nw_x = sx + static_cast<double>(x) / m.pWidth() * mini.width();
        Sint16 box_nw_y = sy + static_cast<double>(y) / m.pHeight() * mini.height();
        Sint16 box_se_x = sx + static_cast<double>(x + mapArea.w - 1) / m.pWidth() * mini.width();
        Sint16 box_se_y = sy + static_cast<double>(y + mapArea.h - 1) / m.pHeight() * mini.height();
        Uint16 box_width = box_se_x - box_nw_x;
        Uint16 box_height = box_se_y - box_nw_y;
        sdlDashedLineH(box_nw_x, box_nw_y, box_width, white);
        sdlDashedLineV(box_nw_x, box_nw_y, box_height, white);
        sdlDashedLineH(box_nw_x, box_se_y, box_width, white);
        sdlDashedLineV(box_se_x, box_nw_y, box_height, white);

        SDL_UpdateRect(screen, 0, 0, 0, 0);
        SDL_Delay(1000);
    }

    bool isDone = false;
    SDL_Event event;
    while (!isDone) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isDone = true;
            }
        }
        SDL_UpdateRect(screen, 0, 0, 0, 0);
        SDL_Delay(1);
    }

    return EXIT_SUCCESS;
}
