# `Maths` package

This package is a contains classes to help with mathematical operations. Especially to help with parameters calculation

# `Parameter` class
Defined in [`Parameter.hpp`](./interface/Parameter.hpp), it is a class which is a representation of a measured parameter with (potentially asymmetric) error. Basic arithmatics, as well as access to central value, absolute and relative errors is defined in this class. The treatment of asymmetric error here is wrong! The correct functions for treating asymmetric error is currently under construction.

## `Parameter` formatting
Defining function that help represent the `Parameter` class in string formats. Function are defined in the in [`ParameterFormat.hpp`](./interface//ParameterFormat.hpp) class. Note that most is designed for raw latex style strings.

## `Efficiency` calculation
Calculating selection efficiencies is rather tedious, especially with low statistics or near boundary results. The header [`Efficiency.hpp`](./itnerface/Efficiency.hpp) provides a set for itnerface functions to storing efficiency results in the `Parameter` class. Powered by the ROOT [TEfficiency](https://root.cern.ch/doc/master/classTEfficiency.html) static functions.
