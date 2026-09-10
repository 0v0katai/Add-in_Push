# Add-in Push

Add-in Push is a utility for CASIO graphing calculators that loads an add-in into the RAM with USB, without the slow process of saving it to the calculator's storage memory.

This utility consists of 2 components: the CLI and the helper add-in. The CLI is adapted from [fxlink's push.c](https://git.planet-casio.com/Lephenixnoir/fxsdk/src/commit/a6e46eb/fxlink/modes/push.c) and targets both Linux and Windows platforms. The helper add-in is based on the [add-in of the same name](https://www.planet-casio.com/Fr/programmes/programme4384-1-addin-push-heath-utilitaires-add-ins.html) by [Heath123](https://github.com/Heath123), with the virtual launch address support and proper USB termination fix.

## How to build

### Building CLI

Install the [prerequisite packages](https://next.cahute.org/guides/build/linux.html) for building Cahute in Linux, and the MinGW toolchain if you want to build against Windows. Then, run the bash script to build the CLI:

```
bash ./cli/build.sh <linux|win32|win64>
```

### Building helper add-in

1. Install the full [fxSDK](https://git.planet-casio.com/Lephenixnoir/fxsdk) toolchain.
2. Connect your calculator to PC via USB.
3. Build this add-in by `fxsdk build-cg -s`.

## How to use

1. While keeping the USB inserted, execute the helper add-in in the calculator.
2. Open the CLI executable. Drag and drop either G3A add-in or binary file.
3. Press Enter key to confirm.

## Note

The helper add-in only targets fx-CG50. Support for building against fx-CG100 is removed as [mpm.bin](https://git.planet-casio.com/PlaneteCasio/mpm) now integrates Add-in Push. Currently there is no plan for the fx-9860 and fx-CG10/20 support due to lacking of sufficient RAM capacity in these platforms.

## License

The CLI program is licensed under [GPLv3](cli/LICENSE.md), and the helper add-in is licensed under [MIT License](LICENSE.md).
