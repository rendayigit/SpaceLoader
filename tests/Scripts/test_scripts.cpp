#include "../../lib/Logger/logger.h"
#include "../Test_common.h"

TEST(Scripts, build) {
    log()->Info("logString");
    log()->Flush();
}