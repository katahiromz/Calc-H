// WinVoice.h --- SAPI wrapper module
// See ReadMe.txt and License.txt.
////////////////////////////////////////////////////////////////////////////

#ifndef WIN_VOICE_H
#define WIN_VOICE_H

#include <string>

//#define NO_ATLBASEFAKE
//#define ATLBASEFAKE_CXX11
#include "atlbasefake.h"

#include <sapi.h>

////////////////////////////////////////////////////////////////////////////

// NOTE: CoInitialize or CoInitializeEx call is required.
class WinVoice {
public:
    WinVoice() {
        HRESULT hr = m_voice.CoCreateInstance(CLSID_SpVoice);
        m_available = SUCCEEDED(hr);
        if (m_available) {
            hr = m_voice->SetOutput(NULL, TRUE);
        }
        m_mute = false;
    }
    HRESULT Speak(const std::wstring& str, bool async = true) {
        if (m_mute) {
            return 0;
        }
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
    void SetMute(BOOL on) {
        if (on) {
            m_voice->Speak(L"", SPF_PURGEBEFORESPEAK, NULL);
        }
        m_mute = on;
    }
    bool IsMute() const { return m_mute; }

protected:
    bool m_available;
    CComPtr<ISpVoice> m_voice;
    bool m_mute;
}; // class WinVoice

////////////////////////////////////////////////////////////////////////////

#endif  // ndef WIN_VOICE_H

////////////////////////////////////////////////////////////////////////////
