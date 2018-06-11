
#include "log.hpp"
#include "doctest.h"
#include <sstream>

using namespace std::string_literals;
TEST_SUITE_BEGIN("liblog");

TEST_CASE("liblog - log should write to a ostream"){
    auto file = std::make_shared<std::ostringstream>();
    logger::set_output_location(file);

    LOG(note,"{}", "hello");

    std::string logged = file->str();
    CHECK_EQ(logged, "hello"s);
}