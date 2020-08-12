
#pragma once

#include "CommonPublic.h"

#include "CommonNickname.h"

template<class T>
class CCommonSingleInstance
{
public:
    static T * WINAPI GetInstance()
    {
        if (NULL == ms_pInstance)
        {
            CommonWriteLock lock(ms_InstanceLock);
            if (NULL == ms_pInstance)
            {
                ms_pInstance = new T;
            }
        }

        return ms_pInstance;
    }

    static VOID WINAPI ReleaseInstance()
    {
        if (NULL != ms_pInstance)
        {
            CommonWriteLock lock(ms_InstanceLock);
            if (NULL != ms_pInstance)
            {
                delete ms_pInstance;
                ms_pInstance = NULL;
            }
        }

        return;
    }

    BOOL
        CheckInitFlag()
    {
        return m_bInitFlag;
    }

    VOID
        SetReadyFlag(
        _In_ CONST BOOL & bFlag
        )
    {
        m_bReadyFlag = bFlag;

        return ;
    }

    BOOL
        GetReadyFlag()
    {
        return m_bReadyFlag;
    }

protected:
    CommonMutex ms_InitFlagLock;
    BOOL m_bInitFlag;
    BOOL m_bReadyFlag;

    CCommonSingleInstance()
    {
        m_bInitFlag = FALSE;
        m_bReadyFlag = FALSE;
    }

    ~CCommonSingleInstance()
    {
        m_bInitFlag = FALSE;
        m_bReadyFlag = FALSE;
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

private:
    static T * ms_pInstance;
    static CommonMutex ms_InstanceLock;
};

template<class T> T * CCommonSingleInstance<T>::ms_pInstance = NULL;
template<class T> CommonMutex CCommonSingleInstance<T>::ms_InstanceLock;
