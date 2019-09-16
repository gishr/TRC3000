//
// This code has been transcribed from 
// http://elinux.org/Interfacing_with_I2C_Devices
// for TRC3000, S2 2018
//
// CMakeLists.txt
// cmake_minimum_required(VERSION 2.8)
// project( imu )
// add_executable( imu imu.c )
// target_link_libraries( imu m )
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

#define PI 3.1415926

void main(void) {
    int file;
    char filename[40];
    unsigned char mode_change[2] = {0x00, 0x00}; // buffer for reg addr/value 
    unsigned char read_buffer[20] = {0x00};
    unsigned char start_addr = 0x22; 
    struct timespec time_tag;
 
    int addr = 0x6B;        // The I2C address of the Accelerometer/Gyro
    int m_addr = 0x1E;      // The I2C address of the Magnetometer
    int idx;
    short Ax, Ay, Az, Gx, Gy, Gz, Mx, My, Mz;

    sprintf(filename,"/dev/i2c-1"); // using i2c-1 bus 
    if ((file = open(filename,O_RDWR)) < 0) {
        printf("Failed to open the bus.");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    else 
	printf("%s is now open successfully\n", filename);

    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    else 
	printf("I2C bus access to LSM6DS33 is acquired\n");
	
	mode_change[0] = 0x10; // CTRL1_XL address
	mode_change[1] = 0x80; // 

    if (write(file,mode_change,2) != 2) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to mode change.\n");
        exit(1);
    }
    else
	printf("Accelerometer enabled at 1.66KHz mode.\n");

        mode_change[0] = 0x11; // CTRL2_G address
        mode_change[1] = 0x80; // 1.66KHz sampling

    if (write(file,mode_change,2) != 2) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to mode change.\n");
        exit(1);
    }
    else
        printf("Gyro enabled at 1.66KHz mode.\n");

 
    usleep(100000);

    for (idx = 0;idx < 5000; idx++)
    {

    	if (write(file,&start_addr,1) != 1) {
        	/* ERROR HANDLING: i2c transaction failed */
        	printf("Failed to change address pointer to 0x03.\n");
        	exit(1);
    	}

	if (read(file, read_buffer, 12) !=12 )
	{
		printf("Something's wrong, received data length is not 12\n");
	}
	else 
	{
		clock_gettime(CLOCK_MONOTONIC, & time_tag);

                Gx = (short)(read_buffer[1] << 8 | read_buffer[0]);
                Gy = (short)(read_buffer[3] << 8 | read_buffer[2]);
                Gz = (short)(read_buffer[5] << 8 | read_buffer[4]);

		Ax = (short)(read_buffer[7] << 8 | read_buffer[6]);
		Ay = (short)(read_buffer[9] << 8 | read_buffer[8]);
		Az = (short)(read_buffer[11]<< 8 | read_buffer[10]);

                printf("%6d %6d %6d %6d %6d %6d %lu\n", Ax, Ay, Az, Gx, Gy, Gz, time_tag.tv_nsec); // atan2(Hy,Hx)*180/PI);

	}

    } // end of for

}

