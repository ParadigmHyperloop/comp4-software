#include "States.h"


std::unique_ptr<PodState> PodState::createState(PodStates newState) {
    switch (newState) {
        case psBooting:
            return std::unique_ptr<PodState>(new Booting());
        case psStandby:
            return std::unique_ptr<PodState>(new Booting());
    }
}

unsigned int PodState::getTimeInStateMilis(){
    std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - this->_enterStateTime).count();
}