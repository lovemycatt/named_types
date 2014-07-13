#ifndef NAMED_TUPLES_TYPE_TRAITS_HEADER
#define NAMED_TUPLES_TYPE_TRAITS_HEADER

#include <type_traits>

// This file contains traits agnostic about the named tuple implementation
// thus the dedicated header

namespace named_tuples {
// Just forward declared, does not need to be instantiated for it is just used for traits
template <typename ... T> struct type_list;

// Size of a type list
template <typename ... T> struct size_of;
template <typename ... Types> struct size_of<type_list<Types...>> {
  inline constexpr operator std::size_t () const { return sizeof ... (Types); } 
};

// Does a type list contains a given type
template <typename ... T> struct contains;
template <typename Head, typename ... Tail, typename Id> struct contains<type_list<Head, Tail...>, Id> {
  inline constexpr operator bool () const { return (std::is_same<Id, Head>() ? true : contains<type_list<Tail...>, Id>()); } 
};
template <typename Id> struct contains<type_list<>, Id> {
  inline constexpr operator bool () const { return false; } 
};

// Does a type_list have duplicates
template <typename ... T> struct has_duplicates;
template <typename Head, typename ... Tail> struct has_duplicates<type_list<Head, Tail...>> {
  inline constexpr operator bool () const { return (contains<type_list<Tail...>, Head>() || has_duplicates<type_list<Tail...>>()); } 
};
template <> struct has_duplicates<type_list<>> {
  inline constexpr operator bool () const { return false; } 
};

// Index of a type in a list
template <typename ... T> struct index_of;
template <typename ... Head, typename Current, typename ... Tail, typename Id> struct index_of<type_list<Head ...>, type_list<Current, Tail...>, Id> {
  static constexpr std::size_t value = (std::is_same<Id, Current>() ? sizeof ... (Head) : index_of<type_list<Head..., Current>, type_list<Tail...>, Id>()); 
  inline constexpr operator std::size_t () const { return value; } 
};
template <typename ... Types, typename Id> struct index_of<type_list<Types...>, type_list<>, Id> {
  static constexpr std::size_t value =  sizeof ... (Types);
  inline constexpr operator std::size_t () const { return value; }
};
template <typename Id> struct index_of<type_list<>, Id> {
  static constexpr std::size_t value = 0;
  inline constexpr operator std::size_t () const { return value; }
};
template <typename ... Types, typename Id> struct index_of<type_list<Types ...>, Id> : index_of<type_list<>, type_list<Types...>, Id> {
  static_assert(contains<type_list<Types...>, Id>(), "The type_list must contain the required type Id to compute the index.");
};

}  // namespace named_tuples

#endif  // NAMED_TUPLES_TYPE_TRAITS_HEADER
