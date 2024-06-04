#pragma once

#include "array/fastlist.h"
#include <functional>

template<typename eventArgsType, typename returnType = void>
struct eventHandler {
    //https://stackoverflow.com/questions/4295432/typedef-function-pointer
    //functionType will be the name of the typedef
    typedef std::function<returnType(const eventArgsType &)> functionType;
    //typedef returnType(*functionType)(const eventArgsType& eventArgs);
//
    fastList<functionType> eventList = fastList<functionType>();

    //std::function<returnType(eventArgsType)> functionPtr;

    constexpr void invoke(const eventArgsType &eventArgs) const {
        if (!std::is_same<returnType, void>::value) {
            throw "functions with return types should be overloaded";
        }

        for (functionType f: eventList) {
            f(eventArgs);
        }
    }

    template<typename memberFunctionType, typename structType>
    constexpr void unhook(memberFunctionType memberFunction, structType *structToBindTo) {
        functionType f = std::bind(memberFunction, structToBindTo, std::placeholders::_1);
        //f.target_type();
        eventList.erase(std::distance(eventList.begin(), std::find_if(
                                eventList.begin(), eventList.end(),
                                [&f](const functionType &x) {
                                    return x.target_type() == f.target_type();
                                })));
        //auto it = std::find(eventList.begin(), eventList.end(),)
        //if (eventList[0].target_type() == f.target_type()) {
        //    eventList.erase((size_t) 0);
        //}
        //eventList.erase(f);
        eventList.update();
    }

    constexpr void unhook(functionType f) {
        eventList.erase(f);
        eventList.update();
    }

    //member functions
    template<typename memberFunctionType, typename structType>
    constexpr void hook(memberFunctionType memberFunction, structType *structToBindTo) {
        functionType f = std::bind(memberFunction, structToBindTo, std::placeholders::_1);
        eventList.push_back(f);
        eventList.update();
    }

    constexpr void hook(functionType f) {
        eventList.push_back(f);
        eventList.update();
    }


};