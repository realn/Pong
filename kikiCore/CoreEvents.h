#pragma once

#include <memory>
#include <set>

namespace core {
  template<class _ObserverT> class IEventSource;
  template<class _ObserverT> class IEventTarget;

  template<class _ObserverT>
  class IEventSource {
  public:
    using ObserverT = IEventTarget<_ObserverT>;
    using ObserversT = std::set<ObserverT*>;

  private:
    ObserversT mObservers;

  public:
    IEventSource() = default;
    virtual ~IEventSource();

    void RegisterObserver(ObserverT* observer);
    void UnregisterObserver(ObserverT* observer);

    ObserversT GetObservers() { return mObservers; }
    ObserversT const GetObservers() const { return mObservers; }
  };

  template<class _ObserverT>
  class IEventTarget : 
    public _ObserverT 
  {
  public:
    using SourceT = IEventSource<_ObserverT>;

  private:
    SourceT* mSource = nullptr;

  public:
    IEventTarget() = default;
    virtual ~IEventTarget() { 
      if(mSource) {
        mSource->UnregisterObserver(this); 
      } 
    }

    void SetSource(SourceT* source) {
      mSource = source;
    }
  };

  template<class _ObserverT>
  IEventSource<_ObserverT>::~IEventSource() {
    for(auto observer : mObservers) {
      observer->SetSource(nullptr);
    }
  }

  template<class _ObserverT>
  void IEventSource<_ObserverT>::RegisterObserver(typename IEventSource<_ObserverT>::ObserverT* observer) {
    mObservers.insert(observer);
  }

  template<class _ObserverT>
  void IEventSource<_ObserverT>::UnregisterObserver(typename IEventSource<_ObserverT>::ObserverT* observer) {
    auto it = mObservers.find(observer);
    mObservers.erase(it);
  }

  template<typename _Type>
  void bind(typename IEventSource<_Type>& source, 
            typename IEventTarget<_Type>& target) {
    target.SetSource(&source);
    source.RegisterObserver(&target);
  }
}
