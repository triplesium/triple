#include <catch2/catch_test_macros.hpp>

import triple.ecs.v2;
import triple.refl;
import triple.base;

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Obj {
    int x;
};

using namespace triple;
using namespace triple::ecs::v2;

TEST_CASE("Utils", "") {
    SECTION("Hash") {
        STATIC_REQUIRE(hash_types<Position>() == hash_types<Position>());
        STATIC_REQUIRE(hash_types<Position>() != hash_types<Velocity>());
        STATIC_REQUIRE(
            hash_types<Position>() != hash_types<Velocity, Position>()
        );
        STATIC_REQUIRE(
            hash_types<Position, Velocity>() == hash_types<Velocity, Position>()
        );
    }
}

TEST_CASE("Archetype", "") {
    Archetype<Position, Velocity> a;
    SECTION("Basic") {
        STATIC_REQUIRE(a.has_component<Position>());
        STATIC_REQUIRE(a.has_component<Velocity>());
        STATIC_REQUIRE_FALSE(a.has_component<Obj>());

        auto& x = a.get_column<Position>();

        STATIC_REQUIRE(a.get_column_index<Position>() == 0);
        STATIC_REQUIRE(a.get_column_index<Velocity>() == 1);

        Entity e = 1;
        a.add_entity(e, Row {Position {1.f, 2.f}, Velocity {3.f, 4.f}});

        REQUIRE(a.has_entity(e));
        a.remove_entity(e);
        REQUIRE_FALSE(a.has_entity(e));
    }
    SECTION("Hash") {
        Archetype<Position, Velocity> b;
        Archetype<Velocity, Position> c;
        Archetype<Position> d;
        Archetype<Velocity> e;

        STATIC_REQUIRE(a.hash() == b.hash());
        STATIC_REQUIRE(a.hash() == c.hash());
        STATIC_REQUIRE(a.hash() != d.hash());
        STATIC_REQUIRE(a.hash() != e.hash());
    }
    SECTION("Slice") {
        a.add_entity(1, Row {Position {1.f, 2.f}, Velocity {3.f, 4.f}});
        a.add_entity(2, Row {Position {5.f, 6.f}, Velocity {7.f, 8.f}});

        auto slice = a.slice<Position>();
        auto iter = slice.begin();
        auto [pos1] = *iter;
        REQUIRE(pos1.x == 1.f);
        REQUIRE(pos1.y == 2.f);

        ++iter;
        auto [pos2] = *iter;
        REQUIRE(pos2.x == 5.f);
        REQUIRE(pos2.y == 6.f);

        ++iter;
        REQUIRE(iter == slice.end());
    }
}

TEST_CASE("Query", "") {
    SECTION("Basic") {
        Query<Position, Velocity> q1;
        Query<Position> q2;

        Archetype<Position, Velocity> a1;
        a1.add_entity(1, Row {Position {1.f, 2.f}, Velocity {3.f, 4.f}});
        STATIC_REQUIRE(q1.matches(a1));
        STATIC_REQUIRE(q2.matches(a1));

        Archetype<Position> a2;
        a2.add_entity(1, Row {Position {5.f, 6.f}});
        STATIC_REQUIRE_FALSE(q1.matches(a2));
        STATIC_REQUIRE(q2.matches(a2));

        q2.add_if_matches(a1);
        q2.add_if_matches(a2);

        auto view = q2.view();
        auto iter = view.begin();
        auto [pos1] = *iter;
        REQUIRE(pos1.x == 1.f);
        REQUIRE(pos1.y == 2.f);

        ++iter;
        auto [pos2] = *iter;
        REQUIRE(pos2.x == 5.f);
        REQUIRE(pos2.y == 6.f);

        ++iter;
        REQUIRE(iter == view.end());
    }
}
