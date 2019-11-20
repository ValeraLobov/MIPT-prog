#pragma once
#ifndef UTILS_H
#define UTILS_H
#include "typelist.h"

template <typename T, typename U>

class DerivedFrom
{
    using Small = char;
    class  Big { char value[2]; };
    static Small Test(const U&);
    static Big Test(...);
    static T makeT();
public:
    enum  { exists = sizeof(Test(T())) == sizeof(Small), sameType = false };
};

template <typename T>
class DerivedFrom<T, T>
{
public:
    enum { exists = true, sameType = true };
};

#define DERIVEDCLASS(T, U) (DerivedFrom<const U*, const T*>::exists && !DerivedFrom<const T*, const void*>::sameType)

template <bool flag, typename T, typename U>
struct Selection
{
    typedef T result;
};

template <typename T, typename U>
struct Selection<false, T, U> { typedef U result; };

template <typename ...T>
struct IsBase;

template <typename T>
struct IsBase<T, EmptyTypeList> { typedef T result; };

template <typename T, typename head, typename ...tail, typename ...tailOfTypeLists>
struct IsBase <T, TypeList< TypeList<head, tail...>, tailOfTypeLists...> >
{
private:
typedef typename IsBase<T, TypeList<tailOfTypeLists...> >::result Candidate;
public:
    typedef typename Selection<DERIVEDCLASS(typename Candidate::head, head), TypeList<head, tail...>, Candidate>::result result;
};


template <typename T> struct GetBasedItem;

template<>
struct GetBasedItem <EmptyTypeList> { typedef EmptyTypeList result; };

template <typename head, typename ...tail, typename ...tailOfTypeLists>
struct GetBasedItem < TypeList< TypeList<head, tail...>, tailOfTypeLists...> >
{
private:
typedef typename IsBase<TypeList<head, tail...>, TypeList<tailOfTypeLists...> >::result TheIsBase;
typedef typename Replace<TheIsBase, TypeList<head, tail...>, TypeList<tailOfTypeLists...> >::result L;
public:
typedef typename Append<TheIsBase, L>::result result;
};

template<typename ...T> struct Order;

template<>
struct Order < EmptyTypeList > { typedef EmptyTypeList result; };

template <class H, class ...T>
struct Order < TypeList<H, T...> >
{
private:
typedef typename GetBasedItem< TypeList<H, T...> >::result DTF;
public:
typedef typename Append< typename DTF::head, typename Order<typename DTF::tail>::result >::result result;
};

#endif
