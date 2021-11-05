// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>
#include "custom_string.hpp"

using namespace ARDUINOJSON_NAMESPACE;

template <typename StringWriter>
static size_t print(StringWriter& sb, const char* s) {
  return sb.write(reinterpret_cast<const uint8_t*>(s), strlen(s));
}

template <typename StringWriter, typename String>
void common_tests(StringWriter& sb, const String& output) {
  SECTION("InitialState") {
    REQUIRE(std::string("") == output);
  }

  SECTION("EmptyString") {
    REQUIRE(0 == print(sb, ""));
    REQUIRE(std::string("") == output);
  }

  SECTION("OneString") {
    REQUIRE(4 == print(sb, "ABCD"));
    REQUIRE(std::string("ABCD") == output);
  }

  SECTION("TwoStrings") {
    REQUIRE(4 == print(sb, "ABCD"));
    REQUIRE(4 == print(sb, "EFGH"));
    REQUIRE(std::string("ABCDEFGH") == output);
  }
}

TEST_CASE("StaticStringWriter") {
  char output[20];
  StaticStringWriter sb(output, sizeof(output));

  common_tests(sb, static_cast<const char*>(output));

  SECTION("OverCapacity") {
    REQUIRE(19 == print(sb, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    REQUIRE(0 == print(sb, "ABC"));
    REQUIRE(std::string("ABCDEFGHIJKLMNOPQRS") == output);
  }
}

TEST_CASE("DynamicStringWriter<std::string>") {
  std::string output;
  DynamicStringWriter<std::string> sb(output);
  common_tests(sb, output);
}

TEST_CASE("DynamicStringWriter<custom_string>") {
  custom_string output;
  DynamicStringWriter<custom_string> sb(output);

  REQUIRE(4 == print(sb, "ABCD"));
  REQUIRE("ABCD" == output);
}

TEST_CASE("IsWriteableString") {
  SECTION("std::string") {
    REQUIRE(IsWriteableString<std::string>::value == true);
  }

  SECTION("custom_string") {
    REQUIRE(IsWriteableString<custom_string>::value == true);
  }

  SECTION("basic_string<wchar_t>") {
    REQUIRE(IsWriteableString<std::basic_string<wchar_t> >::value == false);
  }
}
