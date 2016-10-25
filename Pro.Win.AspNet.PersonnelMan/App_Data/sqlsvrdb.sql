create table [accessory](
	[psgid] int null,
	[linkpath] varchar(500) null,
	[name] varchar(500) null
)

create table [clarification](
	[orgid] varchar(50) null,
	[time] datetime null,
	[main] varchar(2000) null,
	[destid] varchar(50) null,
	[hasRead] bit null
)

create table [company](
	[information] varchar(8000) null
)

create table [department](
	[name] varchar(50) null,
	[mgrid] varchar(50) null,
	[descr] varchar(50) null
)

create table [download](
	[name] varchar(500) null,
	[lnkpath] varchar(500) null
)

create table [passage](
	[id] int null,
	[title] varchar(50) null,
	[main] varchar(2000) null,
	[usrid] varchar(50) null,
	[time] datetime null,
	[checked] bit null
)

create table [recommend](
	[usrid] varchar(50) null
)

create table [user](
	[id] varchar(50) null,
	[name] varchar(50) null,
	[depid] varchar(50) null,
	[gender] bit null,
	[phone] varchar(50) null,
	[birth] int null,
	[descr] varchar(2000) null,
	[salary] float null,
	[checked] bit null,
	[pswdhash] int null
)
