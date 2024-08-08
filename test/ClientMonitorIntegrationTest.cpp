#include "structs.h"
#include <gtest/gtest.h>
#include <Client.h>
#include <Monitor.h>



TEST(ClientMonitorIntegration, TestSetandGetFocus) {

    Wind::Monitor(Wind::Dimensions(1980, 200), Wind::Position(10,10),9);

    Wind::Client c(100);
}
