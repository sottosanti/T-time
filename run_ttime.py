import time
start = time.time()
process_time = 0
max_processing_time = 20000
sleep_time = 1800

while process_time < max_processing_time:
    if (time.time() - start) > sleep_time:
        start = time.time()
        service = service_connect() ## Connect to server every 30 minutes
    
    ## Otherwise keep on running this code
    function_which_I_want_to_run_continously()