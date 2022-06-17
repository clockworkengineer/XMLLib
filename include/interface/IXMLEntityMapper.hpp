#pragma once
//
// C++ STL
//
#include <set>
#include <map>
//
// ISource interface
//
#include "ISource.hpp"
//
// XML types
//
#include "XML_Types.hpp"
// =========
// NAMESPACE
// =========
namespace XMLLib
{

    // ===========================
    // XML Entity mapper interface
    // ===========================
    class IXMLEntityMapper
    {
    public:
        // Contructors/destructors
        IXMLEntityMapper() = default;
        IXMLEntityMapper(const IXMLEntityMapper& other) = delete;
        IXMLEntityMapper& operator=(const IXMLEntityMapper& other) = delete;
        IXMLEntityMapper(IXMLEntityMapper&& other) = delete;
        IXMLEntityMapper& operator=(IXMLEntityMapper&& other) = delete;
        virtual ~IXMLEntityMapper() = default;
        virtual void add(const std::string &entityName, const XMLEntityMapping &entityMapping) = 0;
        virtual XMLEntityMapping &get(const std::string &entityName) = 0;
        virtual void remove(const std::string &entityName) = 0;
        [[nodiscard]] virtual bool isPresent(const std::string &entityName) const = 0;
        virtual std::map<std::string, XMLEntityMapping> &getList() = 0;
        virtual XMLValue map(const XMLValue &entityReference) = 0;
        [[nodiscard]] virtual std::string translate(const std::string &toTranslate, char type = '%') const = 0;
        virtual void recursive(const std::string &entityName, ISource::Char type, std::set<std::string> names = {}) = 0;
    };
    //
} // namespace XMLLib