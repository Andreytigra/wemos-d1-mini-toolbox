#include <Arduino.h>
#include <LittleFS.h>

String listContorollers() {
  String json = "{ \"controllers\": [";

  Dir root = LittleFS.openDir("/");

  bool first = true;

  while (root.next()) {
    if (!first) json += ",";
    first = false;
    File file = root.openFile("r");
    json += "\"";
    json += root.fileName();
    json += "\"";
  }

  json += "] }";
  return json;
}


