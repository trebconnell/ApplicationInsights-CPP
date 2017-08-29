#ifndef DOMAIN_H
#define DOMAIN_H

#include "../common/Common.h"
#include "../common/JsonWriter.h"
#include "../common/Nullable.h"
#include "../common/Serializer.h"
#include <map>
#include <string>
#include <vector>

namespace ApplicationInsights
{
	namespace core
	{
		class TELEMETRYCLIENT_API Domain : public ISerializable
		{
		private:
			std::wstring m_envelopeName;
			std::wstring m_baseType;
            //Nullable<std::wstring> m_operationID;
            //Nullable<std::wstring> m_parentID;

		public:
			Domain(std::wstring envelopeName, std::wstring dataType);
			virtual ~Domain();

            //void SetOperationID(const std::wstring& operationID) { m_operationID = operationID; }
            //void SetParentID(const std::wstring& parentID) { m_parentID = parentID; }

            //const Nullable<std::wstring>& GetOperationID() const { return m_operationID; }
            //const Nullable<std::wstring>& GetParentID() const { return m_parentID; }
			const std::wstring& GetEnvelopeName() const { return m_envelopeName; }
			const std::wstring& GetBaseType() const { return m_baseType; }
			virtual void Serialize(Serializer& serializer) const;
		};
	}
}
#endif
