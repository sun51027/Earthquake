# ManagerUtils {#mainpage}

This is a collection of utility classes intended to inhance the analysis experience in CMSSW environment, with the main focus being on channel based analysis over multiple EDM files. See unit testing file for examples on how to use these classes or check out the.

---

## Quick overview

### [BaseClass](./BaseClass/)
Base classes and utility classes for the Main classes to come.


### [Maths](./Maths/)
Math utility functions and classes.


### [SampleMgr](./SampleMgr/)
Wrapper class for handling multiple `EDM` file in "sample" units, with common cross-section, selection efficiencies... etc  in fwlite framework. Handy for custom analysis level code.


### [HistMgr](./HistMgr/)
Histogram manager class, allows multiple class instances of one class with the same title/axis size.  

### PlotUtils
Commonly used plotting functions used in root.

### SysUtils
System interaction utility functions and classes.

---

## Code style overview

* The namespace used in this package will always be `mgr` if present.
* Header files will always exists in the `interface` directory for the functions and classes with the `.hpp` extension. The only exception are EDM plugins classes, where a standalone header file will be omitted.
* Source code file will always have the extension of `.cc`.
* Class definition, public member functions, and function should always be in camel case format, with the first characeter in uppercase.
* Functions should have inline detailed documentation in their own header files.
* Classes should have detailed documentations in a separate markdown file in each supackages `doc` directory (under construction)
* No comment is the best comment! Try and keep the control flow intuitive, try and keep all comment at the beginning of the function and avoid inline comments as much as possible.
# ManagerUtils
