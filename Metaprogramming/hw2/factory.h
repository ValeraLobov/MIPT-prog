#ifndef FACTORY_H
#define FACTORY_H
#include <iostream>
#include <typeinfo>
#include <climits>
#include "utils.h"
#include "typelist.h"

template<typename Base, typename T, typename Root>
class Factory;
template<typename Base, typename ...T, typename ...Root>
class Factory< Base, TypeList<T...>, TypeList<Root...> > : public Base
{
public:
    template <class U>
    auto Get()
    { return new typename TypeAt< Find< U, TypeList<Root...> >::value, TypeList<T...> >::type; }
};

template <typename ...T>
struct BaseProduct;
template <typename T>
struct BaseProduct<T, EmptyTypeList> { typedef Void result; };
template <typename T, typename head, typename ...tail, typename ...tailOfTypeLists>
struct BaseProduct <T, TypeList< TypeList<head, tail...>, tailOfTypeLists...> >
{
public:
    typedef typename Selection<DERIVEDCLASS(head, typename T::head), TypeList<head, tail...>,
            typename BaseProduct<T, TypeList<tailOfTypeLists...> >::result >::result result;
};

template <typename Root, typename T>
class GetFactory;

template <typename Root>
class GetFactory<Root, EmptyTypeList>
{
public:
    typedef EmptyTypeList result;
};
template <typename Root, typename Typehead, typename ...Typetail, typename ...OtherTypes>
class GetFactory< Root, TypeList<TypeList<Typehead, Typetail...>, OtherTypes...> >
{
private:
    typedef typename BaseProduct<TypeList<Typehead, Typetail...>, TypeList<OtherTypes...> >::result TheBaseProduct;
    typedef typename GetFactory< Root, TypeList<OtherTypes...> >::result Factories;
    typedef typename TypeAt< Find< TheBaseProduct, TypeList<OtherTypes...> >::value, Factories >::type Base;
public:
    typedef typename Append<Factory<Base, TypeList<Typehead, Typetail...>, Root>, Factories>::result result;
};

template<typename T>
class GetAbstractFactory
{
private:
    typedef typename Order<T>::result OrderedList;
    typedef typename TypeAt< Length<OrderedList>::value-1, OrderedList >::type Root;
    typedef typename GetFactory<Root, OrderedList>::result Factories;
public:
    template <typename U>
    auto GetConcreteFactory()
    { return new typename TypeAt< FindTypeList< U, OrderedList >::value, Factories >::type; }
};

#endif
