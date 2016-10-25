-module(pregel_client).
-compile(export_all).

connect(MasterHost) ->
	gen_server:start({local, ?MODULE}, ?MODULE, MasterHost, []),
	io:format("Pregel Framework client started.~n").

%=======================================================================================%

init(MasterHost) ->
	Tab = ets:new(?MODULE, []),
	ets:insert(Tab, {host, MasterHost}),
	{ok, Tab}.

run() ->
	run(graph).

run(Module) ->
	gen_server:call(?MODULE, {run_pregel, Module}).

stat() ->
	gen_server:call(?MODULE, {stat}).

ls() ->
	case gen_server:call(?MODULE, {ls}) of
		{ok, String} -> io:format("~n"++beautify(String)++"~n~n");
		Others -> Others
	end.

final() ->
	final(graph).

final(Module) ->
	case gen_server:call(?MODULE, {final, Module}) of
		{ok, String} -> io:format("~n"++beautify(String)++"~n~n");
		Others -> Others
	end.

%=======================================================================================%

beautify(String) ->
	LineList = string:tokens(String, "\n"),
	string:join(LineList, "~n").

%=======================================================================================%

handle_call({final, Module}, _From, Tab) ->
	[{host, MasterHost}] = ets:lookup(Tab, host),
	Result = case rpc:call(MasterHost, pregel_master, job_final, [Module]) of
		{badrpc, nodedown} -> {error, no_master};
		Data -> Data
	end,
	{reply, Result, Tab};

handle_call({stat}, _From, Tab) ->
	[{host, MasterHost}] = ets:lookup(Tab, host),
	Result = case rpc:call(MasterHost, pregel_master, job_stat, []) of
		{badrpc, nodedown} -> {error, no_master};
		Data -> Data
	end,
	{reply, Result, Tab};

handle_call({ls}, _From, Tab) ->
	[{host, MasterHost}] = ets:lookup(Tab, host),
	Result = case rpc:call(MasterHost, pregel_master, job_ls, []) of
		{badrpc, nodedown} -> {error, no_master};
		Data -> Data
	end,
	{reply, Result, Tab};

handle_call({run_pregel, Module}, _From, Tab) ->
	[{host, MasterHost}] = ets:lookup(Tab, host),
	Result = case rpc:call(MasterHost, pregel_master, job_entry, [Module]) of
		{badrpc, nodedown} -> {error, no_master};
		Data -> Data
	end,
	{reply, Result, Tab}.
