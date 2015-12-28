#if N_NATIVE_TESTS
  public class DynamicLibraryTests : N.Tests.Test {

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    internal delegate int rs_trigger(int value);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    internal delegate int rs_trigger_ADFASF(int value);

    public void test_load_and_bind_dynamic_library() {
      var lib = new DynamicLibrary("libextern");
      var trigger = lib.symbol<rs_trigger>();
      var val = trigger(100);
      this.Assert(val == 200);
    }

    public void test_invalid_library() {
      try {
        new DynamicLibrary("libextern_ADFADSFASF");
        this.Assert(false);
      }
      catch(DynamicLibraryException e) {
        this.Assert(e.code == DynamicLibraryError.INVALID_LIBRARY);
      }
    }

    public void test_invalid_symbol() {
      try {
        var lib = new DynamicLibrary("libextern");
        lib.symbol<rs_trigger_ADFASF>();
        this.Assert(false);
      }
      catch(DynamicLibraryException e) {
        this.Assert(e.code == DynamicLibraryError.INVALID_SYMBOL);
      }
    }
  }
#endif
