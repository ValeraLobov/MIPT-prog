#ifndef TYPELIST_H
#define TYPELIST_H
#include <iostream>
#include <typeinfo>
#include <climits>
#include "utils.h"

struct Void {};

template<typename ...Args>
struct TypeList
{
public:
    typedef Void head;
    typedef Void tail;
};
typedef TypeList<> EmptyTypeList;
template<typename H, typename ...T>
struct TypeList<H, T...>
{
public:
    typedef H head;
    typedef TypeList<T...> tail;
};

template<typename TL>
struct IsEmpty :
        std::true_type
{
};
template<>
struct IsEmpty<TypeList<Void, Void>> :
        std::true_type
{
};

template<typename ...Args>
struct IsEmpty<TypeList<Args...>> :
        std::integral_constant<bool,
                std::is_same<typename TypeList<Args...>::head, Void>::value &&
                IsEmpty<typename TypeList<Args...>::tail>::value>
{
};

template<typename T, typename TL>
struct Contains :
        std::false_type
{};

template<typename ...Args>
struct Contains< Void, TypeList<Args...> > :
        std::false_type
{};

template<typename T, typename ...Args>
struct Contains< T, TypeList<Args...> > :
        std::integral_constant<bool,
                std::is_same<typename TypeList<Args...>::head, T>::value ||
                Contains<T, typename TypeList<Args...>::tail>::value>
{};

template<typename TL>
struct Length :
        std::integral_constant<unsigned int, 0>
{
};
template<typename ...Args>
struct Length<TypeList<Args...>> :
        std::integral_constant<unsigned int,
                IsEmpty<TypeList<Args...>>::value
                ? 0
                : 1 + Length<typename TypeList<Args...>::tail>::value>
{};


template<unsigned int N, typename TL>
struct TypeAt
{
    typedef Void type;
};
template<typename ...Args>
struct TypeAt< UINT_MAX - 1, TypeList<Args...> >
{
    typedef Void type;
};

template<typename ...Args>
struct TypeAt< 0, TypeList<Args...> >
{
    typedef typename TypeList<Args...>::head type;
};
template<unsigned int N, typename ...Args>
struct TypeAt< N, TypeList<Args...> >
{
    static_assert(N < Length< TypeList<Args...> >::value, "N is too big");

    typedef typename TypeAt<N - 1, typename TypeList<Args...>::tail>::type type;
};


struct Constants
{
    typedef std::integral_constant<unsigned int, UINT_MAX> position;
};

std::ostream& operator<<(std::ostream& ostr, EmptyTypeList)
{
    ostr << "{}";
    return ostr;
}

template<typename TL>
void PrintTypeListHelper(TL, std::ostream& ostr)
{
}
template<typename T>
void PrintTypeListhead(T, std::ostream& ostr)
{
    ostr << typeid(T).name();
}
template<typename ...Args>
void PrintTypeListhead(TypeList<Args...> tl, std::ostream& ostr)
{
    ostr << tl;
}
template<typename head, typename ...Args>
void PrintTypeListHelper(TypeList<head, Args...>, std::ostream& ostr)
{
    PrintTypeListhead(head(), ostr);
}

template<typename ...Args>
std::ostream& operator<<(std::ostream& ostr, TypeList<Args...> tl)
{
    ostr << '{';
    PrintTypeListHelper(tl, ostr);
    ostr << '}';
    return ostr;
}


template<typename TOrTL2, typename TL>
struct Append
{};

template<typename T, typename ...Args>
struct Append<T, TypeList<Args...>>
{
    typedef TypeList<Args..., T> result;
};

template<typename ...Args1, typename ...Args2, typename ...Args3, typename ...Args4>
struct Append<TypeList< TypeList<Args1...>, Args2... >, TypeList< TypeList<Args3...>, Args4... > >
{
    typedef TypeList<TypeList<Args1...>, Args2..., TypeList<Args3...>, Args4... > result;
};

template <typename T, typename U, typename TList> struct Replace;

template  <typename T, typename U>
struct Replace<T, U, EmptyTypeList>
{
    typedef EmptyTypeList result;
};

template <typename T, typename U, typename ...tail>
struct Replace<T, U, TypeList<T, tail...> > { typedef TypeList<U, tail...> result; };

template <typename T, typename U, typename head, typename ...tail>
struct Replace<T, U, TypeList<head, tail...> >
{
    typedef typename Append<head, typename Replace< T, U, TypeList<tail...> >::result>::result result;
};

template<typename T, typename U>
class Equals
{};
template<typename T, typename ...U>
class Equals< T, TypeList<U...> >
{
public:
    typedef typename std::integral_constant<bool,
            std::is_same<T, typename TypeList<U...>::head>::value != 0> result;
};

template<typename T, unsigned int IndexFrom, typename TL>
struct FindHelper : std::integral_constant<unsigned int, 0>
{};

template<typename T, unsigned int IndexFrom>
struct FindHelper<T, IndexFrom, EmptyTypeList> : std::integral_constant<unsigned int, 0>
{};

template<typename T, unsigned int IndexFrom, typename ...Args>
struct FindHelper<T, IndexFrom, TypeList<Args...>> :
        std::integral_constant<unsigned int, std::is_same<typename TypeList<Args...>::head, T>::value
                                             ? IndexFrom : IndexFrom + 1 + FindHelper<T, IndexFrom, typename TypeList<Args...>::tail>::value>
{};

template<typename T, typename TL>
struct Find
{};

template<>
struct Find<Void, EmptyTypeList> : std::integral_constant<unsigned int, UINT_MAX - 1>
{};

template<typename T>
struct Find<T, EmptyTypeList> : Constants::position
{};

template<typename ...Args>
struct Find< Void, TypeList<Args...> > : Constants::position
{};

template<typename T, typename ...Args>
struct Find< T, TypeList<Args...> > :
        std::integral_constant<unsigned int,
                Contains< T, TypeList<Args...> >::value
                ? FindHelper< T, 0, TypeList<Args...> >::value
                : Constants::position::value>
{};



template<typename T, unsigned int IndexFrom, typename TL>
struct FindTypeListHelper : std::integral_constant<unsigned int, 0>
{};


template<typename T, unsigned int IndexFrom, typename ...head, typename ...tail>
struct FindTypeListHelper< T, IndexFrom, TypeList<TypeList<head...>, tail...> > :
        std::integral_constant< unsigned int, Equals< T, TypeList<head...> >::result::value
                                              ? IndexFrom
                                              : IndexFrom + 1 + FindTypeListHelper<T, IndexFrom, TypeList<tail...>>::value>
{};

template<typename T, typename TL>
struct FindTypeList
{};

template<typename T>
struct FindTypeList<T, EmptyTypeList> :
        Constants::position
{};

template<typename ...Args>
struct FindTypeList< EmptyTypeList, TypeList<Args...> > :
        Constants::position
{};

template<typename T, typename ...head, typename ...tail>
struct FindTypeList< T, TypeList<TypeList<head...>, tail...> > :
        std::integral_constant<unsigned int, FindTypeListHelper< T, 0, TypeList< TypeList<head...>, tail...> >::value>
{};

#endif

