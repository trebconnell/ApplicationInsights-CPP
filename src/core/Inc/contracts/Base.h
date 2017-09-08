#ifndef BASE_H
#define BASE_H

#include "../Inc/common/Common.h"
#include "../Inc/common/JsonWriter.h"
#include "../Inc/common/Nullable.h"
#include "../Inc/common/Serializer.h"
#include <map>
#include <string>
#include <vector>

namespace ApplicationInsights
{
	namespace core
	{
		class TELEMETRYCLIENT_API Base : public ISerializable
		{
		private:
			std::wstring m_baseType;

		public:
			Base();
			virtual ~Base();

			const std::wstring& GetBaseType() const { return m_baseType; }
			void SetBaseType(const std::wstring& value) { m_baseType = value; }

			virtual void Serialize(Serializer& serializer) const;
		};
	}
}
#endif
