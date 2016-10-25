create table token(
    usrid int
);

create table clients(
	id		varchar(255) PRIMARY KEY,
	name	varchar(255),
	male	bit,
	passhash	int,
	mid	int
);

create table messages(
	scrid	varchar(255) references clients(id),
	desid	varchar(255) references clients(id),
	msg		image,
	cdate	datetime,
	readed	bit,
	type	varchar(255),
);

create table groups(
	id		int identity(1,1) primary key,
	name	varchar(255),
	crtid	varchar(255) references clients(id),
	super	bit
);

create table relations(
	groupid	int references groups(id),
	fndid	varchar(255) references clients(id),
	fndnm	varchar(255),
	primary key(groupid,crtid,fndid)
);

create table request(
	srcid	varchar(255) references clients(id),
	desid	varchar(255) references clients(id),
	groupid	int references groups(id),
	fndnm	varchar(255),
	srcnm	varchar(255),
	primary key(srcid,desid)
);

create table msgfile(
	id		int identity(1,1) primary key,
	scrid	varchar(255) references clients(id),
	desid	varchar(255) references clients(id),
	msg		image,
	fname	varchar(255)
);
