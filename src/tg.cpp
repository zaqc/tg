//============================================================================
// Name        : tg.cpp
// Author      : zaqc
// Version     :
// Copyright   : free
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>

#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "AccordDeviceBase.h"
#include "FrameBuffer.h"
using namespace vector;
//----------------------------------------------------------------------------

extern int g_WaitSyncCount;

int main(int argc, char *argv[]) {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Event event;

	// int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	if (SDL_CreateWindowAndRenderer(800, 480, SDL_WINDOW_BORDERLESS, &window,
			&renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"Couldn't create window and renderer: %s", SDL_GetError());
		return 3;
	}

	surface = SDL_LoadBMP("/home/zaqc/sample.bmp");
	if (!surface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"Couldn't create surface from image: %s", SDL_GetError());
		return 3;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"Couldn't create texture from surface: %s", SDL_GetError());
		return 3;
	}
	SDL_FreeSurface(surface);


	g_Device = new vector::AccordDeviceBase();
	g_Device->SetDefaultParam();

	vector::FrameBuffer *fb = new vector::FrameBuffer();
	delete fb;


	timeval ts, ts_prev;
	gettimeofday(&ts, 0);
	int fc = 0;
	ts_prev = ts;

	SDL_Point p[1024];

	while (1) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_MOUSEBUTTONDOWN)
				std::cout << event.motion.x << " " << event.motion.y
						<< std::endl;
		}

		gettimeofday(&ts, 0);
		float delta = (ts.tv_sec * 1000000 + ts.tv_usec)
				- (ts_prev.tv_sec * 1000000 + ts_prev.tv_usec);

		fc++;

		if (delta >= 1000000.0f) {
			cout << (float) fc * 1000000.0 / delta << " WaitSync=" << g_WaitSyncCount
					<< endl;

			fc = 0;
			ts_prev = ts;
		}

		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
		SDL_SetRenderDrawColor(renderer, 32, 32, 32, 0xFF);
		SDL_RenderClear(renderer);

		//for (int i = 0; i < 100; i++) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 32);

		unsigned char data[1024 * 8];
		g_Device->GetData(data, 1024 * 8);
		for (int i = 0; i < 1024; i++) {
			p[i].x = i * 800 / 1024;
			p[i].y = 400 - data[i];
		}
		SDL_RenderDrawLines(renderer, p, 1024);
		for (int i = 0; i < 256; i++) {
			p[i].x = i * 800 / 256;
			p[i].y = 480 - data[i] * 480 / 255;
		}
		SDL_RenderDrawLines(renderer, p, 256);

		//rand() % 320, rand() % 240,
		//rand() % 320, rand() % 240);
		//}

		//SDL_RenderCopy(renderer, texture, NULL, NULL);
		g_Device->SyncWait();
		SDL_RenderPresent(renderer);
	}

	delete g_Device;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}

//static bool quitting = false;
//static float r = 0.0f;
//static SDL_Window *window = NULL;
//static SDL_GLContext gl_context;
//
//void render() {
//
//	SDL_GL_MakeCurrent(window, gl_context);
//
//	r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
//
//	glClearColor(r, 0.4f, 0.1f, 1.0f);
//	glClear( GL_COLOR_BUFFER_BIT);
//
//	SDL_GL_SwapWindow(window);
//
//} //render
//
//int SDLCALL watch(void *userdata, SDL_Event* event) {
//
//	if (event->type == SDL_APP_WILLENTERBACKGROUND) {
//		quitting = true;
//	}
//
//	return 1;
//}
//
//int main(int argc, char *argv[]) {
//
//	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
//		return 1;
//	}
//
//	window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED,
//	SDL_WINDOWPOS_UNDEFINED, 640, 320, SDL_WINDOW_OPENGL);
//
//	gl_context = SDL_GL_CreateContext(window);
//
//	SDL_AddEventWatch(watch, NULL);
//
//	timeval ts, ts_prev;
//	gettimeofday(&ts, 0);
//	int fc = 0;
//	ts_prev = ts;
//
//	while (!quitting) {
//
//		gettimeofday(&ts, 0);
//		float delta = (ts.tv_sec * 1000000 + ts.tv_usec)
//				- (ts_prev.tv_sec * 1000000 + ts_prev.tv_usec);
//
//		fc++;
//
//		if (delta >= 1000000.0f) {
//			cout << (float) fc * 1000000.0 / delta << endl;
//
//			fc = 0;
//			ts_prev = ts;
//		}
//
//		SDL_Event event;
//		while (SDL_PollEvent(&event)) {
//			if (event.type == SDL_QUIT) {
//				quitting = true;
//			}
//		}
//
//		render();
//		SDL_Delay(15);
//
//	}
//
//	SDL_DelEventWatch(watch, NULL);
//	SDL_GL_DeleteContext(gl_context);
//	SDL_DestroyWindow(window);
//	SDL_Quit();
//
//	exit(0);
//
//} //main
