#include <PluginProcessor.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE ("one is equal to one", "[dummy]")
{
    REQUIRE (1 == 1);
}

// https://github.com/McMartin/FRUT/issues/490#issuecomment-663544272

/**
    Punting on tests for now. There are several issues that are making this break.
    1. Memory leaks. Wierd.
 */
//ValentineAudioProcessor testPlugin;
//TEST_CASE("Plugin instance name", "[name]")
//{
//  CHECK_THAT(testPlugin.getName().toStdString(),
//             Catch::Matchers::Equals("Valentine"));
//}
