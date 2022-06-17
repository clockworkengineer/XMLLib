#pragma once
//
// XML types
//
#include "XML_Types.hpp"
// =========
// NAMESPACE
// =========
namespace XMLLib
{
    // =======================
    // XML validator interface
    // =======================
    class IXMLValidator
    {
    public:
        // Contructors/destructors
        IXMLValidator() = default;
        IXMLValidator(const IXMLValidator& other) = delete;
        IXMLValidator& operator=(const IXMLValidator& other) = delete;
        IXMLValidator(IXMLValidator&& other) = delete;
        IXMLValidator& operator=(IXMLValidator&& other) = delete;
        virtual ~IXMLValidator() = default;
        virtual void validate(XMLNodeProlog &prolog) = 0;
    };

} // namespace XMLLib