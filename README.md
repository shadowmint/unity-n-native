# n-native

This is a helper package for working with native code.

## Usage

Roughly, define a delegate type matching the C code:

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    internal delegate int plugin_return_int();

Compile and copy the DLL into the Plugins/ folder and configure it, then
use DynamicLibrary to load and execute code from it:

    var lib = new DynamicLibrary("libplugin");
    var return_int = lib.Symbol<plugin_return_int>();
    var val = return_int();

See the tests in the `Editor/` folder for each class for full usage examples.

### Caveats

Currently only a posix loader is implemented; windows support is still a TODO.

In order to pick an implementation (that will actually compile...) you must
define an implementation in:

    Player Settings > Other Settings > Scripting Define Symbols

Notice that you can specify multiple targets like this:

    FOO;BAR;FOO_BAR

Currently supported implementation targets are:

- N_NATIVE_IMPL_POSIX

TODO: Refactor and import the LoadLibrary implementation on windows.

## Install

From your unity project folder:

    npm init
    npm install shadowmint/unity-n-native --save
    echo Assets/packages >> .gitignore
    echo Assets/packages.meta >> .gitignore

The package and all its dependencies will be installed in
your Assets/packages folder.

## Development

Setup and run tests:

    npm install
    npm install ..
    cd test
    npm install
    gulp

Remember that changes made to the test folder are not saved to the package
unless they are copied back into the source folder.

To reinstall the files from the src folder, run `npm install ..` again.

### Building the plugin

See `tests/plugin`; compile the shared library and copy it into the `Plugins`
folder. Note that on OSX the shared library must be renamed `foo.bundle` not
`foo.dylib`.

### Tests

All tests are wrapped in `#if ...` blocks to prevent test spam.

You can enable tests in: Player settings > Other Settings > Scripting Define Symbols

The test key for this package is: N_NATIVE_TESTS
