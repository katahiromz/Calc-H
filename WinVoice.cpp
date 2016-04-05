// WinVoice.cpp --- SAPI wrapper module
// See ReadMe.txt and License.txt.
////////////////////////////////////////////////////////////////////////////

#include "WinVoice.h"

#ifdef WIN_VOICE_TEST
    int main(void) {
        CoInitialize(NULL);
        {
            WinVoice voice;
            voice.SetVolume(100);
            voice.Speak("aroha", false);
            voice.Speak(L"aroha", false);
        }
        CoUninitialize();
        return 0;
    }
#endif

////////////////////////////////////////////////////////////////////////////
