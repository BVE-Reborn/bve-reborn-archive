#pragma once

#ifdef WIN32
#define DIRECTORY ""
#else
#define DIRECTORY "libparsers/"
#endif

void test_csv();
void test_csv_obj();
void test_animated();
void test_fs();
void test_csv_route();
