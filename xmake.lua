add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

add_requires("utest.h")

target("main")
    set_kind("binary")
    set_languages("c11")
    add_files("src/*.c")
    remove_files("src/test.c")
    add_headerfiles("src/*.h")
    set_rundir("$(projectdir)/resources")

target("tests")
    set_kind("binary")
    set_languages("c11")
    add_packages("utest.h")
    add_defines("TESTS")
    add_files("src/*.c")
    remove_files("src/main.c")
    add_headerfiles("src/*.h")


