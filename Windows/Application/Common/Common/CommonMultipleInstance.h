
#pragma once

#include "CommonPublic.h"

#include "CommonNickname.h"

template<class T>
class CCommonMultipleInstance
{
protected:
    CommonMutex ms_InitFlagLock;
    BOOL m_bInitFlag;

    CCommonMultipleInstance()
    {
        m_bInitFlag = FALSE;
    }

    ~CCommonMultipleInstance()
    {
        m_bInitFlag = FALSE;
    }

    VOID
        SetInitFlag(
        _In_ CONST BOOL & bFlag
        )
    {
        m_bInitFlag = bFlag;

        return;
    }

    BOOL
        GetInitFlag()
    {
        return m_bInitFlag;
    }
};
