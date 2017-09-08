#ifndef LOCATION_H
#define LOCATION_H

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
		class TELEMETRYCLIENT_API Location : public ISerializable
		{
		private:
			Nullable<std::wstring> m_ip;

		public:
			Location();
			virtual ~Location();

			const Nullable<std::wstring>& GetIp() const { return m_ip; }
			void SetIp(const Nullable<std::wstring>& value) { m_ip = value; }

			virtual void Serialize(Serializer& serializer) const;
		};
	}
}
#endif
