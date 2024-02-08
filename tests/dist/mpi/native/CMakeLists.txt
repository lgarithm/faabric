add_executable(bench_mpi_allreduce
    main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../benchmarks/mpi_allreduce.cpp
)

target_compile_definitions(bench_mpi_allreduce PRIVATE USE_REAL_MPI=1)
