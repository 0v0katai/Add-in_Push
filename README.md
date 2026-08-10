# Add-in Push

This is the reverse-engineered version of [Add-in Push](https://www.planet-casio.com/Fr/programmes/programme4384-1-addin-push-heath-utilitaires-add-ins.html) by [Heath123](https://github.com/Heath123).

## How to use

You have to install the full [fxSDK](https://git.planet-casio.com/Lephenixnoir/fxsdk) toolchain before proceeding to the following steps.

1. Connect your calculator to PC via USB.
2. Build this add-in by `fxsdk build-cg -s`.
3. While keeping the USB inserted, execute this add-in in the calculator.
4. Go to your gint add-in source folder and run `fxsdk build-cg-push -s`.

If you want to use Add-in Push with your PrizmSDK project, build your add-in as usual and send the binary directly by `fxlink -sw /path/to/addin.bin`.

## Notes

This project only targets fx-CG50. Support for building against fx-CG100 is removed as mpm.bin now integrates Add-in Push. fx-9860 and fx-CG10/20 are not supported due to lacking of sufficient extended RAM region.
