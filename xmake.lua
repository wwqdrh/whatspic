add_rules("mode.debug", "mode.release")

package("foo")
    add_deps("cmake", "opencv", "libiconv")
    set_sourcedir(path.join(os.scriptdir(), "src/foo"))
    set_policy("package.install_always", true)
    on_install(function (package)
        local configs = {}
        import("package.tools.cmake").install(package, configs)
    end)
package_end()

add_requires("foo")

target("demo")
    set_kind("binary")
    add_packages("foo")
    add_files("src/main.cpp")
    add_rpathdirs("/usr/local/lib/libtorch/lib")
    