./t > out.log 2> err.log

cat err.log | grep bench_allreduce | tee result.log
