#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024

void high_priority_thread(void *, void *, void *);
void low_priority_thread(void *, void *, void *);

K_THREAD_STACK_DEFINE(high_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(low_stack, STACK_SIZE);

struct k_thread high_thread_data;
struct k_thread low_thread_data;

K_MUTEX_DEFINE(print_mutex);

static void safe_print(const char *msg)
{
    k_mutex_lock(&print_mutex, K_FOREVER);
    printk("%s\n", msg);
    k_mutex_unlock(&print_mutex);
}

void high_priority_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    volatile uint32_t counter = 0;

    while (1) {
        counter++;

        if ((counter % 5000000) == 0) {
            safe_print("High-priority busy thread is still running");
        }

        /* No sleep here on purpose */
    }
}

void low_priority_thread(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        safe_print("Low-priority periodic thread is running");
        k_sleep(K_SECONDS(2));
    }
}

int main(void)
{
    safe_print("Phase 2 Goal 3: Busy-thread priority experiment");

    k_thread_create(&high_thread_data, high_stack, STACK_SIZE,
                    high_priority_thread,
                    NULL, NULL, NULL,
                    3, 0, K_NO_WAIT);

    k_thread_create(&low_thread_data, low_stack, STACK_SIZE,
                    low_priority_thread,
                    NULL, NULL, NULL,
                    7, 0, K_NO_WAIT);

    while (1) {
        safe_print("Main thread is running");
        k_sleep(K_SECONDS(3));
    }

    return 0;
}