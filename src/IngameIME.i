%module(directors="1") IngameIME

#ifndef __cplusplus
  #error "-c++ must be specified in command line argument to process IngameIME.i"
#endif

%{
#include "IngameIME.hpp"
%}

%include <stdint.i>
%include <std_string.i>
%include <std_function.i>

%define %ICallbackHolder(Name, Class, ...)
%std_function(Name, void, __VA_ARGS__);
%warnfilter(401) Class;
%extend Class {
public:
  /**
   * @brief Set callback
   *
   * @param callback callback to set, nullable
   * @return previous callback, nullable
   */
  std::function<void(__VA_ARGS__)> setCallback(std::function<void(__VA_ARGS__)> callback) {
    return $self->IngameIME::Name##Holder::setCallback(callback);
  }
};
%enddef

%ICallbackHolder(PreEditCallback, IngameIME::InputContext, const IngameIME::CompositionState, const IngameIME::PreEditContext*);
%ICallbackHolder(CommitCallback, IngameIME::InputContext, const std::string);
%ICallbackHolder(CandidateListCallback, IngameIME::InputContext, const IngameIME::CandidateListState, const IngameIME::CandidateListContext*)
%ICallbackHolder(InputModeCallback, IngameIME::InputContext, const IngameIME::InputMode)

%include <exception.i>
%exception {
  try {
    $action
  }
  catch (const std::bad_alloc& e) {
    SWIG_exception(SWIG_MemoryError, e.what());
  }
  catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%include <std_list.i>
%template(string_list) std::list<std::string>;

%immutable IngameIME::PreEditContext::selStart;
%immutable IngameIME::PreEditContext::selEnd;
%immutable IngameIME::PreEditContext::content;
%immutable IngameIME::CandidateListContext::selection;
%immutable IngameIME::CandidateListContext::candidates;
%nodefaultctor IngameIME::PreEditContext;
%nodefaultctor IngameIME::CandidateListContext;

%typemap(jtype)  HWND "long"
%typemap(jstype) HWND "long"
%typemap(jni)    HWND "jlong"
%typemap(javain) HWND "$javainput"
%typemap(in) HWND %{
  $1 = (HWND)$input; 
%}

%include "../include/IngameIME.hpp"