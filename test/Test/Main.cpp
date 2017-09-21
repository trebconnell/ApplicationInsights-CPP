#include "Inc/TelemetryClient.h"
#include <chrono>
#include "Inc/common/Utils.h"

using namespace ApplicationInsights::core;

std::wstring ToWStr(const wchar_t* str)
{
    return str;
}

std::wstring GetTimeStr()
{
    //auto time = std::chrono::system_clock::now().time_since_epoch();
    //auto timeMsTotal = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();

    //auto getTimePart = [&](unsigned mult)
    //{
    //    auto timePart = timeMsTotal % mult;
    //    timeMsTotal /= mult;
    //    return (unsigned)timePart;
    //};
    //auto timeMs = getTimePart(1000000);
    //auto timeS = getTimePart(60);
    //auto timeM = getTimePart(60);
    //auto timeH = getTimePart(12);
    //// TODO: won't work with months not having 31 days.
    //auto timeD = getTimePart(31);

    //wchar_t timeBuf[256];
    ////                                     DD   .HH   :MM   :SS   .MMMMMM
    //swprintf_s(timeBuf, L"%02u.%02u:%02u:%02u.%-6u", timeD, timeH, timeM, timeS, timeMs);

    return Utils::GetCurrentDateTime();
}

int main()
{
    std::wstring ikey = L"169b3c1b-3020-4c58-acab-83e36df4c831";

    TelemetryClient* client = new TelemetryClient(ikey);

    for (auto i = 0u; i < 100; ++i)
    {
        {
            //Operation op;
            //op.SetId(ToWStr(L"op") + std::to_wstring(i));
            //op.SetName(ToWStr(L"opname"));

            //client.Track(op);
            client->GetContext()->SetOperationName(ToWStr(L"opname"));
            client->GetContext()->SetOperationID(ToWStr(L"op") + std::to_wstring(i));
        }

        auto reqTime = GetTimeStr();
        auto reqID = ToWStr(L"req") + std::to_wstring(i / 2);
        client->GetContext()->PushParentID(reqID);

        {
            EventData eventData;
            eventData.SetName(ToWStr(L"TestOpEvent"));
            client->Track(eventData);
            client->Flush();
        }
    }

    //    {
    //        wchar_t msg[256];
    //        swprintf_s(msg, L"Fun trace message test %u", i);

    //        MessageData traceData;
    //        traceData.SetMessage(ToWStr(msg));
    //        traceData.SetSeverityLevel(i == 3 ? SeverityLevel::Warning : SeverityLevel::Information);
    //        client.Track(traceData);
    //    }

    //    {
    //        wchar_t msg[256];
    //        swprintf_s(msg, L"Fun generic trace msg");

    //        MessageData traceData;
    //        traceData.SetMessage(ToWStr(msg));
    //        traceData.SetSeverityLevel(i == 4 ? SeverityLevel::Error : SeverityLevel::Information);
    //        client.Track(traceData);
    //    }

    //    {
    //        RequestData request;
    //        request.SetName(ToWStr(L"TestOpRequest"));
    //        request.SetId(reqID);
    //        request.SetSuccess(i == 4 ? false : true);
    //        request.SetDuration(L"00.00:00:00.300000");
    //        request.SetResponseCode(L"100");
    //        request.SetStartTime(reqTime);
    //        client.GetContext()->PopParentID();
    //        client.Track(request);
    //    }

    //    {
    //        wchar_t msg[256];
    //        swprintf_s(msg, L"Trace outside request");

    //        MessageData traceData;
    //        traceData.SetMessage(ToWStr(msg));
    //        traceData.SetSeverityLevel(SeverityLevel::Information);
    //        client.Track(traceData);
    //    }

    //    //client.SetOperationID(L"");
    //}

    //{
    //    wchar_t msg[256];
    //    swprintf_s(msg, L"Trace outside everything");

    //    MessageData traceData;
    //    traceData.SetMessage(ToWStr(msg));
    //    traceData.SetSeverityLevel(SeverityLevel::Information);
    //    client.Track(traceData);
    //}

    
    client->Flush();

    EventData eventData;
    eventData.SetName(ToWStr(L"TestOpEventFinal"));
    client->Track(eventData);
    client->Flush();
    delete client;
}
