// Speeching.h --- SAPI wrapper module
// See ReadMe.txt and License.txt.
////////////////////////////////////////////////////////////////////////////

#ifndef CALC_H_SPEECHING_H
#define CALC_H_SPEECHING_H

#include <string>

//#define NO_ATLBASEFAKE
//#define ATLBASEFAKE_CXX11
#include "atlbasefake.h"

#include <sapi.h>

////////////////////////////////////////////////////////////////////////////

// NOTE: CoInitialize or CoInitializeEx call is required.
class Speeching {
public:
    Speeching() {
        HRESULT hr = m_voice.CoCreateInstance(CLSID_SpVoice);
        m_available = SUCCEEDED(hr);
        if (m_available) {
            hr = m_voice->SetOutput(NULL, TRUE);
        }
    }
    HRESULT Speak(const std::wstring& str, bool async = true) {
        DWORD flags = SPF_PURGEBEFORESPEAK;
        if (async) {
            flags |= SPF_ASYNC;
        }
        return m_voice->Speak(str.c_str(), flags, NULL);
    }
    HRESULT Speak(const std::string& str, bool async = true) {
        std::wstring wide;
        int len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1,
                                        NULL, 0);
        wide.resize(len);
        ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1,
                              &wide[0], len);
        return Speak(wide, async);
    }
    bool IsAvailable() const { return this && m_available; }
    HRESULT GetVolume(USHORT *volume) { return m_voice->GetVolume(volume); }
    HRESULT SetVolume(USHORT volume) { return m_voice->SetVolume(volume); }
    HRESULT Pause() { return m_voice->Pause(); }
    HRESULT Resume() { return m_voice->Resume(); }
    HRESULT WaitUntilDone(ULONG msec) { return m_voice->WaitUntilDone(msec); }
    HRESULT GetRate(LONG *rate) { return m_voice->GetRate(rate); }
    HRESULT SetRate(LONG rate) { return m_voice->SetRate(rate); }

protected:
    bool m_available;
    CComPtr<ISpVoice> m_voice;
}; // class Speeching

////////////////////////////////////////////////////////////////////////////

#endif  // ndef CALC_H_SPEECHING_H

////////////////////////////////////////////////////////////////////////////
