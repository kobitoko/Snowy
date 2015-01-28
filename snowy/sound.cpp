#include "sound.h"
#include "errorHandler.h"

Sound::Sound() {
	if(SDL_Init(SDL_INIT_AUDIO) != 0) {
		std::string errmsg = "SDL_Init error: " + std::string(SDL_GetError());
		callError(errmsg);
	}
	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	int inits = Mix_Init(flags);
	if((inits & flags) != flags) {
		std::string errmsg = "Mix_Init error: " + std::string(Mix_GetError());
		callError(errmsg);
	}
	//  MIX_DEFAULT_FREQUENCY is 22050Hz , MIX_DEFAULT_FORMAT is AUDIO_S16SYS. http://jcatki.no-ip.org:8080/SDL_mixer/SDL_mixer_11.html#SEC11
	 if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		 std::string errmsg = "Mix_OpenAudio error: " + std::string(Mix_GetError());
		 callError(errmsg);
	 }
	openedAudio = 1;
	//Mix_Chunk is SFX, Mix_Music is BGM;
}

Sound::~Sound() {
	for(auto&& it: sfx)
		Mix_FreeChunk(it.second);
	for(auto&& it: bgm)
			Mix_FreeMusic(it.second);
	for(int i=0; i < openedAudio; ++i)
		Mix_CloseAudio();
	Mix_Quit();
}

Sound& Sound::get() {
	static Sound sndMaster;
	return sndMaster;
}

int Sound::loadMusic(const char* musicName, const char* fileName) {
	if(bgm.count(musicName) > 0)
		return 0;
	bgm[musicName] = Mix_LoadMUS(fileName);
	if(bgm[musicName] == nullptr)	{
		std::string errmsg = "Sound method loadMusic error: " + std::string(Mix_GetError());
		callError(errmsg);
	}
	return 1;
}

Mix_Music* Sound::getMusic(const char* bgmName) {
	if(bgm.count(bgmName) == 0)
		return nullptr;
	return bgm[bgmName];
}

void Sound::removeMusic(const char* name) {
	if(bgm.count(name) > 0) {
		Mix_FreeMusic(bgm[name]);
		bgm.erase(name);
	}
}


int Sound::loadSFX(const char* sfxName, const char* fileName) {
	if(sfx.count(sfxName) > 0)
		return 0;
	sfx[sfxName] = Mix_LoadWAV(fileName);
	if(sfx[sfxName] == nullptr)	{
		std::string errmsg = "Sound method loadMusic error: " + std::string(Mix_GetError());
		callError(errmsg);
	}
	return 1;
}

Mix_Chunk* Sound::getSFX(const char* sfxName) {
	if(sfx.count(sfxName) == 0)
		return nullptr;
	return sfx[sfxName];
}

void Sound::removeSFX(const char* name) {
	if(bgm.count(name) > 0) {
		Mix_FreeChunk(sfx[name]);
		bgm.erase(name);
	}
}
