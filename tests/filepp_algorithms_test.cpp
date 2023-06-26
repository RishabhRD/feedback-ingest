#include "data_source/filepp/algorithms.hpp"
#include <doctest/doctest.h>

TEST_CASE("Rightly convert filepp conversation to schema") {
  std::string const content =
      R"([
  {
    "app_version": "4.18.0",
    "conversation": [
      {
        "User": "Can you help me with adding post to story button"
      },
      {
        "User": "Now, I can't see it. Its become hard to use."
      },
      {
        "Support": "You don't need help sir!"
      },
      {
        "Support": "It's not a bug, it's a feature!!"
      }
    ]
  },
  {
    "app_version": "4.19.0",
    "conversation": [
      {
        "User": "Hey! Add to story button is not even visible now"
      },
      {
        "User": "Why is that?"
      },
      {
        "Support": "This is a new feature. Hope you enjoy it."
      }
    ]
  }
])";

  auto const schema = rd::filepp::to_schema(123, 321, content);

  REQUIRE_EQ(schema.size(), 2);

  REQUIRE_EQ(schema[0].source_id, 123);
  REQUIRE_EQ(schema[0].tenant_id, 321);
  REQUIRE_EQ(schema[0].entry.metadata.app_version, "4.18.0");
  REQUIRE_EQ(schema[0].entry.metadata.location, tl::nullopt);
  REQUIRE_EQ(schema[0].entry.metadata.impressions, tl::nullopt);
  REQUIRE_EQ(schema[0].entry.metadata.rating, tl::nullopt);
  REQUIRE_EQ(schema[0].entry.feedback.index(), 0);
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback).size(), 4);
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[0].user.index(), 0);
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[1].user.index(), 0);
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[2].user.index(), 1);
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[3].user.index(), 1);
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[0].message,
             "Can you help me with adding post to story button");
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[1].message,
             "Now, I can't see it. Its become hard to use.");
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[2].message,
             "You don't need help sir!");
  REQUIRE_EQ(std::get<0>(schema[0].entry.feedback)[3].message,
             "It's not a bug, it's a feature!!");

  REQUIRE_EQ(schema[1].source_id, 123);
  REQUIRE_EQ(schema[1].tenant_id, 321);
  REQUIRE_EQ(schema[1].entry.metadata.app_version, "4.19.0");
  REQUIRE_EQ(schema[1].entry.metadata.location, tl::nullopt);
  REQUIRE_EQ(schema[1].entry.metadata.impressions, tl::nullopt);
  REQUIRE_EQ(schema[1].entry.metadata.rating, tl::nullopt);
  REQUIRE_EQ(schema[1].entry.feedback.index(), 0);
  REQUIRE_EQ(std::get<0>(schema[1].entry.feedback).size(), 3);
  REQUIRE_EQ(std::get<0>(schema[1].entry.feedback)[0].user.index(), 0);
  REQUIRE_EQ(std::get<0>(schema[1].entry.feedback)[1].user.index(), 0);
  REQUIRE_EQ(std::get<0>(schema[1].entry.feedback)[2].user.index(), 1);
  REQUIRE_EQ(std::get<0>(schema[1].entry.feedback)[0].message,
             "Hey! Add to story button is not even visible now");
  REQUIRE_EQ(std::get<0>(schema[1].entry.feedback)[1].message, "Why is that?");
  REQUIRE_EQ(std::get<0>(schema[1].entry.feedback)[2].message,
             "This is a new feature. Hope you enjoy it.");
}
