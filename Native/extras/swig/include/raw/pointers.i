%include <builtin/cpointer_ns.i>

%define %raw_ptr_wrapper(UNMANAGED_TYPE, MANAGED_TYPE)
%pointer_class_namespace(raw, UNMANAGED_TYPE, MANAGED_TYPE##p)

%extend raw::MANAGED_TYPE##p {
  %proxycode %{
    public static unsafe MANAGED_TYPE* get_raw(MANAGED_TYPE##p wrapped){
      return (MANAGED_TYPE*) wrapped.swigCPtr.Handle;
    }
    public static unsafe MANAGED_TYPE* get_raw($typemap(cstype, UNMANAGED_TYPE*) wrapped){
      return (MANAGED_TYPE*) $typemap(cstype, UNMANAGED_TYPE*).getCPtr(wrapped).Handle;
    }
    public static unsafe $typemap(cstype, UNMANAGED_TYPE*) wrap(MANAGED_TYPE* ptr){
      return new $typemap(cstype, UNMANAGED_TYPE*)(new global::System.IntPtr((void*) ptr), false);
    }
  %}
}
%enddef

%raw_ptr_wrapper(float, float)
%raw_ptr_wrapper(double, double)
%raw_ptr_wrapper(short, short)
%raw_ptr_wrapper(int, int)
%raw_ptr_wrapper(long long, long)
%raw_ptr_wrapper(unsigned char, byte)
%raw_ptr_wrapper(unsigned short, ushort)
%raw_ptr_wrapper(unsigned int, uint)
%raw_ptr_wrapper(unsigned long long, ulong)
