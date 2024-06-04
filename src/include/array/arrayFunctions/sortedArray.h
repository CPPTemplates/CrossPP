#include <vector>
template<typename listType, typename t>
constexpr bool addWithoutDupes(listType &list, const t &value) {
    if (!arrayContains(list, value)) {
        list.push_back(value);
        return true;
    }
    return false;
}

template<typename listType, typename t>
inline bool insertSortedWithoutDoubles(listType &list, const t &value) {
    static_assert(!std::is_pointer<listType>::value, "pointers are not allowed");
    const auto index = std::lower_bound(list.begin(), list.end(), value);
    //insert sorted
    if ((index != list.end()) && (*index == value)) {
        return false;
    } else {
        list.insert(index, value);
        return true;
    }
}

//CAUTION! THIS FUNCTION IS VERY SLOW! PLEASE JUST SORT() AND UNIQUE() THE LIST AFTERWARDS
template<typename listType, typename t, typename compareFunction = std::less<t>>
inline bool insertSortedWithoutDoubles(listType &list, const t &value, compareFunction function) {
    static_assert(!std::is_pointer<listType>::value, "pointers are not allowed");
    const auto index = std::lower_bound(list.begin(), list.end(), value, function);
    //insert sorted
    if ((index != list.end()) && (*index == value)) {
        return false;
    } else {
        list.insert(index, value);
        return true;
    }
}

//returns indexes sorted from less to more
template<typename T>
std::vector<size_t> sort_indexes(const std::vector<T> &v) {

    // initialize original index locations
    std::vector<size_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values
    std::stable_sort(idx.begin(), idx.end(),
                [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });

    return idx;
}
/// @brief 
/// @tparam t 
/// @param amount 
/// @param capacities the amount that each array member is able to hold
/// @return 
template<typename t = size_t>
std::vector<t> divideOverArray(t& amount, const std::vector<t>& capacities){
    //first step, sort capacities
    const std::vector<size_t>& sortedIndexes = sort_indexes(capacities);
    //sortedindexes contains the indexes of the capacities sorted from less to more
    std::vector<t> divideAmounts = std::vector<t>(capacities.size());
    fp slotsLeft = (fp)sortedIndexes.size();
    for(csize_t& sortedIndex : sortedIndexes){
        t amountToPut = math::minimum(math::ceil<t>((fp)amount / slotsLeft), capacities[sortedIndex]) ;
        divideAmounts[sortedIndex] = amountToPut;
        amount -= amountToPut;
        slotsLeft--;
    }
    return divideAmounts;
}
