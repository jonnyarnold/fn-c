#include "cli.cpp"

TEST_CASE("parseCli") {

  SECTION("'help' returns success") {
    char* args[2];
    args[0] = "fn";
    args[1] = "help";

    REQUIRE( parseCli(2, &args[0]) == 0 );
  }

  SECTION("no args returns success") {
    char* args[1];
    args[0] = "fn";

    REQUIRE( parseCli(1, &args[0]) == 0 );
  }

}
