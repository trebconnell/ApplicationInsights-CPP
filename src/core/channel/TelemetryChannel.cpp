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
using namespace Concurrency;

const int MAX_BUFFER_SIZE = 100;


class TelemetryChannel final : public ITelemetryChannel
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="TelemetryChannel"/> class.
    /// </summary>
    /// <param name="config">The configuration.</param>
    TelemetryChannel(TelemetryClientConfig &config) : m_config(&config)
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
    virtual ~TelemetryChannel()
    {

    }

    void Enqueue(TelemetryContext &context, Domain &telemetry) override
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
            SendAsync();
        }

        if (context.GetSession().GetIsNew().HasValue() && context.GetSession().GetIsNew().GetValue() == L"True")
        {
            Nullable<std::wstring> strFalse = std::wstring(L"False");
            context.GetSession().SetIsFirst(strFalse);
            context.GetSession().SetIsNew(strFalse);
        }
    }

    void SendAsync() override
    {
        std::vector<std::wstring> tempBuff = m_buffer;

        if (tempBuff.size() != 0)
        {
            auto t = InternalSend(tempBuff);
            m_tasks.push_back(t);
        }
        m_buffer.clear();
    }

    void FlushAll() override
    {
        auto joinTask = when_all(begin(m_tasks), end(m_tasks));
        joinTask.wait();
    }

    Concurrency::task<void> InternalSend(std::vector<std::wstring> &buffer)
    {
        return create_task([buffer]()
        {
            if (buffer.size() > 0)
            {
                std::wstring bufferStr;
                {
                    bufferStr += L"[";
                    for (auto& buf : buffer) {
                        bufferStr += buf + L",";
                    }
                    bufferStr += L"]";
                }

#ifdef CPP_LIB_DEBUG
                std::wstring req = L"REQUEST :\r\n" + bufferStr;
                Utils::WriteDebugLine(req);
#endif
                HttpRequest request(HTTP_REQUEST_METHOD::POST, L"dc.services.visualstudio.com", L"/v2/track", bufferStr);
                request.GetHeaderFields().SetField(L"Content-Type", L"application/json");

                request.Send([](const HttpResponse &response) {
#ifdef CPP_LIB_DEBUG
                    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                    std::wstring wstrResp = converter.from_bytes(response.GetPayload());

                    std::wstring wstrOutput = L"RESPONSE :\r\n" + wstrResp;
                    Utils::WriteDebugLine(wstrOutput);
#endif
                    if (response.GetErrorCode() >= static_cast<int>(HTTP_RESPONSE_CODE::HTTP_SVR_ERROR))
                    {
                        //TODO: log
                    }
#ifdef _DEBUG
#ifdef WINAPI_FAMILY_PARTITION
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) // Windows phone or store
                    SetEvent(hRespRecv);
#endif
#endif
#endif
                });
            }
        });
    }

private:
    int m_channelId;
    int m_seqNum;
    int m_maxBufferSize;
    TelemetryClientConfig *m_config;
    std::vector<std::wstring> m_buffer;
    concurrent_vector<task<void>> m_tasks;
    RequestTracker* m_requestTracker;
    HttpResponse resp;

#ifdef WINAPI_FAMILY_PARTITION
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) // Windows phone or store
    HANDLE hRespRecv;
#endif
#endif
};

ITelemetryChannel* TelemetryChannelFactory::CreateTelemetryChannel(TelemetryClientConfig &config)
{
    return new TelemetryChannel(config);
}