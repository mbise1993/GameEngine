#pragma once
#ifndef ENGINE_SOUND_WIN32_SND_MANAGER_H
#define ENGINE_SOUND_WIN32_SND_MANAGER_H

class Win32SoundManager
{
public:

	enum SoundFile
	{
		MUSIC, STOP_ALL
	};

	static void playSound( SoundFile sound );
};

#endif

