# `mgr::HiggsCombineSubmitter` Classes

These classes allow Higgs combine jobs to be submitted from within a C++ code which will be handy if you are using automatically generated datacard directly.

## `mgr::CombineRequest` Class
We represent a request for a Higgs Combine job by a all public class with the following parameters

* `std::string card_file` : Path to the card file
* `std::string output_file` : Path to store the output of the higgs combine package
* `int         mass` : The mass point label of the job
* `std::string combine_method`: the Higgs Combine method to invoke
* `std::string addition_options`: additional options for the combine method, defaults to empty string
* `std::string log_file` : path to store the output of the higgs combine package, default to `/dev/null`

All in all this will correspond to the two commands:
```
combine \
      -M <combine_method> \
      -m <mass>           \
      <addition_options>  \
      <card_file>  &> <log_file>
mv <higssraw_output.root> <output_file>
```
Note that this class will make no effort to check the correctness of the any of the input paramters. So the user will be responsible to check that input parameters will generate commands that could successfully run.

## `mgr::HiggsCombineSubmitter` Class
This is the main manager to allow for sub submission. Since the higgs combine method is heavily version dependent, you will need to specify where to pass the command to. So to initialized the submitter class, we will need a json file containing the following information.

```json
{
   "Scram Arch" : "slc6_amd64_gcc491",
   "Store Path" : "<store_path>",
   "CMSSW Version": "CMSSW_7_4_7",
   "Higgs Combine Version" : "v6.2.1"
}
```

During the initialization of the submitter class, it will check if the directory `<store_path>/CMSSW_X_Y_Z/src` exists with the correct verions of the higgs combine package installed. If it fails the test. It will raise an error. For the recommended version of the Higgs Combine Package, check the [official twiki page](https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideHiggsAnalysisCombinedLimit).

After you have initialized a submitter class, you can call the following submitter method

* `std::string MakeScripts( const CombineRequest& )`: generate a script the could be used for submission. The return string will be the path to the generated script (should be under the present working directory).

* `int SubmitDataCard( const CombineRequest& )`: run the generated script, return the execution status of the combine command, and remove the generated script.

* `std::vector<int> SubmitParallel( const vector<CombineRequest>& )`: return a list of generated scripts, wait until all the jobs have completed. and return the results of each submitted job.
