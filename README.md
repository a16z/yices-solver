# yices-solver

[![Linux Build](https://github.com/a16z/yices-solver/actions/workflows/build-linux.yml/badge.svg)](https://github.com/a16z/yices-solver/actions/workflows/build-linux.yml)
[![macOS Build](https://github.com/a16z/yices-solver/actions/workflows/build-mac.yml/badge.svg)](https://github.com/a16z/yices-solver/actions/workflows/build-mac.yml)
[![Windows Build](https://github.com/a16z/yices-solver/actions/workflows/build-windows.yml/badge.svg)](https://github.com/a16z/yices-solver/actions/workflows/build-windows.yml)

A platform-tagged Python package bundling the [Yices 2](https://github.com/SRI-CSL/yices2) SMT solver binaries, [available on PyPI](https://pypi.org/project/yices-solver/).


### What is Yices?

Yices is a fast and open source SMT (Satistifiability Modulo Theories) solver, comparable to Z3, CVC5, Bitwuzla, etc.

SMT solvers are commonly used in software verification, bug finding, constraint solving and optimization.

For instance, [a16z/halmos](https://github.com/a16z/halmos) is a symbolic testing tool that uses SMT solvers such as Yices as a backend.


### Why a `yices-solver` Python package?

Until now, there was no easy way for a Python application to require the Yices runtime. Users would typically be instructeed to install Yices separately or to build it from source. This package solves this, it is now possible to require installation of the Yices binaries directly:

```toml
# pyproject.toml

[project]
dependencies = [
    "yices-solver==2.6.5"
]
```

The dependency resolution mechanism will take care of resolving the appropriate version of the package to install given the current platform and architecture (e.g. Linux/x86, macOS/ARM, etc.).


### Difference with the `yices` package

Note that this package *only* includes the binaries and not the Python bindings for yices. The Python bindings are released separately as the [yices](https://pypi.org/project/yices/) package on PyPI, published by the authors of Yices.

In summary:

- [yices-solver](https://pypi.org/project/yices-solver/) (this package): binaries and shared libraries only, no Python bindings
- [yices](https://pypi.org/project/yices/) (published by SRI): Python bindings only, no binaries or shared libraries


## Quick start

To use the solver interactively, we recommend using [uv](https://github.com/astral-sh/uv):

```sh
# this will:
# - create a virtual environment,
# - install yices-solver in it
# - make the binaries (e.g. yices-smt2) available on the PATH
uv tool install yices-solver

# check that the binary is available
yices-smt2 --version

# solve a simple smt2 query
echo "(set-logic QF_LIA) (declare-const x Int) (assert (< x 10)) (check-sat) (get-model)" | yices-smt2
```


## Advanced usage

### In projects

To install in a manually managed venv using pip:

```sh
python -m venv .venv
source .venv/bin/activate
python -m pip install "yices-solver~=2.6.5"

# check that the binary is available
yices --version
```


> [!TIP]
> In order to keep the version number of this package and of yices in sync (e.g. version 2.6.5 of `yices-solver` should bundle version 2.6.5 of the Yices binaries), we use [post-release](https://peps.python.org/pep-0440/#post-releases) tags when we make changes to the CI scripts. As a result, you should probably use a version specifier like `"yices-solver~=2.6.5"` to pick-up the latest post-release tag for the 2.6.5 version.


### In scripts

You can also add it to your pyproject.toml as a dependency or inline as a script using a uv shebang:

```py
#! /usr/bin/env uv run

# /// script
# requires-python = ">=3.12"
# dependencies = [
#   "yices-solver~=2.6.5",
# ]
# ///

import subprocess

# this should print the location the yices binary installed inside the temporary venv
subprocess.run(["which", "yices-smt2"])

# this should invoke the yices binary as an external process and print `sat`
subprocess.run(
    ['yices-smt2'],
    input='(set-logic QF_LIA) (declare-const x Int) (assert (> x 0)) (check-sat)',
    text=True
)
```

### With the Python bindings

Support is experimental and needs to be documented, but it should be possible to:
- install both `yices` and `yices-solver`
- locate the `lib/` directory in the venv
- overwrite the dynamic library loading path (e.g. `DYLD_LIBRARY_PATH` on macOS)
- import `yices` such that libyices loads properly and the API can be used normally


## How this works

### Build process

We publish 4 wheels:
- `yices_solver-x.y.z-py3-none-manylinux_2_28_x86_64.whl`
- `yices_solver-x.y.z-py3-none-macosx_13_0_x86_64.whl`
- `yices_solver-x.y.z-py3-none-macosx_14_0_arm64.whl`
- `yices_solver-x.y.z-py3-none-win_amd64.whl`

The Linux and macOS wheels are:
- built from source with the appropriate Yices tag (meaning that `yices_solver-x.y.z` should package the binaries for Yices tag `x.y.z`)
- with MCSAT support enabled
- statically linked against libcudd, libpoly and GMP

The Windows wheel is not built from source, we extract the official Yices release binaries and repackage them as a wheel. A build from source on Windows would be a welcome contribution!

We try to make the publishing process as transparent and secure as possible, which is why we use [Trusted Publishing](https://blog.trailofbits.com/2023/05/23/trusted-publishing-a-new-benchmark-for-packaging-security/).

The build scripts can be found here:
- [.github/workflows/build-linux.yml](https://github.com/a16z/yices-solver/blob/main/.github/workflows/build-linux.yml)
- [.github/workflows/build-mac.yml](https://github.com/a16z/yices-solver/blob/main/.github/workflows/build-mac.yml)
[.github/workflows/build-windows.yml](https://github.com/a16z/yices-solver/blob/main/.github/workflows/build-windows.yml)


### Installation

When you install the `yices-solver` package (e.g. with pip), the resolver should pick the right wheel for your OS and architecture. The macOS/Linux wheels are organized like this:

```
yices_solver-x.y.z
├── yices_solver
│   └── lib
│       ├── libyices.2.dylib /
│       └── libyices.a
├── yices_solver-x.y.z.data
│   └── data
│       └── bin
│           ├── yices
│           ├── yices-sat
│           ├── yices-smt
│           └── yices-smt2
└── yices_solver-x.y.z.dist-info
    ├── METADATA
    ├── RECORD
    ├── WHEEL
    ├── licenses
    │   └── LICENSE
    └── top_level.txt
```

And similarly, the Windows wheel:

```
yices_solver-x.y.z-py3-none-win_amd64
├── yices_solver
│   └── lib
│       └── libyices.dll
├── yices_solver-x.y.z.data
│   └── data
│       └── Scripts
│           ├── yices-sat.exe
│           ├── yices-smt.exe
│           ├── yices-smt2.exe
│           └── yices.exe
└── yices_solver-x.y.z.dist-info
    ├── licenses
    │   └── LICENSE
    ├── METADATA
    ├── RECORD
    ├── top_level.txt
    └── WHEEL
```

As a result, when the wheels are installed in a virtual environment, the binaries should be placed under `<venv>/bin` (resp. `<venv>\Scripts`) and available in PATH.


## License

The `yices-solver` package is a binary distribution of Yices with no modifications to the source code, and inherits the GPL-3.0 license of Yices itself.


## Acknowledgements

Yices is developed independently by the Computer Science Laboratory at SRI International (https://yices.csl.sri.com/), all credits go to the developers and contributors.

Please refer to the Yices manual for questions about usage, issues, bug reports, etc.


## Disclaimer

_This code is being provided as is. No guarantee, representation or warranty is being made, express or implied, as to the safety or correctness of the code. It has not been audited and as such there can be no assurance it will work as intended, and users may experience delays, failures, errors, omissions or loss of transmitted information. Nothing in this repo should be construed as investment advice or legal advice for any particular facts or circumstances and is not meant to replace competent counsel. It is strongly advised for you to contact a reputable attorney in your jurisdiction for any questions or concerns with respect thereto. a16z is not liable for any use of the foregoing, and users should proceed with caution and use at their own risk. See a16z.com/disclosures for more info._
