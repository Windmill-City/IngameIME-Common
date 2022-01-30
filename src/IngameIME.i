%module(directors="1") IngameIME

#ifndef __cplusplus
  #error "-c++ must be specified in command line argument to process IngameIME.i"
#endif

%{
#include "IngameIME.hpp"
%}

%include <stdint.i>
%include <std_wstring.i>
%include <std_shared_ptr.i>
%include <std_list.i>

%shared_ptr(IngameIME::Locale)
%shared_ptr(IngameIME::InputProcessor)
%shared_ptr(IngameIME::Composition)
%shared_ptr(IngameIME::InputContext)

%immutable;
extern IngameIME::Global& IngameIME::Global::Instance;

namespace std {
  %template(Strings) list<wstring>;
  %template(InputProcessors) list<shared_ptr<IngameIME::InputProcessor>>;
}

%include "std_function.i"

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
   * @note Extend [A Name] and call Name::ctor with [A Name] to get callback on target language side
   */
  std::function<void(__VA_ARGS__)> setCallback(std::function<void(__VA_ARGS__)> callback) {
    return $self->IngameIME::Name##Holder::setCallback(callback);
  }
};
%enddef

%ICallbackHolder(PreEditRectCallback, IngameIME::Composition, IngameIME::PreEditRect&);
%ICallbackHolder(PreEditCallback, IngameIME::Composition, const IngameIME::CompositionState, const IngameIME::PreEditContext*);
%ICallbackHolder(CommitCallback, IngameIME::Composition, const std::wstring);
%ICallbackHolder(CandidateListCallback, IngameIME::Composition, const IngameIME::CandidateListState, const IngameIME::CandidateListContext*)
%ICallbackHolder(InputProcessorCallback, IngameIME::Global, const IngameIME::InputProcessorState, const IngameIME::InputProcessorContext&);

%include "Composition.hpp"
%include "InputContext.hpp"

%include "InputProcessor.hpp"
%include "IngameIME.hpp"