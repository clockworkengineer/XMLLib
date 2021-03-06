//
// Unit Tests: XML
//
// Description:
//
// ================
// Test definitions
// =================
#include "XML_Tests.hpp"
// =======================
// JSON class namespace
// =======================
using namespace XMLLib;
// ==========
// Test cases
// ==========
TEST_CASE("Parse XML with Unicode characters. ", "[XML][Parse][Unicode]")
{
  std::string xmlString;
  SECTION("Japanese characters", "[XML][Parse][[Unicode]")
  {
    xmlString = "<?xml version=\"1.0\" encoding=\"utf-8\"?><config><start_text>転送</start_text></config>\n";
    BufferSource source { xmlString };
    XML xml;
    REQUIRE_NOTHROW(xml.parse(source));
  }
  SECTION("Well-formed XML document including Chinese, Armenian and Cyrillic characters", "[XML][Parse][[Unicode]")
  {
    xmlString = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                "<俄语 լեզու=\"ռուսերեն\">данные</俄语>\n";
    BufferSource source { xmlString };
    XML xml;
    xml.parse(source);
    REQUIRE(xml.root().name() == "俄语");
    REQUIRE(xml.root().getAttributeList().size() == 1);
    REQUIRE(xml.root().getAttribute("լեզու").value.parsed == "ռուսերեն");
    REQUIRE(xml.root().getContents() == "данные");
  }
}