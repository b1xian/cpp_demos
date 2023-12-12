#ifndef VISITOR_NEW_H
#define VISITOR_NEW_H

template<typename... Types>
struct VisitorNew;

template<typename T, typename... Types>
struct VisitorNew<T, Types...> : VisitorNew<Types...> {
    using VisitorNew<Types...>::Visit;
    virtual void Visit(const T&) = 0;
};

template<typename T>
struct VisitorNew<T> {
    virtual void Visit(const T&) = 0;
};
#endif