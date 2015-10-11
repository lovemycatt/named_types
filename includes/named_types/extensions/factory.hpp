#pragma once
#include "../named_tuple.hpp"
#include "../rt_named_tuple.hpp"

namespace named_types {
namespace extensions {

namespace __factory_impl {
template <class T, class Base, class ... Args> std::function<Base*(Args...)> make_builder() {
  return ([](Args... args) -> Base* { return new T(std::forward<Args>(args)...); });
}
}  // namespace __factory_impl


template <class BaseClass, class ... T> class factory;
template <class BaseClass, class ... Types, class ... Tags> class factory<BaseClass, Types(Tags)...> {
 public:
  template <class ... BuildArgs> BaseClass* create(std::string const& name, BuildArgs&& ... args) {
    using builder_tuple_type = named_tuple<std::function<BaseClass*(BuildArgs...)>(Tags)...>;
    static builder_tuple_type const builders_ { __factory_impl::make_builder<Types,BaseClass,BuildArgs...>() ... };
    static const_rt_view<builder_tuple_type> const rt_view_(builders_);
    auto builder = rt_view_.template retrieve<std::function<BaseClass*(BuildArgs...)>>(name);
    return builder ? (*builder)(std::forward<BuildArgs>(args)...) : nullptr;
  }
  
};



}  // namespace extension
}  // namespace named_types