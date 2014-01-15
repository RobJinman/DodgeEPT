#include <sstream>
#include <dodge/xml/xml.hpp>
#include "MapSettings.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MapSettings::loadFromXml
//===========================================
void MapSettings::loadFromXml(XmlNode node) {
   // <settings>
   //   <boundary>
   //     <Range>
   //       <pos>
   //         <Vec2f/>
   //       </pos>
   //       <size>
   //         <Vec2f/>
   //       </size>
   //     </Range>
   //   </boundary>
   //   <numSegments>
   //     <Vec2i/>
   //   </numSegments>
   //   <segmentSize>
   //     <Vec2f/>
   //   </segmentSize>
   //   <segmentsDir/>
   // </settings>

   XML_NODE_CHECK(node, settings);

   node = node.firstChild();
   XML_NODE_CHECK(node, boundary);

   boundary = Range(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(node, numSegments);
   numSegments = Vec2i(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(node, segmentSize);
   segmentSize = Vec2f(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(node, segmentsDir);
   segmentsDir = node.getString();
}

//===========================================
// MapSettings::toXml
//===========================================
XmlDocument MapSettings::toXml() const {
   XmlDocument doc;

   stringstream ss;

   XmlNode settings = doc.addNode("settings");
      XmlNode boundary = settings.addNode("boundary");
         XmlNode range = boundary.addNode("Range");
            XmlNode pos = range.addNode("pos");
               XmlNode brp_vec2f = pos.addNode("Vec2f");
                  ss.str("");
                  ss << this->boundary.getPosition().x;

                  brp_vec2f.addAttribute("x", ss.str().data());

                  ss.str("");
                  ss << this->boundary.getPosition().y;

                  brp_vec2f.addAttribute("y", ss.str().data());
            XmlNode size = range.addNode("size");
               XmlNode brs_vec2f = size.addNode("Vec2f");
                  ss.str("");
                  ss << this->boundary.getSize().x;

                  brs_vec2f.addAttribute("x", ss.str().data());

                  ss.str("");
                  ss << this->boundary.getSize().y;

                  brs_vec2f.addAttribute("y", ss.str().data());
      XmlNode numSegments = settings.addNode("numSegments");
         XmlNode ns_vec2i = numSegments.addNode("Vec2i");
            ss.str("");
            ss << this->numSegments.x;

            ns_vec2i.addAttribute("x", ss.str().data());

            ss.str("");
            ss << this->numSegments.y;

            ns_vec2i.addAttribute("y", ss.str().data());
      XmlNode segmentSize = settings.addNode("segmentSize");
         XmlNode ss_vec2f = segmentSize.addNode("Vec2f");
            ss.str("");
            ss << this->segmentSize.x;

            ss_vec2f.addAttribute("x", ss.str().data());

            ss.str("");
            ss << this->segmentSize.y;

            ss_vec2f.addAttribute("y", ss.str().data());
      XmlNode segmentsDir = settings.addNode("segmentsDir");
         segmentsDir.setValue("data/xml/0"); // TODO

   return doc;
}