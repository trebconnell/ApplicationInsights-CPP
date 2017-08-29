#include "../../src/core/TelemetryClient.h"
#include <chrono>
#pragma comment(lib, "G:/devl/appi/x64/Debug/AppInsights.lib")

using namespace ApplicationInsights::core;

std::wstring ToWStr(const wchar_t* str)
{
    return str;
}

std::wstring GetTimeStr()
{
    auto time = std::chrono::system_clock::now().time_since_epoch();
    auto timeMsTotal = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();

    auto getTimePart = [&](unsigned mult)
    {
        auto timePart = timeMsTotal % mult;
        timeMsTotal /= mult;
        return (unsigned)timePart;
    };
    auto timeMs = getTimePart(1000000);
    auto timeS = getTimePart(60);
    auto timeM = getTimePart(60);
    auto timeH = getTimePart(12);
    // TODO: won't work with months not having 31 days.
    auto timeD = getTimePart(31);

    wchar_t timeBuf[256];
    //                                     DD   .HH   :MM   :SS   .MMMMMM
    swprintf_s(timeBuf, L"%0u.2.%0u.2:%0u.2:%0u.2.%u.6", timeD, timeH, timeM, timeS, timeMs);

    return timeBuf;
}

int main()
{
    std::wstring ikey = L"f91ebe0b-17ca-47c4-8397-140bb6680399";
    TelemetryClient client(ikey);

    for (auto i = 0u; i < 5; ++i)
    {
        //{
        //    Operation op;
        //    op.SetId(ToWStr(L"op") + std::to_wstring(i));
        //    op.SetName(ToWStr(L"opname"));

        //    client.Track(op);
        //    client.SetOperationID(op.GetId());
        //}

        auto reqTime = GetTimeStr();
        auto reqID = ToWStr(L"req") + std::to_wstring(i / 2);
        //client.PushParentID(reqID);

        //{
        //    EventData eventData;
        //    eventData.SetName(ToWStr(L"TestOpEvent"));
        //    client.Track(eventData);
        //}

        {
            wchar_t msg[256];
            swprintf_s(msg, L"Fun trace message test %u", i);

            MessageData traceData;
            traceData.SetMessage(ToWStr(msg));
            traceData.SetSeverityLevel(i == 3 ? SeverityLevel::Warning : SeverityLevel::Information);
            client.Track(traceData);
        }

        {
            wchar_t msg[256];
            swprintf_s(msg, L"Fun generic trace msg");

            MessageData traceData;
            traceData.SetMessage(ToWStr(msg));
            traceData.SetSeverityLevel(i == 4 ? SeverityLevel::Error : SeverityLevel::Information);
            client.Track(traceData);
        }

        {
            RequestData request;
            request.SetName(ToWStr(L"TestOpRequest"));
            request.SetId(reqID);
            request.SetSuccess(i == 4 ? false : true);
            request.SetDuration(L"00.00:00:00.300000");
            request.SetResponseCode(L"100");
            request.SetStartTime(reqTime);
            client.PopParentID();
            client.Track(request);
        }

        {
            wchar_t msg[256];
            swprintf_s(msg, L"Trace outside request");

            MessageData traceData;
            traceData.SetMessage(ToWStr(msg));
            traceData.SetSeverityLevel(SeverityLevel::Information);
            client.Track(traceData);
        }

        //client.SetOperationID(L"");
    }

    {
        wchar_t msg[256];
        swprintf_s(msg, L"Trace outside everything");

        MessageData traceData;
        traceData.SetMessage(ToWStr(msg));
        traceData.SetSeverityLevel(SeverityLevel::Information);
        client.Track(traceData);
    }

    client.Flush();
}
