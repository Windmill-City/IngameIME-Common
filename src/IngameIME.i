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
extern IngameIME::Global& IngameIME::global;

namespace std {
  %template(Strings) list<wstring>;
  %template(InputProcessors) list<shared_ptr<IngameIME::InputProcessor>>;
}

%include "std_function.i"
%include <swiginterface.i>

%define %ICallbackHolder(Name, ...)
%std_function(Name, void, __VA_ARGS__);
%interface_custom("%sProxy", "I%s", IngameIME::ICallbackHolder<__VA_ARGS__>);
namespace IngameIME {
  %rename(Name##Holder) ICallbackHolder<__VA_ARGS__>;
  %rename(set##Name) ICallbackHolder<__VA_ARGS__>::setCallback;
  class ICallbackHolder<__VA_ARGS__> {
  public:
    /**
     * @brief Set callback
     *
     * @param callback callback to set, nullable
     * @return previous callback, nullable
     */
    std::function<void(__VA_ARGS__)> setCallback(std::function<void(__VA_ARGS__)> callback);
  };
}
%enddef

%define %ICallbackHolder_shared_ptr(Name, ...)
%shared_ptr(IngameIME::ICallbackHolder<__VA_ARGS__>);
%ICallbackHolder(Name, __VA_ARGS__);
%enddef

%ICallbackHolder_shared_ptr(PreEditRectCallback, IngameIME::PreEditRect&);
%ICallbackHolder_shared_ptr(PreEditCallback, const IngameIME::CompositionState, const IngameIME::PreEditContext*);
%ICallbackHolder_shared_ptr(CommitCallback, const std::wstring);
%ICallbackHolder_shared_ptr(CandidateListCallback, const IngameIME::CandidateListState, const IngameIME::CandidateListContext*)
%ICallbackHolder(InputProcessorCallback, const IngameIME::InputProcessorState, const IngameIME::InputProcessorContext&);

%include "Composition.hpp"
%include "InputContext.hpp"

%include "InputProcessor.hpp"
%include "IngameIME.hpp"