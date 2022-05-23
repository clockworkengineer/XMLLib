#pragma once
//
// C++ STL
//
#include <string>
#include <utility>
// =========
// NAMESPACE
// =========
namespace XMLLib
{
    //
    // XML value
    //
    struct XMLValue
    {
    public:
        XMLValue(const std::string &unparsedValue, const std::string &parsedValue) : unparsed(unparsedValue), parsed(parsedValue)
        {
        }
        [[nodiscard]] bool isEntityReference() const
        {
            return (unparsed[0] == '&' && unparsed[1] != '#' && unparsed.back() == ';');
        }
        [[nodiscard]] bool isCharacterReference() const
        {
            return (unparsed[0] == '&' && unparsed[1] == '#' && unparsed.back() == ';');
        }
        std::string unparsed;
        std::string parsed;
    };
    //
    // XML Attribute
    //
    struct XMLAttribute
    {
    public:
        XMLAttribute(const std::string &attributeName, const XMLValue &attributeValue) : name(attributeName), value(attributeValue)
        {
        }
        std::string name;
        XMLValue value;
    };
    //
    // XML External reference
    //
    struct XMLExternalReference
    {
    public:
        XMLExternalReference(const std::string &refType, const std::string &refSystemID, const std::string &refPublicID) : type(refType), systemID(refSystemID), publicID(refPublicID)
        {
        }
        std::string type;
        std::string systemID;
        std::string publicID;
    };
    //
    // XML Entity mapping
    //
    struct XMLEntityMapping
    {
    public:
        XMLEntityMapping(const std::string &mappingInternal, const XMLExternalReference &mappingExternal, std::string mappingNotation) : internal(mappingInternal), external(mappingExternal), notation(mappingNotation)
        {
        }
        std::string internal;
        XMLExternalReference external{"", "", ""};
        std::string notation;
    };
} // namespace XMLLib
