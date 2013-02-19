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
#ifndef SDL_HELPER_H
#define SDL_HELPER_H

#include "SDL.h"
#include "SDL_image.h"
#include <memory>

using SdlSurface = std::shared_ptr<SDL_Surface>;

// Handle to the screen (duh).  Must fill this in with SDL_SetVideoMode before
// any other SDL drawing functions will work.
extern SDL_Surface *screen;

// Like std::make_shared, but with SDL_Surface.
SdlSurface make_surface(SDL_Surface *surf);

// Create a new surface of the given width and height.  Return a null surface
// on failure.
SdlSurface sdlCreateSurface(Sint16 width, Sint16 height);

// Convert the given surface to the screen format.  Return a null surface on
// failure.
SdlSurface sdlDisplayFormat(const SdlSurface &src);

// Draw the full surface to the screen using (px,py) as the upper-left corner.
// Use the raw SDL_BlitSurface if you need something more specific.
void sdlBlit(const SdlSurface &surf, Sint16 px, Sint16 py);

// Clear the given region of the screen.
void sdlClear(SDL_Rect region);

// Load an image from disk.  Returns a null surface on failure.
SdlSurface sdlLoadImage(const char *filename);

// Draw a dashed line to the screen starting at (px,py).
void sdlDashedLineH(Sint16 px, Sint16 py, Uint16 len, Uint32 color);
void sdlDashedLineV(Sint16 px, Sint16 py, Uint16 len, Uint32 color);

#endif
