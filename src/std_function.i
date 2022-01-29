%{
  #include <functional>
  #include <iostream>

  #ifndef SWIG_DIRECTORS
  #error "Directors must be enabled in your SWIG module for std_function.i to work correctly"
  #endif
%}

// These are the things we actually use
#define param(num,type) $typemap(jstype,type) arg ## num
#define unpack(num,type) arg##num
#define lvalref(num,type) type&& arg##num
#define forward(num,type) std::forward<type>(arg##num)

// This is the mechanics
#define FE_0(...)
#define FE_1(action,a1) action(0,a1)
#define FE_2(action,a1,a2) action(0,a1), action(1,a2)
#define FE_3(action,a1,a2,a3) action(0,a1), action(1,a2), action(2,a3)
#define FE_4(action,a1,a2,a3,a4) action(0,a1), action(1,a2), action(2,a3), action(3,a4)
#define FE_5(action,a1,a2,a3,a4,a5) action(0,a1), action(1,a2), action(2,a3), action(3,a4), action(4,a5)

#define GET_MACRO(_1,_2,_3,_4,_5,NAME,...) NAME
%define FOR_EACH(action,...) 
  GET_MACRO(__VA_ARGS__, FE_5, FE_4, FE_3, FE_2, FE_1, FE_0)(action,__VA_ARGS__)
%enddef

#ifdef SWIGJAVA

%define %std_function(Name, Ret, ...)

%feature("director") A##Name;
%typemap(javaclassmodifiers) A##Name "public abstract class";

%javamethodmodifiers A##Name::call "protected abstract";
%typemap(javaout) Ret A##Name::call ";" // Suppress the body of the abstract method

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

%rename(Name) std::function<Ret(__VA_ARGS__)>;
%rename(call) std::function<Ret(__VA_ARGS__)>::operator();
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
        return new std::function<Ret(__VA_ARGS__)>([&](FOR_EACH(lvalref,__VA_ARGS__)){
              return in.call(FOR_EACH(forward,__VA_ARGS__));
        });
      }
    }
  };
}
%enddef

#elif SWIGCSHARP

%define %std_function(Name, Ret, ...)

%feature("director") A##Name;
%typemap(csmodifiers) A##Name "public abstract class";

%warnfilter(844) A##Name;
%csmethodmodifiers A##Name::call "protected abstract";
%typemap(csout) Ret A##Name::call ";" // Suppress the body of the abstract method

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

%rename(Name) std::function<Ret(__VA_ARGS__)>;
%rename(call) std::function<Ret(__VA_ARGS__)>::operator();
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

#else
  #error "std_function.i not implemented for target language"
#endif