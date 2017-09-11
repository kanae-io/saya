# saya

High layer components for modern C++ (partially header-only library)  
Current status: _public alpha_

---

## IMPORTANT NOTICE

### 1. **Make sure your toolchain environment is sane.**

_Modifying toolchain related binaries under system directories such as `/usr/bin/` and `/usr/local/bin/` may lead to serious hazard._  
_It is advised not to symlink CC binaries to an unofficial distribution, like `ln -s g++-7 g++`._

### 2. **Make sure your C++ runtime environment is sane.**

**NEVER** install unofficial runtimes (like `libc++` on Ubuntu) into system directories.  
Also, **DON'T** statically link C++ runtime to your program unless you know what you are doing.

### 3. **Make sure your Boost installation is sane.**

**NEVER** use Boost binaries provided by your distro.

Doing so will essentially degrade your program by disabling recent C++ features.  
Fundamental optimizations like `-march=native` will not be applied, too.  
See the section [General Advice](#general-advice) for further details.

**NEVER** Install Boost under `/usr/local/` .

Instead, dig a prefix for your specific project:

```bash
mkdir /usr/local/my-project-prefix/
```

Recommended build options are as follows:

    ./bootstrap.sh --with-toolset=clang --without-libraries=mpi,python,graph_parallel -with-icu=/usr/lib/x86_64-linux-gnu --prefix=/usr/local/my-project-prefix/
    ./b2 toolset=clang threading=multi variant=release link=shared runtime-link=shared address-model=64 cflags="-march=native" cxxflags="-std=c++14" define=BOOST_SYSTEM_NO_DEPRECATED -j5

---

## <a name="general-advice"></a>General Advice

### Deploying your C++ application to cloud platform

It is important to optimize your binary to the target instance type.  
You must ensure your application is configured properly before deploying to a cloud platform.

1. Check the **minimum guaranteed architecture level** for your instance family.  
  _Example:_ If your instance is **ec2.c4**, the current documentation says: `Intel Xeon E5-2666 v3 (Haswell)`
2. Check the compiler's documentation and get the best `-march` option.  
  _Example:_ For Haswell, it is `-march=haswell`

**Note: If you are using some kind of building framework such as `distcc`, you should not specify the `-march=` option.**  
**Note: If the architecture of your CI's build slaves differ from your production environment's, consider to cross-compile.**


---

## Requirements

### OS

- Ubuntu 16.04 (tested on [Ubuntu Server](https://www.ubuntu.com/download/server) 16.04.2 LTS)
- _Other linux: Untested, should work_
- _macOS: Untested, should work_
- _*BSD: Not supported._
- Windows 10: VS 2017, 64bit

### C++ Version

- C++17
- C++14 (deprecated)

### Toolchain

- [clang++](http://clang.llvm.org/cxx_status.html) >= 6 ([Ubuntu repo](http://apt.llvm.org/)) or
- [g++](https://gcc.gnu.org/projects/cxx-status.html) >= 7.1.0 ([Ubuntu repo](https://launchpad.net/~ubuntu-toolchain-r/+archive/ubuntu/test))
- [CMake](http://www.cmake.org/) >= 3.8

### Library

- [Boost C++ Libraries](http://www.boost.org/) >= 1.65.1

---

## Building

```bash
rm -rf build && mkdir build && cd build
cmake [-DPARAM=VALUE] ..
make -j5
make -j5 build_all_tests
ctest -j 5 -V
```

### CMake Parameters

| Name | Value | Default | Description |
|---|:---:|:---:|---|
| `BOOST_ROOT` | _Path_ | _N/A_ |  (Mandatory) Boost root path |
| `CMAKE_TOOLCHAIN_FILE` | _Path_ | _N/A_ |  (Mandatory) Your toolchain definition file |
| `CMAKE_BUILD_TYPE` | `Release` or `Debug` | `Release` |  Enable/disable optimization |
| `SAYA_FORCE_BOOST_VERSION` | _Version_ | `"1.65.0"` | Boost version for your toplevel project |

---

## Using saya in your program (CMake)

Generate `saya-config.cmake`:

```bash
cd path/to/saya/build
make install
```

```cmake
set(saya_DIR "path/to/saya/build")
find_package(saya REQUIRED)
add_dependencies(your_program saya)
```

---

## License
Copyright (c) 2017- Nana Sakisaka.

Distributed under the MIT License; see [LICENSE](LICENSE) for the copy.

