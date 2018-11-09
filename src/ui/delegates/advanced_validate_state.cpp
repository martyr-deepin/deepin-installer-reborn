#include "ui/delegates/advanced_validate_state.h"

QDebug operator<< (QDebug debug, const installer::AdvancedValidateState &state) {
    debug << state;
    return debug;
}
