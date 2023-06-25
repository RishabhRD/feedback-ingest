#include "discourse_data_source.hpp"
#include <doctest/doctest.h>
#include <iostream>
#include <range/v3/algorithm/sort.hpp>

using namespace std::chrono_literals;

TEST_CASE("Creates correct url to fetch posts between") {
  auto begin_date = std::chrono::sys_days{2021y / 1 / 1};
  auto end_date = std::chrono::sys_days{2021y / 1 / 30};
  std::string expected_url =
      "https://meta.discourse.org/"
      "search.json?q=after%3a2021-01-01+before%3a2021-01-30";

  REQUIRE_EQ(rd::discourse::make_url_for_posts_between(begin_date, end_date),
             expected_url);
}

TEST_CASE("Creates correct url to fetch posts of topic_id") {
  int topic_id = 12341;
  std::vector<int> ids{123, 124, 125, 126};
  std::string expected = "https://meta.discourse.org/t/12341/"
                         "posts.json?post_ids%5B%5D=123,124,125,126";

  REQUIRE_EQ(rd::discourse::make_url_for_post_ids(topic_id, ids), expected);
}

TEST_CASE("Can get posts with certain topic") {
  std::string raw_result = R"(
    {
      "posts": [
        {
          "id": 874799,
          "name": "Osvid",
          "username": "Óscar_David_Sánchez",
          "avatar_template": "/user_avatar/meta.discourse.org/óscar_david_sánchez/{size}/181941_2.png",
          "created_at": "2021-01-16T20:49:42.681Z",
          "like_count": 1,
          "blurb": "Hi! I think this is a great component. Just one quick question: Does it show the avatar flair image? I have installed this theme component in my local discourse installation. While I can see the flair...",
          "post_number": 38,
          "topic_title_headline": "Topic Author",
          "topic_id": 111021
        },
        {
          "id": 883527,
          "name": "Diana Rojas",
          "username": "Diana_Rojas1",
          "avatar_template": "/user_avatar/meta.discourse.org/diana_rojas1/{size}/200820_2.png",
          "created_at": "2021-02-02T22:06:55.714Z",
          "like_count": 1,
          "blurb": "Hi there When I install the \"header submenus\" component the component appears but overlaps my current forum header. image (6) The Discourse version I have is 2.4.1 Please if someone can guide me how t...",
          "post_number": 109,
          "topic_title_headline": "Header Submenus",
          "topic_id": 94584
        },
        {
          "id": 875618,
          "name": "hosna",
          "username": "hosna",
          "avatar_template": "/user_avatar/meta.discourse.org/hosna/{size}/99969_2.png",
          "created_at": "2021-01-19T08:24:29.678Z",
          "like_count": 2,
          "blurb": "Is this plugin no longer working? @fzngagan @angus",
          "post_number": 359,
          "topic_title_headline": "Topic Ratings Plugin",
          "topic_id": 94584
        }
      ],
      "users" : []
    }
)";

  auto post_ids = rd::discourse::get_post_ids(raw_result, 94584);
  ranges::sort(post_ids);
  REQUIRE_EQ(post_ids, std::vector<int>({875618, 883527}));
}

