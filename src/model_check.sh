cbmc fridge.c --unwind 18 --function kkv_init --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check 
cbmc fridge.c --unwind 18 --function kkv_destroy --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check
cbmc fridge.c --unwind 18 --function kkv_put --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check
cbmc fridge.c --unwind 18 --function kkv_get --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check
cbmc main.c --unwind 18 --function non_blocking_get --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check
cbmc main.c --unwind 18 --function blocking_get --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check

