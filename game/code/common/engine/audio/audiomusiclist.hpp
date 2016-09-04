#ifndef AUDIO_MUSICLIST_HPP
#define AUDIO_MUSICLIST_HPP

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

#include "common/engine/system/array.hpp"
#include "common/engine/system/fixedstring.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////
namespace Audio
{
	class MusicList
	{
	public:
		MusicList()
			: mCurrentIndex( 0 )
			, mRandom( false )
			, mLoop( false )
		{

		}
		~MusicList()
		{
		}

		void AddFile(const char* filename)
		{
			mMusicList.PushBack(filename);
		}

		void Clear()
		{
			mMusicList.Clear();
		}

		void SetRandom(bool rand)
		{
			mRandom = rand;
		}

		bool GetRandom()
		{
			return mRandom;
		}

		void SetLoop(bool loop)
		{
			mLoop = loop;
		}

		bool GetLoop()
		{
			return mLoop;
		}

		const char* GetNextFile()
		{
			mCurrentIndex++;

			if ( !mLoop )
			{
				// if loop is not set, then stop
				if ( mCurrentIndex >= mMusicList.GetSize() )
				{
					return NULL;
				}
			}

			mCurrentIndex = mCurrentIndex % mMusicList.GetSize();

			return mMusicList[mCurrentIndex];
		}

		const char* GetPrevFile()
		{
			mCurrentIndex--;
			if (mCurrentIndex < 0)
			{
				mCurrentIndex = mMusicList.GetSize() - 1;
			}

			return mMusicList[mCurrentIndex];
		}

		const char* GetCurrentFile()
		{
			return mMusicList[mCurrentIndex];
		}

		bool SetCurrentFile(const char* filename)
		{
			for ( int i = 0; i < mMusicList.GetSize(); i++ )
			{
				if (System::StringICmp(mMusicList[i], filename) == 0)
				{
					mCurrentIndex = i;
					return true;
				}
			}
			return false;
		}

	private:

		Array< System::FixedString<256> > mMusicList;
		int mCurrentIndex;
		bool mRandom;
		bool mLoop;
	};
}

#endif