#ifndef SESSION_H
#define SESSION_H

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
		class TELEMETRYCLIENT_API Session : public ISerializable
		{
		private:
			Nullable<std::wstring> m_id;
			Nullable<std::wstring> m_isFirst;
			Nullable<std::wstring> m_isNew;

		public:
			Session();
			virtual ~Session();

			const Nullable<std::wstring>& GetId();
			void SetId(const Nullable<std::wstring>& value);

			const Nullable<std::wstring>& GetIsFirst();
			void SetIsFirst(const Nullable<std::wstring>& value);

			const Nullable<std::wstring>& GetIsNew();
			void SetIsNew(const Nullable<std::wstring>& value);

			virtual void Serialize(Serializer& serializer) const;
		};
	}
}
#endif
