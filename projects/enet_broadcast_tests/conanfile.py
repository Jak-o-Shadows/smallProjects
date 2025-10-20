from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake

class EnetBroadcastExample(ConanFile):
    name = "EnetBroadcastExample"
    version = "0.1"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.20]")  # Pretty arbitrary tbh

    def requirements(self):
        self.requires("asio/1.28.1")

    def configure(self):
        self.options["asio"].header_only = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)
