
import os

from conan import ConanFile
from conan.tools.gnu import AutotoolsToolchain, Autotools
from conan.tools.layout import basic_layout
from conan.tools.apple import fix_apple_shared_install_name


class glvi_cborConan(ConanFile):
    name = "glvi_cbor"
    version = "0.1"
    package_type = "library"
    win_bash = True

    # Optional metadata
    license = "GPL-3.0-or-later"
    author = "Kai Lothar John (l.john@glvi.de)"
    url = "https://github.com/glvi/cbor_cpp"
    description = "Utilities for decoding Concise Binary Object Notation"
    topics = ("cbor")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = "configure.ac", "Makefile.am", "src/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        basic_layout(self)

    def generate(self):
        at_toolchain = AutotoolsToolchain(self)
        at_toolchain.update_configure_args({
            "--prefix": None ## Deals with the issue that CONAN sets prefix to / by default
        })
        at_toolchain.generate()

    def build(self):
        autotools = Autotools(self)
        autotools.autoreconf()
        autotools.configure()
        autotools.make()

    def package(self):
        autotools = Autotools(self)
        autotools.install()
        fix_apple_shared_install_name(self)

    def package_info(self):
        self.cpp_info.libs = ["glvi_cbor"]
