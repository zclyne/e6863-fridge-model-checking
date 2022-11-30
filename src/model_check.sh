cbmc fridge.c --unwind 18 --function kkv_init --trace
cbmc fridge.c --unwind 18 --function kkv_destroy --trace
cbmc fridge.c --unwind 18 --function kkv_put --trace
cbmc fridge.c --unwind 18 --function kkv_get --trace
cbmc main.c --unwind 18 --function non_blocking_get --trace
cbmc main.c --unwind 18 --function blocking_get --trace

