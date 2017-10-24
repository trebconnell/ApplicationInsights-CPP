#ifndef BASETELEMETRYCONTEXT_H
#define BASETELEMETRYCONTEXT_H

#include <string>
#include "TelemetryClientConfig.h"
#include "Contracts/Contracts.h"
#include "Common/Common.h"

namespace ApplicationInsights
{
    namespace core
    {
        class TELEMETRYCLIENT_API BaseTelemetryContext
        {
        public:
            /// <summary>
            /// Initializes a new instance of the <see cref="BaseTelemetryContext"/> class.
            /// </summary>
            /// <param name="iKey">The i key.</param>
            BaseTelemetryContext(const std::wstring& iKey);

            /// <summary>
            /// Finalizes an instance of the <see cref="BaseTelemetryContext"/> class.
            /// </summary>
            virtual ~BaseTelemetryContext();

            /// <summary>
            /// Initializes the context.
            /// </summary>
            virtual void InitContext();

            /// <summary>
            /// Gets the user.
            /// </summary>
            /// <returns>the user context</returns>
            User& GetUser() { return m_user; }
            const User& GetUser() const { return m_user; }

            /// <summary>
            /// Gets the device.
            /// </summary>
            /// <returns>tne device context</returns>
            Device& GetDevice() { return m_device; }
            const Device& GetDevice() const { return m_device; }

            /// <summary>
            /// Gets the application.
            /// </summary>
            /// <returns>the application context</returns>
            Application& GetApplication() { return m_app; }
            const Application& GetApplication() const { return m_app; }

            /// <summary>
            /// Gets the session.
            /// </summary>
            /// <returns>the session context</returns>
            Session& GetSession() { return m_session; }
            const Session& GetSession() const { return m_session; }

            void SetOperationName(const std::wstring& opName);
            void SetOperationID(const std::wstring& opID);
            void PushParentID(const std::wstring& parentID);
            void PopParentID();

            /// <summary>
            /// Renews the session.
            /// </summary>
            void RenewSession();

            /// <summary>
            /// Gets the context tags.
            /// </summary>
            /// <param name="tags">The tags.</param>
            /// <returns>RESULT_OK if succedded</returns>
            RESULT GetContextTags(wstring_wstring_map& buffer);

        protected:
            /// <summary>
            /// Initializes the user context.
            /// </summary>
            virtual void InitUser();

            /// <summary>
            /// Initializes the device context.
            /// </summary>
            virtual void InitDevice();

            /// <summary>
            /// Initializes the application.
            /// </summary>
            virtual void InitApplication();

            /// <summary>
            /// Initializes the session.
            /// </summary>
            virtual void InitSession();

            User m_user;
            Device m_device;
            Application m_app;
            Session m_session;
            std::wstring m_iKey;

            std::wstring m_opName;
            std::wstring m_opID;
            std::vector<std::wstring> m_parentIds;
        };
    }
}
#endif
