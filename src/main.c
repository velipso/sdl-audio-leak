// (c) Copyright 2017, Sean Connelly (@voidqk), http://syntheti.cc
// MIT License
// Project Home: https://github.com/voidqk/sdl-audio-leak

#include "SDL.h"

#include <stdio.h>

void sdl_copy_audio(void *userdata, Uint8* stream, int len){
	fprintf(stderr, "Not used\n");
}

int main(int argc, char **argv){
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec want, have;
	SDL_AudioDeviceID dev;
	SDL_memset(&want, 0, sizeof(want));
	want.freq = 48000;
	want.format = AUDIO_F32;
	want.channels = 2;
	want.samples = 2048;
	want.callback = sdl_copy_audio;
	dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
	if (dev == 0)
		fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
	else{
		//SDL_PauseAudioDevice(dev, 0); // begin playing
		printf("hit a key to close audio device\n");
		fgetc(stdin);
		SDL_CloseAudioDevice(dev);
	}
	printf("hit a key to exit\n");
	fgetc(stdin);
	SDL_Quit();
	return 0;
}
