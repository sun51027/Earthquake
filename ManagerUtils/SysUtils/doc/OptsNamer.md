# `mgr::OptsNamer.md` Class

When we are interacting with our program via command line, we which our input to be short. While we might wish to represent this options input in a more discriptive string or even other datatypes. This class allows us to do just that.

## Preparing the options and the corresponding json file.

This program uses [boost program options](http://www.boost.org/doc/libs/1_61_0/doc/html/program_options.html) for option parsing, so you will need to get familiar with the single option input settings, anyway, we define our options like maybe like:

```c++
boost::program_options::options_description  desc("My options");
desc.add_options()
   ("channel,c" , opt::value<string>(), "which channel to run" )
   ("fitfunc,f" , opt::value<string>(), "which fit function to use")
;
```

We could then parse the arguments of the main function to our `OptsNamer` class by:

```c++
mynamer.LoadOptions( desc , argc, argv )
```

We could then check the raw input of an arguments by the functions:

* `bool HasOption( "channel" )`: check if argument "channel" has been passed to main function
* `std::string InputStr( "channel" )`: return input argument to "channel" as a string
* `double      InputDou( "channel" )`: return input argument to "channel" as a double
* `int         InputInt( "channel" )`: return input argument to "channel" as a int

For extended strings, we will need to load our `OptsNamer` class with a json file (using the `LoadJsonFile(std::string)`) with the following format
```json
{
   "channel":{
      "Input1":{
         "Name1":"Very Long and descriptive name",
         "Name2":"\\latex \\frac{style}{input}",
         "Name3":"#root #frace{style}{input}"
      },
      "Input2":{
         "Name1":"Very Long and descriptive name again",
         "Name2":"\\latex \\frac{style}{input} again",
         "Name3":"#root #frace{style}{input} again"
      }
   },
   "fitfunc":{
      "Input1":{
         "Name1":"Very Long and descriptive name",
         "Name2":"\\latex \\frac{style}{input}",
         "Name3":"#root #frace{style}{input}"
      },
      "Input2":{
         "Name1":"Very Long and descriptive name again",
         "Name2":"\\latex \\frac{style}{input} again",
         "Name3":"#root #frace{style}{input} again"
      }   
   }
}
```
Then we could call the method `myname.GetExtName( "channel" , "Name1")`. If we call the our main function like
```
./main --channel Input1 --fitfunc Input2
```
The method call above would return `"Very Long and descriptive name"`.

You could still get the raw results of the boost program options parsing by the method `GetMap()`.
