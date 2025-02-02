#include <catch2/catch_test_macros.hpp>

import triple.ecs.v2;
import triple.refl;
import triple.base;

struct Position {
    float x;
    float y;

    auto operator<=>(const Position&) const = default;
};

struct Velocity {
    float x;
    float y;
    auto operator<=>(const Velocity&) const = default;
};

struct Obj {
    int x;
};

using namespace triple;
using namespace triple::refl;
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
    SECTION("Basic") {
        Archetype a(0, {type<Position>(), type<Velocity>()});
        Entity e1 = 0;
        auto row = a.alloc(e1);
        a.set_component(type<Position>(), row, Position {1, 2});
        Position& p = a.get_component(type<Position>(), row).value<Position>();
        REQUIRE(p.x == 1);
        REQUIRE(p.y == 2);
    }
}

TEST_CASE("World", "") {
    World w;
    SECTION("Components") {
        Entity e1 = w.entity();
        Entity e2 = w.entity();

        w.add_component<Position>(e1, Position {1, 2});
        w.add_component<Position>(e2, Position {3, 4});
        w.add_component<Velocity>(e1, Velocity {5, 6});

        REQUIRE(w.has_component<Position>(e1));
        REQUIRE(w.has_component<Position>(e2));
        REQUIRE(w.has_component<Velocity>(e1));
        REQUIRE_FALSE(w.has_component<Velocity>(e2));

        REQUIRE(w.get_component<Position>(e1).x == 1);
        REQUIRE(w.get_component<Position>(e1).y == 2);
        REQUIRE(w.get_component<Position>(e2).x == 3);
        REQUIRE(w.get_component<Position>(e2).y == 4);
        REQUIRE(w.get_component<Velocity>(e1).x == 5);
        REQUIRE(w.get_component<Velocity>(e1).y == 6);

        w.remove_component<Position>(e1);
        REQUIRE_FALSE(w.has_component<Position>(e1));
    }
}

TEST_CASE("Query", "") {
    World w;
    Entity e1 = w.entity();
    Entity e2 = w.entity();

    w.add_component<Position>(e1, Position {1, 2});
    w.add_component<Position>(e2, Position {3, 4});
    w.add_component<Velocity>(e1, Velocity {5, 6});
    SECTION("") {
        Query<Position> q(w);
        auto iter = q.begin();
        auto [pos1] = *iter;
        REQUIRE((pos1 == Position {1, 2} || pos1 == Position {3, 4}));
        ++iter;
        auto [pos2] = *iter;
        REQUIRE(
            ((pos1 != pos2) &&
             (pos2 == Position {1, 2} || pos2 == Position {3, 4}))
        );
    }
}
