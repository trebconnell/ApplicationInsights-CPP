#include "stdafx.h"
#include "Inc/channel/TelemetryChannel.h"
#include "Inc/TelemetryContext.h"
#include "Inc/Contracts/Contracts.h"
#include "Inc/common/Utils.h"
#include "Inc/common/StringWriter.h"
#include <stdlib.h> 
#include <time.h>
#include <locale>
#include <codecvt>

using namespace ApplicationInsights::core;

const int MAX_BUFFER_SIZE = 100;

/// <summary>
/// Initializes a new instance of the <see cref="TelemetryChannel"/> class.
/// </summary>
/// <param name="config">The configuration.</param>
TelemetryChannel::TelemetryChannel(TelemetryClientConfig &config)
    : m_config(&config)
{
    srand((int)time(0));
    m_channelId = rand();
    m_seqNum = 0;
    m_maxBufferSize = MAX_BUFFER_SIZE;

#ifdef WINAPI_FAMILY_PARTITION
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) // Windows phone or store
    hRespRecv = CreateEventEx(nullptr, L"RecvResp", 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
#endif
#endif
}

/// <summary>
/// Finalizes an instance of the <see cref="TelemetryChannel"/> class.
/// </summary>
TelemetryChannel::~TelemetryChannel()
{

}

/// <summary>
/// Enqueues the specified context.
/// </summary>
/// <param name="context">The context.</param>
/// <param name="telemetry">The telemetry.</param>
void TelemetryChannel::Enqueue(TelemetryContext &context, Domain &telemetry)
{
    Data data;
    data.SetBaseData(telemetry);
    data.SetBaseType(telemetry.GetBaseType());

    std::wstring buffer;
    StringWriter content(&buffer);
    JsonWriter json(content);

    Envelope envelope;
    envelope.SetData(data);
    envelope.SetIKey(m_config->GetIKey());
    envelope.SetTime(Utils::GetCurrentDateTime());
    envelope.SetName(telemetry.GetEnvelopeName());
    envelope.SetSeq(std::to_wstring(m_channelId) + L":" + std::to_wstring(m_seqNum++));

    wstring_wstring_map tags;
    context.GetContextTags(tags);
    envelope.SetTags(tags);

    json.WriteObjectValue(&envelope);
    m_buffer.push_back(content.ToString());

    if ((int)m_buffer.size() >= m_maxBufferSize)
    {
        Send();
    }

    if (context.GetSession().GetIsNew().HasValue() && context.GetSession().GetIsNew().GetValue() == L"True")
    {
        Nullable<std::wstring> strFalse = std::wstring(L"False");
        context.GetSession().SetIsFirst(strFalse);
        context.GetSession().SetIsNew(strFalse);
    }
}

/// <summary>
/// Sends this instance.
/// </summary>
void TelemetryChannel::Send()
{
    if (m_buffer.size() > 0)
    {
        std::wstring buffer;

        buffer += L"[";
        for (auto &buf : m_buffer) {
            buffer += buf + L",";
        }
        buffer += L"]";

        m_buffer.clear();

#ifdef CPP_LIB_DEBUG
        std::wstring req = L"REQUEST :\r\n" + buffer;
        Utils::WriteDebugLine(req);
#endif

        std::vector<std::wstring> in_progress_buffer(m_buffer);

        HttpRequest request(HTTP_REQUEST_METHOD::POST, L"dc.services.visualstudio.com", L"/v2/track", buffer);
        request.GetHeaderFields().SetField(L"Content-Type", L"application/json");
        request.Send([this, in_progress_buffer](const HttpResponse &response) {
#ifdef CPP_LIB_DEBUG
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring wstrResp = converter.from_bytes(response.GetPayload());

            std::wstring wstrOutput = L"RESPONSE :\r\n" + wstrResp;
            Utils::WriteDebugLine(wstrOutput);
#endif

            if (response.GetErrorCode() >= static_cast<int>(HTTP_RESPONSE_CODE::HTTP_SVR_ERROR))
            {
                // reload the buffer if there was some sort of server-side issue
                m_buffer.insert(m_buffer.begin(), in_progress_buffer.begin(), in_progress_buffer.end());
            }

#ifdef _DEBUG
            resp = response;
#ifdef WINAPI_FAMILY_PARTITION
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) // Windows phone or store
            SetEvent(hRespRecv);
#endif
#endif
#endif
        });
    }
}
