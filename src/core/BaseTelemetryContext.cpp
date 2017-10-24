#include "stdafx.h"
#include "Inc/BaseTelemetryContext.h"
#include "Inc/common/Utils.h"

using namespace ApplicationInsights::core;

#define AddToMapIfHasValue(map, key, value) if (value.HasValue()) { map[key] = value.GetValue(); }

/// <summary>
/// Initializes a new instance of the <see cref="BaseTelemetryContext"/> class.
/// </summary>
/// <param name="iKey">The i key.</param>
BaseTelemetryContext::BaseTelemetryContext(const std::wstring& iKey)
{
    m_iKey = iKey;
}

/// <summary>
/// Finalizes an instance of the <see cref="BaseTelemetryContext"/> class.
/// </summary>
BaseTelemetryContext::~BaseTelemetryContext()
{
}

/// <summary>
/// Initializes the context.
/// </summary>
void BaseTelemetryContext::InitContext()
{
    InitUser();
    InitDevice();
    InitApplication();
    InitSession();
}

/// <summary>
/// Initializes the user context.
/// </summary>
void BaseTelemetryContext::InitUser()
{
    //TODO: anonymize username
    wchar_t userName[UNLEN + 1];
    DWORD userNameLength = _countof(userName);
    if (!GetUserNameW(userName, &userNameLength))
    {
        userName[0] = '\0';
    }
    Nullable<std::wstring> uuid = std::wstring(userName);
    Nullable<std::wstring> date = Utils::GetCurrentDateTime();
    m_user.SetId(uuid);
    m_user.SetAccountAcquisitionDate(date);
}


/// <summary>
/// Initializes the device context.
/// </summary>
void BaseTelemetryContext::InitDevice()
{
    //TODO: https://sourceforge.net/p/predef/wiki/OperatingSystems/
    Nullable<std::wstring> strOs;
    strOs.SetValue(L"Windows");
    m_device.SetOs(strOs);

    Nullable<std::wstring> strType;
    strType.SetValue(L"Other");
    m_device.SetType(strType);

    //TODO: anonymize machine name
    Nullable<std::wstring> machineName;
    TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    GetComputerName(computerName, &size);
    machineName.SetValue(std::wstring(computerName));
    m_device.SetMachineName(machineName);
}

void BaseTelemetryContext::SetOperationID(const std::wstring& opID)
{
    m_opID = opID;
}

void BaseTelemetryContext::SetOperationName(const std::wstring& opName)
{
    m_opName = opName;
}

void BaseTelemetryContext::PushParentID(const std::wstring& parentID)
{
    m_parentIds.emplace_back(parentID);
}

void BaseTelemetryContext::PopParentID()
{
    m_parentIds.pop_back();
}

/// <summary>
/// Initializes the application.
/// </summary>
void BaseTelemetryContext::InitApplication()
{
    //TODO
    //app.SetBuild();
    //app.SetVer();
}

/// <summary>
/// Initializes the session.
/// </summary>
void BaseTelemetryContext::InitSession()
{
    Nullable<std::wstring> sessionId = Utils::GenerateRandomUUID();
    m_session.SetId(sessionId);

    Nullable<std::wstring> strTrue = std::wstring(L"True");
    m_session.SetIsFirst(strTrue);
    m_session.SetIsNew(strTrue);
}

/// <summary>
/// Renews the session.
/// </summary>
void BaseTelemetryContext::RenewSession()
{
    Nullable<std::wstring> sessionId = Utils::GenerateRandomUUID();
    m_session.SetId(sessionId);

    Nullable<std::wstring> strTrue = std::wstring(L"True");
    Nullable<std::wstring> strFalse = std::wstring(L"False");
    m_session.SetIsFirst(strFalse);
    m_session.SetIsNew(strTrue);
}

/// <summary>
/// Gets the context tags.
/// </summary>
/// <param name="tags">The tags.</param>
/// <returns>RESULT_OK if succedded</returns>
RESULT BaseTelemetryContext::GetContextTags(wstring_wstring_map& tags)
{
    tags.clear();

    AddToMapIfHasValue(tags, L"ai.user.accountAcquisitionDate", m_user.GetAccountAcquisitionDate());
    AddToMapIfHasValue(tags, L"ai.user.accountId", m_user.GetAccountId());
    AddToMapIfHasValue(tags, L"ai.user.userAgent", m_user.GetUserAgent());
    AddToMapIfHasValue(tags, L"ai.user.id", m_user.GetId());

    AddToMapIfHasValue(tags, L"ai.device.id", m_device.GetId());
    AddToMapIfHasValue(tags, L"ai.device.ip", m_device.GetIp());
    AddToMapIfHasValue(tags, L"ai.device.language", m_device.GetLanguage());
    AddToMapIfHasValue(tags, L"ai.device.locale", m_device.GetLocale());
    AddToMapIfHasValue(tags, L"ai.device.model", m_device.GetModel());
    AddToMapIfHasValue(tags, L"ai.device.network", m_device.GetNetwork());
    AddToMapIfHasValue(tags, L"ai.device.oemName", m_device.GetOemName());
    AddToMapIfHasValue(tags, L"ai.device.os", m_device.GetOs());
    AddToMapIfHasValue(tags, L"ai.device.osVersion", m_device.GetOsVersion());
    AddToMapIfHasValue(tags, L"ai.device.roleInstance", m_device.GetRoleInstance());
    AddToMapIfHasValue(tags, L"ai.device.roleName", m_device.GetRoleName());
    AddToMapIfHasValue(tags, L"ai.device.screenResolution", m_device.GetScreenResolution());
    AddToMapIfHasValue(tags, L"ai.device.type", m_device.GetType());
    AddToMapIfHasValue(tags, L"ai.device.machineName", m_device.GetMachineName());

    AddToMapIfHasValue(tags, L"ai.application.ver", m_app.GetVer());

    AddToMapIfHasValue(tags, L"ai.session.id", m_session.GetId());

    if (!m_parentIds.empty())
    {
        tags[L"ai.operation.parentId"] = m_parentIds.back();
    }
    if (!m_opID.empty())
    {
        tags[L"ai.operation.id"] = m_opID;
    }
    if (!m_opName.empty())
    {
        tags[L"ai.operation.name"] = m_opName;
    }

    AddToMapIfHasValue(tags, L"ai.session.isFirst", m_session.GetIsFirst());
    AddToMapIfHasValue(tags, L"ai.session.isNew", m_session.GetIsNew());

    return RESULT_OK;
}