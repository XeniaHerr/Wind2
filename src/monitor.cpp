#include <Monitor.h>
#include <ios>
#include <sys/types.h>


using namespace Wind;


Monitor::Monitor(u_int32_t x, u_int32_t y) : realDimensions(x,y) {

                usableDimensions.width = x;
                usableDimensions.height = y - barHeight; //DO some calculations first;
                
                is_active = false;
}

auto Monitor::getDimensions() const -> decltype(usableDimensions) {

return usableDimensions;
}


