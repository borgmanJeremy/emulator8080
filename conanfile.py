from conans import ConanFile, CMake, tools


class EmulatorConan(ConanFile):
    name = "emulator"
    version = "0.0.1"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Emulator here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"


    def configure(self):
        self.options["SDL2"].sdl2main = False 

    def requirements(self):
        self.requires("cxxopts/v2.2.0")
        self.requires("Catch2/2.11.0@catchorg/stable")
        self.requires("sdl2/2.0.10@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["FMT_HEADER_ONLY"] = False
        cmake.definitions["FMT_HEADER_ONLY"] = False
        cmake.configure(source_folder="emulator")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="hello")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["hello"]

