# yices-solver

[![Linux Build](https://github.com/a16z/yices-solver/actions/workflows/build-linux.yml/badge.svg)](https://github.com/a16z/yices-solver/actions/workflows/build-linux.yml)
[![macOS Build](https://github.com/a16z/yices-solver/actions/workflows/build-mac.yml/badge.svg)](https://github.com/a16z/yices-solver/actions/workflows/build-mac.yml)
[![Windows Build](https://github.com/a16z/yices-solver/actions/workflows/build-windows.yml/badge.svg)](https://github.com/a16z/yices-solver/actions/workflows/build-windows.yml)

Platform-tagged wheels bundling the [Yices 2](https://github.com/SRI-CSL/yices2) SMT solver binaries (CLI + shared library).

Note that this *only* includes the binaries and not the Python bindings for yices, released separately as the [yices](https://pypi.org/project/yices/) package:

- [yices-solver](https://pypi.org/project/yices-solver/) (this package): binaries and shared libraries only, no Python bindings
- [yices](https://pypi.org/project/yices/) (published by SRI): Python bindings only, no binaries or shared libraries

## Usage

Make the binaries available on the PATH:

```sh
uv tool install yices-solver

# check that the binary is available
yices --version
```

Install in a venv using uv (recommended):

```sh
uv venv
uv pip install yices-solver

# check that the binary is available
uv run yices --version
```

Install in a venv using pip:

```sh
python -m venv .venv
source .venv/bin/activate
python -m pip install yices-solver

# check that the binary is available
yices --version
```

## How this works

### Build process

We publish 4 wheels:
- `yices_solver-x.y.z-py3-none-manylinux_2_28_x86_64.whl`
- `yices_solver-x.y.z-py3-none-macosx_13_0_x86_64.whl`
- `yices_solver-x.y.z-py3-none-macosx_14_0_arm64.whl`
- `yices_solver-x.y.z-py3-none-win_amd64.whl`

The Linux and macOS wheels are:
- built from source with the appropriate Yices tag
- with MCSAT enabled
- statically linked against libcudd, libpoly and GMP

The Windows wheel is not built from source, we extract the official Yices release binaries and repackage them as a wheel.


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
