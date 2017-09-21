#ifndef TELEMETRYCHANNEL_H
#define TELEMETRYCHANNEL_H

#include "TelemetryClientConfig.h"
#include "TelemetryContext.h"
#include "Contracts/Contracts.h"
#include "Inc/common/Common.h"
#include "Utils/HttpRequest.h"

#ifdef WINAPI_FAMILY_PARTITION
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) // Windows phone or store
#include <Windows.h>
#endif 
#endif

class RequestTracker;
namespace ApplicationInsights
{
    namespace core
    {
        struct TELEMETRYCLIENT_API ITelemetryChannel
        {
            /// <summary>
            /// Enqueues the specified context.
            /// </summary>
            /// <param name="context">The context.</param>
            /// <param name="telemetry">The telemetry.</param>
            virtual void Enqueue(TelemetryContext &context, Domain &telemetry) = 0;

            /// <summary>
            /// Sends this instance.
            /// </summary>
            virtual void SendAsync() = 0;

            virtual void FlushAll() = 0;
        };

        struct TELEMETRYCLIENT_API TelemetryChannelFactory
        {
            static ITelemetryChannel* CreateTelemetryChannel(TelemetryClientConfig &config);
        };

    }
}
#endif