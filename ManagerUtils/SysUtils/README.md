# `SysUtils` Package

A collection of packages that could help with system interactions. Also existing C functions to C++ string formats. List of header files.


## List of header files
Here is the exhaustive list of all the header files in this file.

### [HiggCombineSubmitter](./interface/HiggsCombineSubmitter.hpp)
Since the [higgs combine package](https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit) is heavily version dependent. You might wish to control the submission of locally generated datacard to a designated higgs combine package stored in a different CMSSW directory. This class allows you to be ablve to do so using C++ code. For the full documentation, see the dedicated class [documentation](./doc/HiggsCombineSubmitter.md)


### [OptsNamer](./interface/OptsNamer.hpp)
When we are passing arguments via command line, we typically want the arguments to be short, but when displaying the displayed argument of a generated graph or document, we might want a longer more descriptive version of the argument. This class allows us to store the argument passed to a main function in the [boost::program_options](http://www.boost.org/doc/libs/1_61_0/doc/html/program_options.html) style and extract the extended names/variables with ease. For the full documentation, see the the dedicated class [documentation](./doc/OptsNamer.md)

### [PackagePathMgr](./interface/PackagePathMgr.hpp)
We wish to store the program output to a `results` directory under a subpackage. Given the structure of the CMSSW packages, this class helps to handle string to do so. For the full documentation, see the dedicated class [documentation](./doc/PackagePathMgr.md)

### [PathUtils](./interface/PathUtils.hpp)
A collection of functions for processing system paths as `std::string`. Detail documentation written directly in the [header file](./interface/PathUtils.hpp)

### [ProcessUtils](./interface/ProcessUtils.hpp)
A collection of functions for detecting processes running on the machine. Handing for script automation with `C++` generated scripts. Detailed documentations written directly in the [header file](./interface/ProcessUtils.hpp)

### [TimeUtils](./interface/TimeUtils.hpp)
Time displaying utility functions. Detail documentations written directly in the [header file](./interface/TimeUtils.hpp)
