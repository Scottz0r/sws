/**
 * GPIO module for Linux-based Raspberry Pi with 40 pin headers using BCM* chips.
 * 
 * Assumes output pin is on BCM pin 17 (physical board pin 11).
 */
#include "sws_gpio.h"

#include "sws_logger.h"

// Linux specific headers:
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

// Pin 11 Board = Pin 17 BCM
#define FSET_PIN11 17 /* Set */
#define FCLR_PIN11 17 /* Clear */
#define FLVL_PIN11 17 /* Level */
#define FSEL_PIN11 21 /* Function select */
#define FSEL_OUT 1

#define GPIO_MEM_SIZE 0xb4

// GPCLEAR0 is 40 bytes (0x28; 40 / 4 = 10 u32s) from GPIO start.
#define GPCLEAR0_U32_OFF (0x28 / 4)

// GPSET0 is 28 bytes (0x1c; 28 / 4 = 7 u32s) from GPIO start.
#define GPSET0_U32_OFF (0x1c / 4)

// GPLVL0 is 52 bytes (0x34; 52 / 4 = 13 u32s) from GPIO start.
#define GPLVL0_U32_OFF (0x34 / 4)

static volatile u32 *gpio_reg = MAP_FAILED;

SWS_rc_t SWS_gpio_init()
{    
    // Memory map GPIO file descriptor.
    int fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
    
    if (!fd) 
    {
        SWS_log("Failed to open GPIO file descriptor");
        return SWS_ERROR;
    }
    
    // GPIO has last 4 bytes defined at 0xb0, so total size is 0xb4.
    gpio_reg = (uint32_t*)mmap(NULL, GPIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    // Close file descriptor after mmap (regardless of success).
    close(fd);

    if (gpio_reg == MAP_FAILED)
    {
        SWS_log("Failed to map memory");
        return SWS_ERROR;
    }
        
    // Setup pin as output. For Pin 17, it's GPFSEL1 (4 bytes from start)
    u32 fsel_val = FSEL_OUT;
    *(gpio_reg + 1) = (fsel_val << FSEL_PIN11);
    
    return SWS_OK;
}

SWS_gpio_state_t SWS_gpio_get()
{    
    uint32_t levels = *(gpio_reg + GPLVL0_U32_OFF);
    uint32_t is_set = levels & (1 << FLVL_PIN11);
    
    if (is_set > 0)
    {
        return SWS_GPIO_HIGH;
    }
    else
    {
        return SWS_GPIO_LOW;
    }
}

void SWS_gpio_set(SWS_gpio_state_t value)
{
    if (value == SWS_GPIO_LOW)
    {
        // Clear pin.
        *(gpio_reg + GPCLEAR0_U32_OFF) = (1 << FCLR_PIN11);
    }
    else
    {
        // Write 1/high to pin
        *(gpio_reg + GPSET0_U32_OFF) = (1 << FSET_PIN11);
    }
}
