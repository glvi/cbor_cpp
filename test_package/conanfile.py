
import os

from conan import ConanFile
from conan.tools.gnu import AutotoolsToolchain, Autotools, AutotoolsDeps
from conan.tools.layout import basic_layout
from conan.tools.build import can_run


class glvi_cborTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "AutotoolsDeps", "AutotoolsToolchain"
    win_bash = True

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        autotools = Autotools(self)
        autotools.autoreconf()
        autotools.configure()
        autotools.make()

    def layout(self):
        basic_layout(self)

    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "main")
            self.run(cmd, env="conanrun")
