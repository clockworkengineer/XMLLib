#pragma once
//
// C++ STL
//
#include <string>
#include <set>
//
// XML DTD
//
#include "DTD.hpp"
#include "DTD_Element.hpp"
#include "DTD_Parsed.hpp"
//
// XML Validator interface
//
#include "IXMLValidator.hpp"
// =========
// NAMESPACE
// =========
namespace XMLLib
{
    // ================
    // CLASS DEFINITION
    // ================
    class XML_Validator : public IXMLValidator
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ============
        // CONSTRUCTORS
        // ============
        explicit XML_Validator(DTDParsed &dtdParsed) : m_dtdParsed(dtdParsed) {}
        // ==========
        // DESTRUCTOR
        // ==========
        // ==============
        // PUBLIC METHODS
        // ==============
        void validate(XMLNodeElement &prolog) override;
        // ================
        // PUBLIC VARIABLES
        // ================
    private:
        // ===========================
        // PRIVATE TYPES AND CONSTANTS
        // ===========================
        // ===========================================
        // DISABLED CONSTRUCTORS/DESTRUCTORS/OPERATORS
        // ===========================================
        // ===============
        // PRIVATE METHODS
        // ===============
        void elementError(const XMLNodeElement &xNodeElement, const std::string &error);
        void checkAttributes(XMLNode &xmlNode);
        void checkContentSpecification(XMLNode &xmlNode);
        void checkElement(XMLNode &xmlNode);
        void checkElements(XMLNode &xmlNode);
        bool checkIsNMTOKENOK(const std::string &nmTokenValue);
        bool checkIsIDOK(const std::string &idValue);
        void checkAttributeValue(XMLNode &xmlNode, const DTDAttribute &attribute);
        void checkAttributeType(XMLNode &xmlNode, const DTDAttribute &attribute);
        bool checkIsPCDATA(const XMLNode &xmlNode);
        bool checkIsEMPTY(const XMLNode &xmlNode);
        void checkAgainstDTD(XMLNodeElement &prolog);
        // =================
        // PRIVATE VARIABLES
        // =================
        std::set<std::string> m_assignedIDValues;
        std::set<std::string> m_assignedIDREFValues;
        long m_lineNumber = 1;
        DTDParsed &m_dtdParsed;
    };
} // namespace XMLLib
