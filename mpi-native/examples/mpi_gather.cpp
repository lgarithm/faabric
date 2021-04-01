#include <faabric/mpi/mpi.h>
#include <stdio.h>

#include <faabric/mpi-native/MpiExecutor.h>
#include <faabric/util/logging.h>
int main(int argc, char** argv)
{
    auto logger = faabric::util::getLogger();
    auto& scheduler = faabric::scheduler::getScheduler();
    auto& conf = faabric::util::getSystemConfig();

    bool __isRoot;
    int __worldSize;
    if (argc < 2) {
        logger->debug("Non-root process started");
        __isRoot = false;
    } else if (argc < 3) {
        logger->error("Root process started without specifying world size!");
        return 1;
    } else {
        logger->debug("Root process started");
        __worldSize = std::stoi(argv[2]);
        __isRoot = true;
        logger->debug("MPI World Size: {}", __worldSize);
    }

    // Pre-load message to bootstrap execution
    if (__isRoot) {
        faabric::Message msg = faabric::util::messageFactory("mpi", "exec");
        msg.set_mpiworldsize(__worldSize);
        scheduler.callFunction(msg);
    }

    {
        faabric::executor::SingletonPool p;
        p.startPool();
    }

    return 0;
}

int faabric::executor::mpiFunc()
{
    MPI_Init(NULL, NULL);

    int rank;
    int worldSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    // Build an array the right size
    int nPerRank = 4;
    int n = worldSize * nPerRank;

    // Set up containers in root
    int* expected = nullptr;
    int* actual = nullptr;
    int root = 2;
    if (rank == root) {
        expected = new int[n];
        for (int i = 0; i < n; i++) {
            expected[i] = i;
        }

        actual = new int[n];
    }

    // Build the data chunk for this rank
    int startIdx = rank * nPerRank;
    int* thisChunk = new int[nPerRank];
    for (int i = 0; i < nPerRank; i++) {
        thisChunk[i] = startIdx + i;
    }

    MPI_Gather(thisChunk,
               nPerRank,
               MPI_INT,
               actual,
               nPerRank,
               MPI_INT,
               root,
               MPI_COMM_WORLD);

    if (rank == root) {
        for (int i = 0; i < n; i++) {
            if (actual[i] != expected[i]) {
                printf("Gather failed!\n");
                return 1;
            }
        }
        printf("Gather as expected\n");
    }

    delete[] thisChunk;
    delete[] actual;
    delete[] expected;

    MPI_Finalize();

    return 0;
}