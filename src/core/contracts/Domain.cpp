#include "stdafx.h"
#include "Domain.h"

using namespace ApplicationInsights::core;

Domain::Domain(std::wstring envelopeName, std::wstring baseType) :
    m_envelopeName(envelopeName),
    m_baseType(baseType)
{
}

Domain::~Domain()
{
}

void Domain::Serialize(Serializer& serializer) const
{
    //if (m_operationID.HasValue())
    //{
    //    serializer.WritePropertyName(L"operation_Id");
    //    serializer.WriteStringValue(m_operationID.GetValue());
    //}

    //if (m_parentID.HasValue())
    //{
    //    serializer.WritePropertyName(L"operation_parentId");
    //    serializer.WriteStringValue(m_parentID.GetValue());
    //}
}

