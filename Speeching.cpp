// Speeching.cpp --- SAPI wrapper module
// See ReadMe.txt and License.txt.
////////////////////////////////////////////////////////////////////////////

#include "Speeching.h"

#ifdef SPEECHING_TEST
    int main(void) {
        CoInitialize(NULL);
        Speeching speeching;
        speeching.SetVolume(100);
        speeching.Speak("aroha", false);
        speeching.Speak(L"aroha", false);
        CoUninitialize();
        return 0;
    }
#endif

////////////////////////////////////////////////////////////////////////////
