# BUTTON experiment for RAT-PAC II 

This is the RAT-PAC II experiment repository for BUTTON (**B**oulby **U**nderground **T**echnical **T**estbed **O**bserving **N**eutrinos). It is intended to be a complimentary package for RAT-PAC II so that simulations can be perfomed with the BUTTON detector system. 

If interested in make your own experiment for RAT-PAC II click [here](https://github.com/rat-pac/RatpacExperiment).

**SIMULATION SOFTWARE NEEDS TO BE INSTALLED SEPARETELY (SEE BELOW)**

## Dependencies

In order to perform BUTTON simulations in RAT-PAC II, the following software much be installed beforehand:
- [RAT-PAC II](https://github.com/rat-pac/ratpac-two)

Which requires:
- [ROOT 6.25+](https://root.cern.ch)
- [Geant4 11.0+](https://geant4.web.cern.ch/)
- [cmake 3.22+](https://cmake.org/)

The most well-supported way of installing RAT-PAC II is via the [ratpac-setup script](https://github.com/rat-pac/ratpac-setup) which will install all the above software. It produces an environment script `env.sh` which sources both `ratpac.sh` and `geant4.sh`. This needs to be sourced prior to installing this experiment.

## Installation

Simply clone this repository and run the script `./rename.sh button`. 
Then compile to produce the rat database and source file that will be named `button.sh`.

```
git clone git@github.com:LSexton2396/BUTTON-RAT2.git
cd BUTTON-RAT2/
./rename.sh button
make
```

## Using the experiment

Before running a simulation ensure that both `ratpac.sh` and `geant4.sh` (or `env.sh`) are sourced before sourcing the experiment `button.sh`.

To use the BUTTON rat database, use the command `button` instead of `rat` when running your macros.

A good test to see if everything is working is to run `vis_qt_button.mac` which will visualise BUTTON. It is found in the macros folder.

```
button macros/vis_qt_button.mac --vis
```

This should bring up the follwing window:
![BUTTON visual example](https://github.com/LSexton2396/BUTTONexperiment/blob/main/BUTTON_visual_example.png?)




