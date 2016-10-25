USER_SPACE=`pwd`/pregel_job

IPS=`ip addr show scope global | grep inet | grep -v inet6 | sed -n 1p`
LOCAL_HOST=`echo $IPS | tr -s 'a-zA-Z/:' ' ' | awk '{print $1}'`

erlc pregel_master.erl
erl -name master@$LOCAL_HOST -eval "pregel_master:start(\"$USER_SPACE\")"
