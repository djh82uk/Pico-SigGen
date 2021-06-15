add_executable(multicore_runner_queue
        multicore_runner_queue.c
        )

target_link_libraries(multicore_runner_queue
        pico_multicore
        pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(multicore_runner_queue)

