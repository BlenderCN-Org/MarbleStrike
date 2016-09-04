#ifndef AUDIO_SOUNDBANK_HPP
#define AUDIO_SOUNDBANK_HPP

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class SoundBank
	{
	public:
		SoundBank() 
		{
		}

		~SoundBank() 
		{
		}

		
		void AddFile(const char* filename) 
		{ 
			mSoundList.PushBack( filename ); 
		}

		void Clear() 
		{ 
			mSoundList.Clear(); 
		}

		int GetSize() const
		{ 
			return mSoundList.GetSize(); 
		}

		const char* GetFileAt(int index) const
		{ 
			return mSoundList[index]; 
		}

	private:

		Array< System::FixedString<256> > mSoundList;
	};

}

#endif
