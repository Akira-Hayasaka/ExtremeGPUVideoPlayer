#pragma once

#include "ofMain.h"
#include "ofxExtremeGpuVideo.hpp"

class ExtremeGPUVideoPlayer
{
public:

	ExtremeGPUVideoPlayer() :
		b_use_sound(false)
	{}
	~ExtremeGPUVideoPlayer() {}

	void load(
		const std::filesystem::path _movie_path, 
		const std::filesystem::path _audio_path = "")
	{
		if (_audio_path == "")
			b_use_sound = false;
		else
			b_use_sound = true;


	}

	void close()
	{

	}

	bool isInitialized()
	{

	}

	void update()
	{

	}

	void draw(const float _x, const float _y)
	{
		draw(_x, _y, getWidth(), getHeight());
	}

	void draw(
		const float _x, const float _y, 
		const float _w, const float _h)
	{

	}

	bool isFrameNew()
	{

	}

	void play()
	{

	}

	void stop()
	{

	}

	void setPaused(const bool _b_paused)
	{

	}

	bool isFrameNew()
	{

	}

	float getWidth()
	{

	}

	float getHeight()
	{

	}

	bool isPaused()
	{

	}

	bool isLoaded()
	{

	}

	bool isPlaying()
	{

	}

	ofPixels& getPixels()
	{

	}

	ofTexture& getTexture()
	{

	}

	float getDuration()
	{

	}

	bool getIsMovieDone()
	{

	}

	int getCurrentFrame()
	{

	}

	int getTotalNumFrames()
	{

	}

	float getPosition()
	{

	}

	void setPosition(const float _pct)
	{

	}

	void setFrame(const int _frame)
	{

	}

	float getSpeed()
	{

	}

	void setSpeed(const float _speed)
	{

	}

	void setLoopState(const bool _b_loop)
	{

	}

	void setVolume(const float _volume)
	{

	}

protected:

	ofxExtremeGpuVideo extream_gpu_video;
	ofFbo fbo;

	ofSoundPlayer sound;
	bool b_use_sound;
};