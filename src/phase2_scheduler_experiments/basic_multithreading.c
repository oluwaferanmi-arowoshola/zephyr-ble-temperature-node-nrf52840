#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024
#define PRIORITY 5

void thread1_function(void *, void *, void *);
void thread2_function(void *, void *, void *);

K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);

struct k_thread thread1_data;
struct k_thread thread2_data;

void thread1_function(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        printk("Thread 1 is running\n");
        k_sleep(K_SECONDS(1));
    }
}

void thread2_function(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    while (1) {
        printk("Thread 2 is running\n");
        k_sleep(K_SECONDS(2));
    }
}

int main(void)
{
    printk("Phase 2 Goal 3: Zephyr threads, sleep, and printk\n");

    k_thread_create(&thread1_data, thread1_stack, STACK_SIZE,
                    thread1_function,
                    NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    k_thread_create(&thread2_data, thread2_stack, STACK_SIZE,
                    thread2_function,
                    NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    while (1) {
        printk("Main thread is running\n");
        k_sleep(K_SECONDS(3));
    }

    return 0;
}