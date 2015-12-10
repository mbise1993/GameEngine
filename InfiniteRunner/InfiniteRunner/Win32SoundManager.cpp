#include "Win32SoundManager.h"
#include <Windows.h>
#include <string>

using std::wstring;

void Win32SoundManager::playSound( SoundFile sound )
{
	switch ( sound )
	{
	case MUSIC:
		PlaySound( L"SpaceDebris.wav", NULL, SND_ASYNC );
		break;
	case STOP_ALL:
		PlaySound( nullptr, NULL, SND_PURGE );
		break;
	}
}
