#ifndef FLIGHTCOMPUTER_COMPARINGHELPERS_H
#define FLIGHTCOMPUTER_COMPARINGHELPERS_H

template <class t>
bool inThreshold(t value,t ideal , t threshold){
    t lower = ideal - threshold;
    t upper = ideal + threshold;
    return lower < value && value < upper;
}

template <class t>
bool inRange(t value,t min , t max){
    return min < value && value < max;
}

#endif //FLIGHTCOMPUTER_COMPARINGHELPERS_H
