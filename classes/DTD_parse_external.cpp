//
// Class: DTD
//
// Description: Parse external XML DTD.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "XML.hpp"
#include "DTD.hpp"
#include "XML_sources.hpp"
#include "XML_destinations.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =========
// NAMESPACE
// =========
namespace XMLLib
{
    // ===========================
    // PRIVATE TYPES AND CONSTANTS
    // ===========================
    // ==========================
    // PUBLIC TYPES AND CONSTANTS
    // ==========================
    // ========================
    // PRIVATE STATIC VARIABLES
    // ========================
    // =======================
    // PUBLIC STATIC VARIABLES
    // =======================
    // ===============
    // PRIVATE METHODS
    // ===============
    /// <summary>
    /// Parse conditional DTD (recusively if necessary).
    /// </summary>
    /// <param name="dtdSource">DTD source stream.</param>
    /// <param name="includeOn">If set to false then enclosing conditionals treated as ignored.</param>
    void DTD::parseConditional(ISource &dtdSource, bool includeOn)
    {
        dtdSource.ignoreWS();
        std::string conditionalValue = "";
        if (includeOn)
        {
            if (dtdSource.current() == '%')
            {
                XMLValue entityRerence = parseEntityReference(dtdSource);
                dtdSource.ignoreWS();
                m_entityMapper.map(entityRerence);
                conditionalValue = entityRerence.parsed;
            }
            else if (dtdSource.match(U"INCLUDE"))
            {
                conditionalValue = "INCLUDE";
            }
            else if (dtdSource.match(U"IGNORE"))
            {
                conditionalValue = "IGNORE";
            }
        }
        else
        {
            conditionalValue = "IGNORE";
        }
        if (conditionalValue == "INCLUDE")
        {
            dtdSource.ignoreWS();
            if (dtdSource.current() != '[')
            {
                throw SyntaxError(dtdSource, "Missing opening '[' from conditional.");
            }
            dtdSource.next();
            dtdSource.ignoreWS();
            std::string conditionalDTD;
            while (dtdSource.more() && !dtdSource.match(U"]]"))
            {
                if (dtdSource.match(U"<!["))
                {
                    parseConditional(dtdSource);
                }
                else
                {
                    conditionalDTD += dtdSource.current_to_bytes();
                    dtdSource.next();
                }
            }
            BufferSource conditionalDTDSource(conditionalDTD);
            parseExternalContent(conditionalDTDSource);
        }
        else if (conditionalValue == "IGNORE")
        {
            while (dtdSource.more() && !dtdSource.match(U"]]"))
            {
                if (dtdSource.match(U"<!["))
                {
                    parseConditional(dtdSource, false);
                }
                else
                {
                    dtdSource.next();
                }
            }
        }
        else
        {
            throw SyntaxError(dtdSource, "Conditional value not INCLUDE or IGNORE.");
        }
        if (dtdSource.current() != '>')
        {
            throw SyntaxError(dtdSource, "Missing '>' terminator.");
        }
        dtdSource.next();
        dtdSource.ignoreWS();
    }
    /// <summary>
    /// Parse external DTD.
    /// </summary>
    /// <param name="dtdSource">DTD source stream.</param>
    void DTD::parseExternalContent(ISource &dtdSource)
    {
        while (dtdSource.more())
        {
            if (dtdSource.match(U"<!ENTITY"))
            {
                BufferSource dtdTranslatedSource(m_entityMapper.translate(parseTagBody(dtdSource)));
                parseEntity(dtdTranslatedSource);
            }
            else if (dtdSource.match(U"<!ELEMENT"))
            {
                BufferSource dtdTranslatedSource(m_entityMapper.translate(parseTagBody(dtdSource)));
                parseElement(dtdTranslatedSource);
            }
            else if (dtdSource.match(U"<!ATTLIST"))
            {
                BufferSource dtdTranslatedSource(m_entityMapper.translate(parseTagBody(dtdSource)));
                parseAttributeList(dtdTranslatedSource);
            }
            else if (dtdSource.match(U"<!NOTATION"))
            {
                    BufferSource dtdTranslatedSource(m_entityMapper.translate(parseTagBody(dtdSource)));
                parseNotation(dtdTranslatedSource);
            }
            else if (dtdSource.match(U"<!--"))
            {
                parseComment(dtdSource);
            }
            else if (dtdSource.current() == '%')
            {
                parseParameterEntityReference(dtdSource);
                continue;
            }
            else if (dtdSource.match(U"<!["))
            {
                parseConditional(dtdSource);
                continue;
            }
            else
            {
                throw SyntaxError(dtdSource, "Invalid DTD tag.");
            }
            if (dtdSource.current() != '>')
            {
                throw SyntaxError(dtdSource, "Missing '>' terminator.");
            }
            dtdSource.next();
            dtdSource.ignoreWS();
        }
    }
    /// <summary>
    /// Parse externally defined DTD into XMLNodeDTD.
    /// </summary>
    void DTD::parseExternalRefenceContent()
    {
        if (m_external.type == "SYSTEM")
        {
            FileSource dtdFile(m_external.systemID);
            parseExternalContent(dtdFile);
        }
        else if (m_external.type == "PUBLIC")
        {
            // Public external DTD currently not supported (Use system id ?)
        }
    }
    /// <summary>
    /// Parse an external reference.
    /// </summary>
    /// <param name="dtdSource">DTD source stream.</param>
    /// <returns>External reference.</returns>
    XMLExternalReference DTD::parseExternalReference(ISource &dtdSource)
    {
        XMLExternalReference result;
        if (dtdSource.match(U"SYSTEM"))
        {
            dtdSource.ignoreWS();
            result.type = "SYSTEM";
            result.systemID = parseValue(dtdSource, m_entityMapper).parsed;
        }
        else if (dtdSource.match(U"PUBLIC"))
        {
            dtdSource.ignoreWS();
            result.type = "PUBLIC";
            result.publicID = parseValue(dtdSource, m_entityMapper).parsed;
            result.systemID = parseValue(dtdSource, m_entityMapper).parsed;
        }
        else
        {
            throw SyntaxError(dtdSource, "Invalid external DTD specifier.");
        }
        return (result);
    }
    /// <summary>
    /// Parse externally defined DTD.
    /// </summary>
    /// <param name="dtdSource">DTD source stream.</param>
    void DTD::parseExternal(ISource & /*dtdSource*/)
    {
        parseExternalRefenceContent();
    }
} // namespace XMLLib
