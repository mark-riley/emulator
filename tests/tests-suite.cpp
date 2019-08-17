#include "catch.hpp"

SCENARIO("Scenario Name", "[tag1][tag2]") {
    GIVEN("A situation") {
        WHEN("An action is performed") {
            THEN("We expect"){
                REQUIRE(true);
            }
        }
        WHEN("Another action is performed") {
            THEN("We alternatively expect"){
                REQUIRE(true);
            }
        }
    }
}