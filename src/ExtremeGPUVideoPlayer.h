#pragma once

#include "ofMain.h"
#include "ofxExtremeGpuVideo.hpp"

class ExtremeGPUVideoPlayer
{
public:

	ExtremeGPUVideoPlayer() :
		state(State::init),
		last_frm_num(0), 
		movie_finish_time(ofGetElapsedTimef()),
		speed(1.0), 
		b_loop(true),
		b_use_sound(false),
		b_frame_new(false)
	{}

	~ExtremeGPUVideoPlayer() {}

	void load(
		const std::filesystem::path _movie_path, 
		const std::filesystem::path _audio_path = "")
	{
		extreme_gpu_video.load(
			_movie_path.string(), 
			ofxExtremeGpuVideo::GPU_VIDEO_STREAMING_FROM_STORAGE);

		fbo.allocate(getWidth(), getHeight());
		fbo.begin();
		ofClear(0);
		fbo.end();

		if (_audio_path == "")
			b_use_sound = false;
		else
		{
			b_use_sound = true;
			sound.load(_audio_path);
			sound.setMultiPlay(false);
			sound.setLoop(false);
		}

		state = State::loaded;
	}

	void close()
	{

	}

	bool isInitialized()
	{
		if (state != State::init)
			return true;
		else
			return false;
	}

	void update()
	{
		auto update_mov = [&]() -> void
		{
			auto fnum = get_current_frame_number_based_on_speed();

			extreme_gpu_video.setFrame(fnum);
			extreme_gpu_video.update();

			fbo.begin();
			ofClear(0);
			extreme_gpu_video.begin();
			extreme_gpu_video.getPlaceHolderTexture().draw(0, 0);
			extreme_gpu_video.end();
			fbo.end();
		};

		auto b_mov_done = is_mov_done();

		if (state == State::playing)
		{
			if (!b_loop)
			{
				if (b_mov_done)
				{
					stop();
					state = State::done;
				}
				else
					update_mov();
			}
			else
			{
				if (b_mov_done)
					resume();

				update_mov();
			}
		}
	}

	void draw(const float _x, const float _y)
	{
		draw(_x, _y, getWidth(), getHeight());
	}

	void draw(
		const float _x, const float _y, 
		const float _w, const float _h)
	{
		if (state != State::init)
		{
			fbo.draw(_x, _y, _w, _h);
		}
	}

	bool isFrameNew()
	{
		return b_frame_new;
	}

	void play()
	{
		if (state != State::init)
		{
			state = State::playing;
			resume();
		}
	}

	void stop()
	{
		if (state != State::init)
		{
			state = State::stop;
			reset();

			if (b_use_sound)
				sound.stop();
		}
	}

	void setPaused(const bool _b_paused)
	{
		if (_b_paused)
		{
			if (state == State::playing)
			{
				state = State::paused;
				pausing_begin_time = ofGetElapsedTimef();

				if (b_use_sound)
					sound.setPaused(true);
			}
		}
		else
		{
			if (state == State::paused)
			{
				state = State::playing;

				auto pausing_dur = ofGetElapsedTimef() - pausing_begin_time;
				movie_finish_time += pausing_dur;

				if (b_use_sound)
					sound.setPaused(false);
			}
		}
	}

	bool isPaused()
	{
		if (state == State::paused)
			return true;
		else
			return false;
	}

	float getWidth()
	{
		return extreme_gpu_video.getWidth();
	}

	float getHeight()
	{
		return extreme_gpu_video.getHeight();
	}

	bool isLoaded()
	{
		if (state != State::init)
			return true;
		else
			return false;
	}

	bool isPlaying()
	{
		if (state == State::playing)
			return true;
		else
			return false;
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
		if (state == State::done)
			return true;
		else
			return false;
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
		b_loop = _b_loop;
	}

	void setVolume(const float _volume)
	{

	}

	ofEvent<void> mov_finish_event;

protected:

	void resume()
	{
		reset();

		if (b_use_sound)
		{
			sound.setPosition(0.0);
			sound.play();
		}
	}

	void reset()
	{
		last_frm_num = 0;
		movie_finish_time = ofGetElapsedTimef();
		extreme_gpu_video.setFrame(0);
		extreme_gpu_video.update();
	}

	int get_current_frame_number_based_on_speed()
	{
		auto& vid = extreme_gpu_video;
		auto total_frame_on_true_fps = vid.getFrameCount();
		auto dur = get_movie_dur_by_speed();
		auto elapsed = ofGetElapsedTimef() - movie_finish_time;
		auto cur_time = ofClamp(fmod(elapsed, dur), 0.0, dur);
		auto the_fps = vid.getFramePerSecond() * speed;
		int frame = ofClamp(cur_time * the_fps, 0, total_frame_on_true_fps);
		//ofLogNotice(__FUNCTION__) << id_str << " " << _movie->type_str << " time = " << cur_time << " / " << dur;
		return frame;
	}

	bool is_mov_done()
	{
		auto& vid = extreme_gpu_video;
		int cur_frm_num = vid.getFrameAt();

		if (cur_frm_num != last_frm_num)
			b_frame_new = true;
		else
			b_frame_new = false;

		if (cur_frm_num == vid.getFrameCount() - 1)
		{
			//ofLogNotice(__FUNCTION__) << id_str << " done " << type_str << " movie at " << ofToString(cur_frm_num) << ", elp:" << Globals::ELAPSED_TIME;
			last_frm_num = 0;
			vid.setFrame(0);
			vid.update();
			movie_finish_time = ofGetElapsedTimef();
			ofNotifyEvent(mov_finish_event);
			return true;
		}
		else if (cur_frm_num < last_frm_num)
		{
			//ofLogNotice(__FUNCTION__) << id_str << " done " << type_str << " movie at " << ofToString(cur_frm_num) << " and last frm is " << last_frm_num << ", elp:" << Globals::ELAPSED_TIME;
			last_frm_num = 0;
			vid.setFrame(0);
			vid.update();
			movie_finish_time = ofGetElapsedTimef();
			ofNotifyEvent(mov_finish_event);
			return true;
		}
		else
		{
			//ofLogNotice(__FUNCTION__) << id_str << " playing " << type_str << " movie at " << ofToString(cur_frm_num) << " and last frm is " << last_frm_num << ", elp:" << Globals::ELAPSED_TIME;
			last_frm_num = cur_frm_num;
			return false;
		}
	}

	float get_movie_dur_by_speed()
	{
		auto& vid = extreme_gpu_video;
		auto dur = vid.getDuration() / speed;
		return dur;
	}

	enum struct State { init, loaded, stop, paused, playing, scrubbing, done }; State state;
	float speed;
	float movie_finish_time, pausing_begin_time;
	int last_frm_num;
	bool b_loop;
	bool b_frame_new;

	ofxExtremeGpuVideo extreme_gpu_video;
	ofFbo fbo;

	ofSoundPlayer sound;
	bool b_use_sound;
};