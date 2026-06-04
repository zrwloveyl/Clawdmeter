#include "../../hal/imu_hal.h"

// 本板无 IMU；固定 0 度朝向，不做自动旋转。
void    imu_hal_init(void) {}
void    imu_hal_tick(void) {}
uint8_t imu_hal_rotation_quadrant(void) { return 0; }
