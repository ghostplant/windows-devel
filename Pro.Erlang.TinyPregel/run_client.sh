IPS=`ip addr show scope global | grep inet | grep -v inet6 | sed -n 1p`
LOCAL_HOST=`echo $IPS | tr -s 'a-zA-Z/:' ' ' | awk '{print $1}'`

if [[ ( $1 == "" ) ]]; then
        MASTER_HOST=$LOCAL_HOST
else
        MASTER_HOST=$1
fi

erlc pregel_client.erl &&
erl -name client@$LOCAL_HOST -eval "pregel_client:connect('master@$MASTER_HOST')"
