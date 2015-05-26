#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include <string>

class Sound {
public:
	/// returns a reference to the sound object. By it has 8 channels to play sounds on. Music has its own channel.
    Sound();

	~Sound();

	// Music

	/// loads music into memory, returns 0 if musicName already exists.
	int loadMusic(const char* musicName, const char* fileName);

	///returns music to do stuff with using SDL_Mixer
	Mix_Music* getMusic(const char* bgmName);

	/// removes music from memory.
	void removeMusic(const char* name);

	// Sound Effects

	/// loads a sound into memory, returns 0 if musicName already exists.
	int loadSFX(const char* musicName, const char* fileName);

	///returns a sound to do stuff with using SDL_Mixer
	Mix_Chunk* getSFX(const char* bgmName);

	/// removes a sound from memory.
	void removeSFX(const char* name);

private:

	// for Mix_CloseAudio, need to keep count how many openAudio were called.
	int openedAudio;

	// music
	std::map<std::string,Mix_Music*> bgm;

	// sound effect
	std::map<std::string,Mix_Chunk*> sfx;

};
