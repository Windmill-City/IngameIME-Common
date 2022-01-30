%{
  #include <functional>
  #include <iostream>

  #ifndef SWIG_DIRECTORS
  #error "Directors must be enabled in your SWIG module for std_function.i to work correctly"
  #endif
%}

%define %std_function(Name, Ret, ...)

%feature("director") A##Name;
#if defined(SWIGJAVA)

%typemap(javaclassmodifiers) A##Name "public abstract class";

%javamethodmodifiers A##Name::call "protected abstract";
%typemap(javaout) Ret A##Name::call ";" // Suppress the body of the abstract method

#elif defined(SWIGCSHARP)

%typemap(csmodifiers) A##Name "public abstract class";

%warnfilter(844) A##Name;
%csmethodmodifiers A##Name::call "protected abstract";
%typemap(csout) Ret A##Name::call ";" // Suppress the body of the abstract method

#elif defined(SWIGPYTHON)
// Nothing here
#else
  #warning "std_function.i not implemented for target language"
#endif

%inline %{
  struct A##Name {
    virtual ~A##Name() {}
    /**
     * @brief Run the callback
     *
     * @return Ret
     */
    virtual Ret call(__VA_ARGS__) = 0;
  };
%}

#if defined(SWIGJAVA)

/**
 * @brief Should extend the director class but not the proxy class
 */
%typemap(javaclassmodifiers) std::function<Ret(__VA_ARGS__)> "public final class";

/**
 * @brief Function pointer from java side maybe null
 */
%typemap(in) std::function<Ret(__VA_ARGS__)>
%{ if ($input) $1 = *($&1_ltype)$input; %}
/**
 * @brief If the wrapped function pointer is null, just return null to java side
 */
%typemap(out) std::function<Ret(__VA_ARGS__)>
%{ *($&1_ltype*)&$result = $1 ? new $1_ltype($1) : 0; %}
%typemap(javaout) std::function<Ret(__VA_ARGS__)> {
  long cPtr = $jnicall;
  return (cPtr == 0) ? null : new $typemap(jstype, std::function<Ret(__VA_ARGS__)>)(cPtr, true);
}

%typemap(javabody) std::function<Ret(__VA_ARGS__)> %{
  private transient long swigCPtr;
  private transient boolean swigCMemOwn;

  $javaclassname(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  static long getCPtr($javaclassname obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }
%}

#elif defined(SWIGCSHARP)

/**
 * @brief Should extend the director class but not the proxy class
 */
%typemap(csclassmodifiers) std::function<Ret(__VA_ARGS__)> "public sealed class";

/**
 * @brief Function pointer from csharp side maybe null
 */
%typemap(in) std::function<Ret(__VA_ARGS__)>
%{ if ($input) $1 = *($&1_ltype)$input; %}
/**
 * @brief If the wrapped function pointer is null, just return null to csharp side
 */
%typemap(out) std::function<Ret(__VA_ARGS__)>
%{ *($&1_ltype*)&$result = $1 ? new $1_ltype($1) : 0; %}
%typemap(csout, excode=SWIGEXCODE) std::function<Ret(__VA_ARGS__)> {
    global::System.IntPtr cPtr = $imcall;
    $typemap(cstype, std::function<Ret(__VA_ARGS__)>) ret = (cPtr == global::System.IntPtr.Zero) ? null : new $typemap(cstype, std::function<Ret(__VA_ARGS__)>)(cPtr, true);$excode
    return ret;
}

%typemap(csbody) std::function<Ret(__VA_ARGS__)> %{
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  private bool swigCMemOwnBase;

  $csclassname(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwnBase = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  static global::System.Runtime.InteropServices.HandleRef getCPtr($csclassname obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }
%}

#elif defined(SWIGPYTHON)

/**
 * @brief Function pointer from python side maybe None
 */
%typemap(in) std::function<Ret(__VA_ARGS__)> (void *argp, int res = 0) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(std::function<Ret(__VA_ARGS__)> *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum);
  }
  if (argp) $1 = *(%reinterpret_cast(argp, $&ltype));
  if (newmem & SWIG_CAST_NEW_MEMORY) delete %reinterpret_cast(argp, $&ltype);
}
/**
 * @brief If the wrapped function pointer is null, just return None to python side
 */
%typemap(out) std::function<Ret(__VA_ARGS__)> {
  std::function<Ret(__VA_ARGS__)> *funcresult = $1 ? new std::function<Ret(__VA_ARGS__)>($1) : 0;
  %set_output(SWIG_NewPointerObj(%as_voidptr(funcresult), $descriptor(std::function<Ret(__VA_ARGS__)> *), SWIG_POINTER_OWN));
}
%typemap(varout) std::function<Ret(__VA_ARGS__)> {
  std::function<Ret(__VA_ARGS__)> funcresult = $1 ? new std::function<Ret(__VA_ARGS__)>($1) : 0;
  %set_varoutput(SWIG_NewPointerObj(%as_voidptr(funcresult), $descriptor(std::function<Ret(__VA_ARGS__)> *), SWIG_POINTER_OWN));
}

/**
 * @brief Typecheck for overloading
 */
%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER, noblock=1) std::function<Ret(__VA_ARGS__)> {
  int res = SWIG_ConvertPtr($input, 0, $descriptor(std::function<Ret(__VA_ARGS__)> *), 0);
  $1 = SWIG_CheckState(res);
}

#else
  #warning "std_function.i not implemented for target language"
#endif

%feature("novaluewrapper") std::function<Ret(__VA_ARGS__)>;
%rename(Name) std::function<Ret(__VA_ARGS__)>;

#if defined(SWIGPYTHON)
%rename(__call__) std::function<Ret(__VA_ARGS__)>::operator();
#else
%rename(call) std::function<Ret(__VA_ARGS__)>::operator();
#endif

namespace std {
  %nodefaultctor;
  struct function<Ret(__VA_ARGS__)> {
    // Copy constructor
    function<Ret(__VA_ARGS__)>(const std::function<Ret(__VA_ARGS__)>&);

    // Call operator
    Ret operator()(__VA_ARGS__) const;

    // Director
    %extend {
      function<Ret(__VA_ARGS__)>(A##Name& in) {
        return new std::function<Ret(__VA_ARGS__)>([&](auto&& ...param){
          return in.call(std::forward<decltype(param)>(param)...);
        });
      }
    }
  };
}
%enddef