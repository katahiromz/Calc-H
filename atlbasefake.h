// atlbasefake.h --- bogus CComPtr and CComQIPtr
// Copyright (C) 2016 Katayama Hirofumi MZ. All Rights Reserved.
// This file is public domain software (PDS). Use it at your own risk.
//////////////////////////////////////////////////////////////////////////////

#ifndef ATLBASEFAKE_H
#define ATLBASEFAKE_H     1   // Version 1

#ifndef __cplusplus
  #error C++ compiler was not enabled. Use .cpp or .cc extension. You lose.
#endif

#ifndef ATLBASEFAKE_CXX11
  #if (__cplusplus >= 201103L)  // C++11
    #define ATLBASEFAKE_CXX11
  #endif
#endif

//////////////////////////////////////////////////////////////////////////////

#ifdef __ATLBASE_H__
  // Hereby, <atlbase.h> was already included. Do nothing.
#elif defined(NO_ATLBASEFAKE)
  // Hereby, we use real <atlbase.h>.
  #include <atlbase.h>
#else  // ndef NO_ATLBASEFAKE
  // OK, let's do fake!
  #ifndef _INC_WINDOWS
    #include <windows.h>
  #endif
  #include <oleauto.h>  // for CoCreateInstance
  #include <unknwn.h>   // for IUnknown
  #include <cassert>    // for assert

  inline IUnknown* AtlComPtrAssign(IUnknown** pp, IUnknown* lp) {
    if (NULL == pp) return NULL;
    if (lp) lp->AddRef();
    if (*pp) (*pp)->Release();
    *pp = lp;
    return lp;
  }
  #ifdef ATLBASEFAKE_CXX11
    inline IUnknown* AtlComPtrAssign(std::nullptr_t, IUnknown* lp) {
      return NULL;
    }
  #endif

  inline IUnknown* AtlComQIPtrAssign(IUnknown** pp, IUnknown* lp,
                                     REFIID riid) {
    if (NULL == pp) return NULL;
    IUnknown* temp = *pp;
    *pp = NULL;
    if (lp) lp->QueryInterface(riid, reinterpret_cast<void**>(pp));
    if (temp) temp->Release();
    return *pp;
  }
  #ifdef ATLBASEFAKE_CXX11
    inline IUnknown* AtlComQIPtrAssign(std::nullptr_t, IUnknown* lp,
                                       REFIID riid) {
      return NULL;
    }
  #endif

  template <class T>
  class _NoAddRefReleaseOnCComPtr : public T {
   private:
    STDMETHOD_(ULONG, AddRef)() = 0;
    STDMETHOD_(ULONG, Release)() = 0;
  };

  template <class T>
  class CComPtrBase {
   protected:
    CComPtrBase() throw() { p = NULL; }
    CComPtrBase(int nNull) throw() {
      assert(nNull == 0);
      (void)nNull;
      p = NULL;
    }
  #ifdef ATLBASEFAKE_CXX11
    CComPtrBase(std::nullptr_t) throw() { p = NULL; }
  #endif
    CComPtrBase(T* lp) throw() {
      p = lp;
      if (p) p->AddRef();
    }

   public:
    typedef T _PtrClass;
    ~CComPtrBase() throw() {
      if (p) p->Release();
    }
    operator T*() const throw() { return p; }
    T& operator*() const { return *p; }
    T** operator&() throw() {
      assert(NULL == p);
      return &p;
    }
    _NoAddRefReleaseOnCComPtr<T>* operator->() const throw() {
      assert(p);
      return (_NoAddRefReleaseOnCComPtr<T>*)p;
    }
    bool operator!() const throw() { return (NULL == p); }
    bool operator<(T* pT) const throw() { return p < pT; }
    bool operator!=(T* pT) const { return !operator==(pT); }
    bool operator==(T* pT) const throw() { return p == pT; }
    void Release() throw() {
      T* temp = p;
      if (temp) {
        p = NULL;
        temp->Release();
      }
    }
    bool IsEqualObject(IUnknown* other) throw();
  #ifdef ATLBASEFAKE_CXX11
    bool IsEqualObject(std::nullptr_t) throw();
  #endif
    void Attach(T* p2) throw() {
      if (p) p->Release();
      p = p2;
    }
    T* Detach() throw() {
      T* pt = p;
      p = NULL;
      return pt;
    }
    HRESULT CopyTo(T** ppT) throw() {
      assert(ppT);
      if (ppT == NULL) return E_POINTER;
      *ppT = p;
      if (p) p->AddRef();
      return S_OK;
    }
    HRESULT SetSite(IUnknown* punkParent) throw() {
      return AtlSetChildSite(p, punkParent);
    }
    HRESULT Advise(IUnknown* punk, const IID& iid, LPDWORD pdw) throw() {
      return AtlAdvise(p, punk, iid, pdw);
    }
    HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN punkOuter = NULL,
                             DWORD dwClsContext = CLSCTX_ALL) throw() {
      assert(NULL == p);
      return ::CoCreateInstance(rclsid, punkOuter, dwClsContext, __uuidof(T),
                                (void**)&p);
    }
    HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN punkOuter = NULL,
                             DWORD dwClsContext = CLSCTX_ALL) throw() {
      CLSID clsid;
      HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
      assert(NULL == p);
      if (SUCCEEDED(hr))
        hr = ::CoCreateInstance(clsid, punkOuter, dwClsContext, __uuidof(T),
                                (void**)&p);
      return hr;
    }
    template <class Q>
    HRESULT QueryInterface(Q** pp) const throw() {
      assert(pp);
      return p->QueryInterface(__uuidof(Q), (void**)pp);
    }
    T* p;
  };

  template <class T>
  class CComPtr : public CComPtrBase<T> {
   public:
    CComPtr() throw() {}
    CComPtr(int nNull) throw() : CComPtrBase<T>(nNull) {}
    CComPtr(T* lp) throw() : CComPtrBase<T>(lp) {}
    CComPtr(const CComPtr<T>& lp) throw() : CComPtrBase<T>(lp.p) {}
    T* operator=(T* lp) throw() {
      if (*this != lp) {
        return static_cast<T*>(
            AtlComPtrAssign(reinterpret_cast<IUnknown**>(&this->p), lp));
      }
      return *this;
    }
    template <typename Q>
    T* operator=(const CComPtr<Q>& lp) throw() {
      if (!IsEqualObject(lp)) {
        return static_cast<T*>(AtlComQIPtrAssign(
            reinterpret_cast<IUnknown**>(&this->p), lp, __uuidof(T)));
      }
      return *this;
    }
    T* operator=(const CComPtr<T>& lp) throw() {
      if (*this != lp) {
        return static_cast<T*>(
            AtlComPtrAssign(reinterpret_cast<IUnknown**>(&this->p), lp));
      }
      return *this;
    }
  };

  // specialization for IDispatch
  template <>
  class CComPtr<IDispatch> : public CComPtrBase<IDispatch> {
   public:
    CComPtr() throw() {}
    CComPtr(IDispatch* lp) throw() : CComPtrBase<IDispatch>(lp) {}
    CComPtr(const CComPtr<IDispatch>& lp) throw()
        : CComPtrBase<IDispatch>(lp.p) {}
    IDispatch* operator=(IDispatch* lp) throw() {
      if (*this != lp) {
        return static_cast<IDispatch*>(
            AtlComPtrAssign(reinterpret_cast<IUnknown**>(&p), lp));
      }
      return *this;
    }
    IDispatch* operator=(const CComPtr<IDispatch>& lp) throw() {
      if (*this != lp) {
        return static_cast<IDispatch*>(
            AtlComPtrAssign(reinterpret_cast<IUnknown**>(&p), lp.p));
      }
      return *this;
    }
    HRESULT GetPropertyByName(LPCOLESTR psz, VARIANT* pvar) throw() {
      assert(p);
      assert(pvar);
      DISPID dwDispID;
      HRESULT hr = GetIDOfName(psz, &dwDispID);
      if (SUCCEEDED(hr)) hr = GetProperty(dwDispID, pvar);
      return hr;
    }
    HRESULT GetProperty(DISPID dwDispID, VARIANT* pvar) throw() {
      return GetProperty(p, dwDispID, pvar);
    }
    HRESULT PutPropertyByName(LPCOLESTR psz, VARIANT* pvar) throw() {
      assert(p);
      assert(pvar);
      DISPID dwDispID;
      HRESULT hr = GetIDOfName(psz, &dwDispID);
      if (SUCCEEDED(hr)) hr = PutProperty(dwDispID, pvar);
      return hr;
    }
    HRESULT PutProperty(DISPID dwDispID, VARIANT* pvar) throw() {
      return PutProperty(p, dwDispID, pvar);
    }
    HRESULT GetIDOfName(LPCOLESTR psz, DISPID* pdispid) throw() {
      return p->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&psz), 1,
                              LOCALE_USER_DEFAULT, pdispid);
    }
    HRESULT Invoke0(DISPID dispid, VARIANT* pvarRet = NULL) throw() {
      DISPPARAMS dispparams = {NULL, NULL, 0, 0};
      return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,
                       &dispparams, pvarRet, NULL, NULL);
    }
    HRESULT Invoke0(LPCOLESTR pszName, VARIANT* pvarRet = NULL) throw() {
      HRESULT hr;
      DISPID dispid;
      hr = GetIDOfName(pszName, &dispid);
      if (SUCCEEDED(hr)) hr = Invoke0(dispid, pvarRet);
      return hr;
    }
    HRESULT Invoke1(DISPID dispid, VARIANT* pvarParam1,
                    VARIANT* pvarRet = NULL) throw() {
      DISPPARAMS dispparams = {pvarParam1, NULL, 1, 0};
      return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,
                       &dispparams, pvarRet, NULL, NULL);
    }
    HRESULT Invoke1(LPCOLESTR pszName, VARIANT* pvarParam1,
                    VARIANT* pvarRet = NULL) throw() {
      HRESULT hr;
      DISPID dispid;
      hr = GetIDOfName(pszName, &dispid);
      if (SUCCEEDED(hr)) hr = Invoke1(dispid, pvarParam1, pvarRet);
      return hr;
    }
    HRESULT Invoke2(DISPID dispid, VARIANT* pvarParam1, VARIANT* pvarParam2,
                    VARIANT* pvarRet = NULL) throw();
    HRESULT Invoke2(LPCOLESTR pszName, VARIANT* pvarParam1, VARIANT* pvarParam2,
                    VARIANT* pvarRet = NULL) throw() {
      HRESULT hr;
      DISPID dispid;
      hr = GetIDOfName(pszName, &dispid);
      if (SUCCEEDED(hr)) hr = Invoke2(dispid, pvarParam1, pvarParam2, pvarRet);
      return hr;
    }
    HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams,
                    VARIANT* pvarRet = NULL) throw() {
      DISPPARAMS dispparams = {pvarParams, NULL, static_cast<UINT>(nParams), 0};
      return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD,
                       &dispparams, pvarRet, NULL, NULL);
    }
    HRESULT InvokeN(LPCOLESTR pszName, VARIANT* pvarParams, int nParams,
                    VARIANT* pvarRet = NULL) throw() {
      HRESULT hr;
      DISPID dispid;
      hr = GetIDOfName(pszName, &dispid);
      if (SUCCEEDED(hr)) hr = InvokeN(dispid, pvarParams, nParams, pvarRet);
      return hr;
    }
    static HRESULT PutProperty(IDispatch* p, DISPID dwDispID,
                               VARIANT* pvar) throw() {
      assert(p);
      assert(pvar);
      if (NULL == pvar) return E_POINTER;
      if (NULL == p) return E_INVALIDARG;

      DISPPARAMS dispparams = {NULL, NULL, 1, 1};
      dispparams.rgvarg = pvar;
      DISPID dispidPut = DISPID_PROPERTYPUT;
      dispparams.rgdispidNamedArgs = &dispidPut;

      if (VT_UNKNOWN == pvar->vt || VT_DISPATCH == pvar->vt ||
          (pvar->vt & VT_ARRAY) || (pvar->vt & VT_BYREF)) {
        HRESULT hr =
            p->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT,
                      DISPATCH_PROPERTYPUTREF, &dispparams, NULL, NULL, NULL);
        if (SUCCEEDED(hr)) return hr;
      }
      return p->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT,
                       DISPATCH_PROPERTYPUT, &dispparams, NULL, NULL, NULL);
    }
  #ifdef ATLBASEFAKE_CXX11
    static HRESULT PutProperty(IDispatch*, DISPID, std::nullptr_t) throw() {
      return E_POINTER;
    }
    static HRESULT PutProperty(std::nullptr_t, DISPID, VARIANT*) throw() {
      return E_INVALIDARG;
    }
  #endif
    static HRESULT GetProperty(IDispatch* p, DISPID dwDispID,
                               VARIANT* pvar) throw() {
      assert(p);
      assert(pvar);
      if (NULL == pvar) return E_POINTER;
      if (NULL == p) return E_INVALIDARG;
      DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
      return p->Invoke(dwDispID, IID_NULL, LOCALE_USER_DEFAULT,
                       DISPATCH_PROPERTYGET, &dispparamsNoArgs, pvar, NULL,
                       NULL);
    }
  #ifdef ATLBASEFAKE_CXX11
    static HRESULT GetProperty(IDispatch*, DISPID, std::nullptr_t) throw() {
      assert(0);
      return E_POINTER;
    }
    static HRESULT GetProperty(std::nullptr_t, DISPID, VARIANT*) throw() {
      assert(0);
      return E_INVALIDARG;
    }
  #endif
  };

  template <class T, const IID* piid = &__uuidof(T)>
  class CComQIPtr : public CComPtr<T> {
   public:
    CComQIPtr() throw() {}
    CComQIPtr(T* lp) throw() : CComPtr<T>(lp) {}
    CComQIPtr(const CComQIPtr<T, piid>& lp) throw() : CComPtr<T>(lp.p) {}
    CComQIPtr(IUnknown* lp) throw() {
      if (lp) lp->QueryInterface(*piid, (void**)&this->p);
    }
    T* operator=(T* lp) throw() {
      if (*this != lp) {
        return static_cast<T*>(
            AtlComPtrAssign(reinterpret_cast<IUnknown**>(&this->p), lp));
      }
      return *this;
    }
    T* operator=(const CComQIPtr<T, piid>& lp) throw() {
      if (*this != lp) {
        return static_cast<T*>(
            AtlComPtrAssign(reinterpret_cast<IUnknown**>(&this->p), lp.p));
      }
      return *this;
    }
    T* operator=(IUnknown* lp) throw() {
      if (*this != lp) {
        return static_cast<T*>(
            AtlComQIPtrAssign(reinterpret_cast<IUnknown**>(&this->p), lp,
                              *piid));
      }
      return *this;
    }
  };

  // specialization for IUnknown
  template <>
  class CComQIPtr<IUnknown, &IID_IUnknown> : public CComPtr<IUnknown> {
   public:
    CComQIPtr() throw() {}
    CComQIPtr(IUnknown* lp) throw() {
      if (lp) lp->QueryInterface(__uuidof(IUnknown), (void**)&p);
    }
    CComQIPtr(const CComQIPtr<IUnknown, &IID_IUnknown>& lp) throw()
        : CComPtr<IUnknown>(lp.p) {}
    IUnknown* operator=(IUnknown* lp) throw() {
      if (*this != lp) {
        return AtlComQIPtrAssign(reinterpret_cast<IUnknown**>(&p), lp,
                                 __uuidof(IUnknown));
      }
      return *this;
    }
    IUnknown* operator=(const CComQIPtr<IUnknown, &IID_IUnknown>& lp) throw() {
      if (*this != lp) {
        return AtlComPtrAssign(reinterpret_cast<IUnknown**>(&p), lp.p);
      }
      return *this;
    }
  };

  template <class T>
  bool CComPtrBase<T>::IsEqualObject(IUnknown* other) throw() {
    if (NULL == p && NULL == other) return true;
    if (NULL == p || NULL == other) return false;
    CComPtr<IUnknown> punk1;
    CComPtr<IUnknown> punk2;
    p->QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&punk1));
    other->QueryInterface(__uuidof(IUnknown), reinterpret_cast<void**>(&punk2));
    return punk1 == punk2;
  }
  #ifdef ATLBASEFAKE_CXX11
    template <class T>
    bool CComPtrBase<T>::IsEqualObject(std::nullptr_t) throw() {
      return NULL == p;
    }
  #endif
#endif  // ndef NO_ATLBASEFAKE

//////////////////////////////////////////////////////////////////////////////

#endif  // ndef ATLBASEFAKE_H

//////////////////////////////////////////////////////////////////////////////
