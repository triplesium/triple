#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string>
using namespace std::string_literals;

import triple.refl;
import triple.base;

struct Obj {
    int a;
};

using namespace triple;
using namespace triple::refl;

TEST_CASE("", "") {
    STATIC_REQUIRE(type<int>().name() == "int"s);
    STATIC_REQUIRE(type<Obj>().name() == "Obj"s);

    Cls::new_cls<Obj>().add_member("a", &Obj::a);
    auto& c = cls<Obj>();
    auto& m = c.member("a");
    Obj obj;
    obj.a = 5;
    REQUIRE(m.name() == "a"s);
    REQUIRE(m.get(obj).value<int>() == 5);
    m.set(obj, 10);
    REQUIRE(obj.a == 10);
}

int main(int argc, char* argv[]) {
    return Catch::Session().run(argc, argv);
}
