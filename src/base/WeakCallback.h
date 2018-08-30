#ifndef WEAKCALLBACK_H
#define WEAKCALLBACK_H

#include <memory>
#include <functional>

template <typename CLASS>
class WeakCallback
{
public:
    WeakCallback(const std::weak_ptr<CLASS>& object,
                 const std::function<void (CLASS*)>& function)
            : _object(object),
              _function(function)
    {

    }

    void operator() ()const
    {
        std::shared_ptr<CLASS> ptr(_object.lock());
        if(ptr)
        {
            _function(ptr.get());
        }
    }

private:
    std::weak_ptr<CLASS> _object;
    std::function<void  (CLASS*)> _function;
};

template <typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const std::shared_ptr<CLASS> & object,
                                     void (CLASS::*function)() )
{
    return WeakCallback<CLASS>(object, function);
}

template <typename CLASS>
WeakCallback<CLASS> makeWeakCallback(const std::shared_ptr<CLASS> & object,
                                     void (CLASS::*function)() const)
{
    return WeakCallback<CLASS>(object, function);
}

#endif