TEST_CASE("creates correct data source entry from post") {
  std::string post =
      R"({
        "id": 874799,
        "name": "Osvid",
        "username": "Óscar_David_Sánchez",
        "avatar_template": "/user_avatar/meta.discourse.org/óscar_david_sánchez/{size}/181941_2.png",
        "created_at": "2021-01-16T20:49:42.681Z",
        "cooked": "this is a sample review",
        "post_number": 38,
        "post_type": 1,
        "updated_at": "2021-01-17T00:20:30.259Z",
        "reply_count": 0,
        "reply_to_post_number": null,
        "quote_count": 0,
        "incoming_link_count": 0,
        "reads": 104,
        "readers_count": 103,
        "score": 35.8,
        "yours": false,
        "topic_id": 111021,
        "topic_slug": "topic-author",
        "display_username": "Osvid",
        "primary_group_name": null,
        "flair_name": null,
        "flair_url": null,
        "flair_bg_color": null,
        "flair_color": null,
        "flair_group_id": null,
        "version": 3,
        "can_edit": false,
        "can_delete": false,
        "can_recover": false,
        "can_see_hidden_post": false,
        "can_wiki": false,
        "read": true,
        "user_title": "",
        "bookmarked": false,
        "actions_summary": [
          {
            "id": 2,
            "count": 1
          }
        ],
        "moderator": false,
        "admin": false,
        "staff": false,
        "user_id": 79705,
        "hidden": false,
        "trust_level": 2,
        "deleted_at": null,
        "user_deleted": false,
        "edit_reason": null,
        "can_view_edit_history": true,
        "wiki": false,
        "mentioned_users": [],
        "activity_pub_enabled": false,
        "activity_pub_scheduled_at": null,
        "activity_pub_published_at": null,
        "activity_pub_deleted_at": null,
        "customer_flair_customer": null,
        "reactions": [
          {
            "id": "heart",
            "type": "emoji",
            "count": 1
          }
        ],
        "current_user_reaction": null,
        "reaction_users_count": 1,
        "current_user_used_main_reaction": false,
        "can_accept_answer": false,
        "can_unaccept_answer": false,
        "accepted_answer": false,
        "topic_accepted_answer": false
      })";

  std::string review = "this is a sample review";

  auto res =
      rd::discourse::post_to_data_source_entry(nlohmann::json::parse(post));

  REQUIRE_EQ(res.metadata.app_version, tl::nullopt);
  REQUIRE_EQ(res.metadata.location, tl::nullopt);
  REQUIRE_EQ(res.metadata.rating, 35.8);
  REQUIRE_EQ(res.metadata.impressions, 103);
  REQUIRE_EQ(std::get<1>(res.feedback), review);
}

TEST_CASE("Can convert post stream to schema") {
  std::string posts =
      R"({
  "post_stream": {
    "posts": [
      {
        "id": 874799,
        "name": "Osvid",
        "username": "Óscar_David_Sánchez",
        "avatar_template": "/user_avatar/meta.discourse.org/óscar_david_sánchez/{size}/181941_2.png",
        "created_at": "2021-01-16T20:49:42.681Z",
        "cooked": "this is a sample review",
        "post_number": 38,
        "post_type": 1,
        "updated_at": "2021-01-17T00:20:30.259Z",
        "reply_count": 0,
        "reply_to_post_number": null,
        "quote_count": 0,
        "incoming_link_count": 0,
        "reads": 104,
        "readers_count": 103,
        "score": 35.8,
        "yours": false,
        "topic_id": 111021,
        "topic_slug": "topic-author",
        "display_username": "Osvid",
        "primary_group_name": null,
        "flair_name": null,
        "flair_url": null,
        "flair_bg_color": null,
        "flair_color": null,
        "flair_group_id": null,
        "version": 3,
        "can_edit": false,
        "can_delete": false,
        "can_recover": false,
        "can_see_hidden_post": false,
        "can_wiki": false,
        "read": true,
        "user_title": "",
        "bookmarked": false,
        "actions_summary": [
          {
            "id": 2,
            "count": 1
          }
        ],
        "moderator": false,
        "admin": false,
        "staff": false,
        "user_id": 79705,
        "hidden": false,
        "trust_level": 2,
        "deleted_at": null,
        "user_deleted": false,
        "edit_reason": null,
        "can_view_edit_history": true,
        "wiki": false,
        "mentioned_users": [],
        "activity_pub_enabled": false,
        "activity_pub_scheduled_at": null,
        "activity_pub_published_at": null,
        "activity_pub_deleted_at": null,
        "customer_flair_customer": null,
        "reactions": [
          {
            "id": "heart",
            "type": "emoji",
            "count": 1
          }
        ],
        "current_user_reaction": null,
        "reaction_users_count": 1,
        "current_user_used_main_reaction": false,
        "can_accept_answer": false,
        "can_unaccept_answer": false,
        "accepted_answer": false,
        "topic_accepted_answer": false
      }
    ]
  },
  "id": 111021,
  "related_topics": null
})";

  std::string review = "this is a sample review";

  auto res =
      rd::discourse::raw_posts_to_schema(posts, "source_id", "tenant_id");

  REQUIRE_EQ(res.size(), 1);
  REQUIRE_EQ(res[0].source_id, "source_id");
  REQUIRE_EQ(res[0].tenant_id, "tenant_id");
  REQUIRE_EQ(res[0].entry.metadata.app_version, tl::nullopt);
  REQUIRE_EQ(res[0].entry.metadata.location, tl::nullopt);
  REQUIRE_EQ(res[0].entry.metadata.rating, 35.8);
  REQUIRE_EQ(res[0].entry.metadata.impressions, 103);
  REQUIRE_EQ(std::get<1>(res[0].entry.feedback), review);
}
