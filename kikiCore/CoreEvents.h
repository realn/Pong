#pragma once

#include <memory>
#include <set>

namespace core {
  template<class _ObserverT> class IEventSource;
  template<class _ObserverT> class IEventTarget;

  template<class _ObserverT>
  class IEventSource {
  public:
    using TargetT = IEventTarget<_ObserverT>;
    using TargetsT = std::set<TargetT*>;

  private:
    TargetsT mTargets;

  public:
    IEventSource() = default;
    virtual ~IEventSource();

    void RegisterTarget(TargetT* target);
    void UnregisterTarget(TargetT* target);

    TargetsT GetObservers() { return mTargets; }
    TargetsT const GetObservers() const { return mTargets; }
  };

  template<class _ObserverT>
  class IEventTarget : 
    public _ObserverT 
  {
  public:
    using SourceT = IEventSource<_ObserverT>;
    using SourcesT = std::set<SourceT*>;

  private:
    SourcesT mSources;

  public:
    IEventTarget() = default;
    virtual ~IEventTarget();

    void RegisterSource(SourceT* source);
    void UnregisterSource(SourceT* source);
  };

  template<class _ObserverT>
  IEventSource<_ObserverT>::~IEventSource() {
    for(auto& target : mTargets) {
      target->UnregisterSource(this);
    }
  }

  template<class _ObserverT>
  void IEventSource<_ObserverT>::RegisterTarget(typename IEventSource<_ObserverT>::TargetT* target) {
    mTargets.insert(target);
  }

  template<class _ObserverT>
  void IEventSource<_ObserverT>::UnregisterTarget(typename IEventSource<_ObserverT>::TargetT* target) {
    auto it = mTargets.find(target);
    mTargets.erase(it);
  }

  template<typename _ObserverT>
  IEventTarget<_ObserverT>::~IEventTarget() {
    for(auto source : mSources) {
      source->UnregisterTarget(this);
    }
  }

  template<typename _ObserverT>
  void IEventTarget<_ObserverT>::RegisterSource(typename IEventTarget<_ObserverT>::SourceT* source) {
    mSources.insert(source);
  }

  template<typename _ObserverT>
  void IEventTarget<_ObserverT>::UnregisterSource(typename IEventTarget<_ObserverT>::SourceT* source) {
    auto it = mSources.find(source);
    mSources.erase(it);
  }

  template<typename _Type>
  void bind(typename IEventSource<_Type>& source, 
            typename IEventTarget<_Type>& target) {
    target.RegisterSource(&source);
    source.RegisterTarget(&target);
  }
}
