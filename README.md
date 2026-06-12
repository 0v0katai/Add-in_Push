# Add-in Push

This is the reverse-engineered version of [Add-in Push](https://www.planet-casio.com/Fr/programmes/programme4384-1-addin-push-heath-utilitaires-add-ins.html) by [Heath123](https://github.com/Heath123).

## How to use

You have to install the full [fxSDK](https://git.planet-casio.com/Lephenixnoir/fxsdk) toolchain before proceeding to the following steps.

1. Connect your calculator to PC via USB.
2. Build this add-in by `fxsdk build-cg -s`. Use `-lcw` to target fx-CG100.
3. While keeping the USB inserted, execute this add-in in the calculator.
4. Go to your gint add-in source folder and run `fxsdk build-cg-push -s`.

If you want to use Add-in Push with your PrizmSDK project, open its linker script and change `rom (rx) : o = 0x00300000` to `rom (rx) : o = 0x8C200000`. Build your add-in as usual and send the binary directly by `fxlink -sw /path/to/addin.bin`.

## Notes

This project only targets fx-CG50, fx-CG100 with **OS 02.00.X202** and their derivatives. fx-9860 and fx-CG10/20 are not supported due to lacking of sufficient extended RAM region.
