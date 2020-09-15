
#pragma once

#include "CommonPublic.h"
#include "CommonLog.h"
#include "CommonSingleInstance.h"

typedef
    HRESULT
    ( STDAPICALLTYPE * Shell32_dll_SHCreateItemFromParsingName_Proc_Type)(
    __in PCWSTR pszPath,
    __in_opt IBindCtx *pbc,
    __in REFIID riid,
    __deref_out void **ppv
    );

class CCommonVistaHelper : public CCommonSingleInstance<CCommonVistaHelper>
{
    friend class CCommonSingleInstance<CCommonVistaHelper>;

public:
    COMMON_ERROR
        Init();

    COMMON_ERROR
        Unload();

    HRESULT
        SHCreateItemFromParsingName(
        __in PCWSTR pszPath,
        __in_opt IBindCtx *pbc,
        __in REFIID riid,
        __deref_out void **ppv
        );

private:
    HMODULE m_hShell32;
    Shell32_dll_SHCreateItemFromParsingName_Proc_Type m_Shell32_dll_SHCreateItemFromParsingName;

    CCommonVistaHelper();

    ~CCommonVistaHelper();
};
