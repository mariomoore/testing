#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

/*
SCENARIO("Game with 10 strikes", "[points]") {

}

SCENARIO("Game with 9 strikes and miss", "[points]") {

}

SCENARIO("Game with all misses", "[points]") {

}

SCENARIO("Game with 10 spares", "[points]") {

}

SCENARIO("Game with 9 spares and miss", "[points]") {

}

SCENARIO("Game with all nines", "[points]") {

}
*/


SCENARIO("Count 10 frames") {

}

SCENARIO("Count single frame during game") {
    GIVEN("The bowler got up first try") {
        WHEN("The first ball in a frame knocks down all ten pins") {
            THEN("Frame score is 10") {

            }
        }
        WHEN("The first ball in a frame knocks down 5 pins") {
            THEN("Frame score is 5") {

            }
        }
        WHEN("The firts ball in a frame knocks down none pins") {
            THEN("Frame score is 0") {

            }
        }
    }
    GIVEN("The bowler got up second try") {
        WHEN("The second ball knocks down all remain pins") {
            THEN("Frame score is 10") {

            }
        }
        WHEN("The first ball knocks down 5 pins and the second ball knocks down 4 pins") {
            THEN("Frame score is 9") {

            }
        }
        WHEN("The first and the second ball knocks down none pins") {
            THEN("Frame score is 0") {

            }
        }
    }
}

SCENARIO("Count single frame of 10") {
    GIVEN("Frame with strike") {
        WHEN("Next 2 balls are 10") {
            THEN("Frame score is 20") {

            }
        }
        WHEN("Next 2 balls are 0") {
            THEN("Frame score is 10") {

            }
        }
        WHEN("Next 2 balls are 5") {
            THEN ("Frame score is 15") {

            }
        }
    }
    GIVEN("Frame with spare") {
        WHEN("Next ball is 10") {
            THEN("Frame score is 20") {

            }
        }
        WHEN("Next ball is 0") {
            THEN("Frame score is 10") {

            }
        }
        WHEN("Next ball is 5") {
            THEN ("Frame score is 15") {

            }
        }
    }
}
