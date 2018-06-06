# liblog

Liblog is a library that provides threadsafe logging features. Internally there is a single static logging class that all modules will use to send to a log file. Internally it uses fmt to format code. Its public facing API is simple, with a few macros, an enum, and two control functions.

```cpp
#define LIBLOG_LOG(severity, fmt_str, arguments_to_fmt_str...)
#ifndef LIBLOG_NO_SIMPLE_MACRO_NAMES
#   define LOG(...) LIBLOG_LOG(__VA_ARGS__)
#endif

namespace log {
    enum severity {
        debug,
        info,
        note,
        warning,
        error,
        fatal_error
    };

    void to_log(std::string const& s);
    void set_visable_severity(severity s);
    void set_output_location(std::string const& name);
    void set_output_location(std::unique_ptr<std::ostream> name);
}
```

Liblog uses fmt's new compile time parsing of format strings, eliminating overhead at runtime. See http://fmtlib.net/dev/api.html for details. See https://stackoverflow.com/a/10654526 for details about getting current time of day accurately.

### API

```cpp
#define LIBLOG_LOG(severity, fmt_str, arguments_to_fmt_str...)
#define LOG(...)
```

Variatic macro that does the logging. `severity` is the unqualified name of one of the enum values. All severity at or above `note` are printed by defualt. `debug` messages can only be printed if `LIBLOG_DEBUG` is defined. `fmt_str` is a c-string literal depicting what the error message should say. For a function call that looks like this:

```cpp
// mylib.cpp
LOG(warning, "{} has a problem. See {}.", "MyLibrary", "github.com");
```

It will print out:

```
2018.06.06 16:54.678: MyLibrary has a problem. See github.com.
```

With `LIBLOG_DEBUG` it will print out:

```
2018.06.06 16:54.678: C:/build/mylib/mylib.c:63: MyLibrary has a problem. See github.com.
```

---

```cpp
void log::to_log(std::string const& s);
```

Prints the string directly to the logfile, unqualified. Meant for a log file header or similar construct.

---

```cpp
void log::set_visable_severity(severity s);
```

Choose the cuttoff for what severity messages are displayed. By default, everything above `note` is displayed.  If `LIBLOG_DEBUG` is defined, all messages are printed, regardless of severity level set.  

---

```cpp
void log::set_output_location(std::string const& name);
void log::set_output_location(std::shared_ptr<std::ostream> name);
```

Sets the logfile location. If this is changed after the logfile has been written to, the new log file will NOT have the contents of the old one, the old one will be left in place. If it is passed a string it will open up a file with that path, clearing the file in the process. If it is passed a pointer to ostream, it will print directly to that ostream.
