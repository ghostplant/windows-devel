CREATE DATABASE steduman;

CREATE TABLE steduman.Person(
    id                  VARCHAR(255) PRIMARY KEY,
    name                VARCHAR(255),
    male                BIT,
    mobilePhone         VARCHAR(255),
    teacher             BIT
);

CREATE TABLE steduman.Email(
    personId            VARCHAR(255) REFERENCES steduman.Person(id) ON DELETE CASCADE ON UPDATE CASCADE,
    emailAddress        VARCHAR(255) PRIMARY KEY
);

CREATE TABLE steduman.Experience(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    personId            VARCHAR(255) REFERENCES steduman.Person(id) ON DELETE CASCADE ON UPDATE CASCADE,
    startDate           Date,
    endDate             Date,
    detail              VARCHAR(255)
);

CREATE TABLE steduman.Teacher(
    id                  VARCHAR(255) PRIMARY KEY REFERENCES steduman.Person(id),
    department          VARCHAR(255),
    freeze              BIT,
    passwordHashCode    VARCHAR(255),
    officeAddress       VARCHAR(255),
    officePhone         VARCHAR(255),
    major               VARCHAR(255),
    contactAddress      VARCHAR(255),
    birthDate           DATE,
    expertTitle         VARCHAR(255),
    finalDegree         VARCHAR(255),
    finalSchool         VARCHAR(255),
    finalMajor          VARCHAR(255),
    graduationDate      DATE,
    workingSection      VARCHAR(255)
);

CREATE TABLE steduman.Student(
    id                  VARCHAR(255) PRIMARY KEY REFERENCES steduman.Person(id),
    dormNo              VARCHAR(255),
    dormPhone           VARCHAR(255),
    position            VARCHAR(255),
    awards              VARCHAR(255),
    nation              VARCHAR(255),
    degree              VARCHAR(255),
    entranceYear        INT,
    graduationYear      INT,
    teacherId           VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE steduman.Role(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    name                VARCHAR(255)
);

CREATE TABLE steduman.Play(
    teacherId           VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    roleId              INT REFERENCES steduman.Role(id),
    PRIMARY KEY(teacherId,roleId)
);

CREATE TABLE steduman.Authority(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    name                VARCHAR(255),
    function            VARCHAR(255)
);

CREATE TABLE steduman.Access(
    roleId              INT REFERENCES steduman.Role(id) ON DELETE CASCADE ON UPDATE CASCADE,
    authorityId         INT REFERENCES steduman.Authority(id),
    PRIMARY KEY(roleid,authorityId)
);

CREATE TABLE steduman.Achievement(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    name                VARCHAR(255),
    award               VARCHAR(255),
    level               VARCHAR(255),
    certificationId     VARCHAR(255),
    date                DATE,
    type                VARCHAR(255),
    finishCount         INT
 );

CREATE TABLE steduman.TeacherOfAchievement(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    teacherId           VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    achievementId      INT REFERENCES steduman.Achievement(id) ON DELETE CASCADE ON UPDATE CASCADE,
    rank                INT
);

CREATE TABLE steduman.Patent(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    registryNo          VARCHAR(255),
    type                VARCHAR(255),
    name                VARCHAR(255),
    date                DATE,
    finishCount         INT
 );

CREATE TABLE steduman.TeacherOfPatent(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    teacherId           VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    patentId            INT REFERENCES steduman.Patent(id) ON DELETE CASCADE ON UPDATE CASCADE,
    rank                INT
);

-------------- next step ---------->>>

CREATE TABLE steduman.Meeting(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    name                VARCHAR(255),
    holdStartDate       Date,
    holdEndDate         Date,
    totalPeople         INT,
    foreignPeople       INT
);

CREATE TABLE steduman.Essay(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    title               VARCHAR(255),
    style               VARCHAR(255),
    type                VARCHAR(255),
    firstAuthorId       VARCHAR(255) REFERENCES steduman.Person(id) ON DELETE CASCADE ON UPDATE CASCADE,
    memo                VARCHAR(255),
    totalAuthor         INT,

    periodicalName      VARCHAR(255),
    foreignPeriodical   BIT,
    periodicalHolder    VARCHAR(255),
    periodicalIssnNo    VARCHAR(255),
    periodicalCnNo      VARCHAR(255),
    periodicalPublish   VARCHAR(255),

    meetingId           INT REFERENCES steduman.Meeting(id) ON DELETE CASCADE ON UPDATE CASCADE,
    meetingIsbnNo       VARCHAR(255),

    treatisePublisher   VARCHAR(255),
    treatiseBookNo      VARCHAR(255),
    treatiseTotalKWord  INT,
    treatiseSelfKWord   INT
);

CREATE TABLE steduman.MeetingReport(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    teacherId           VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    essayId             INT REFERENCES steduman.Essay(id) ON DELETE CASCADE ON UPDATE CASCADE,
    meetingId           INT REFERENCES steduman.Meeting(id) ON DELETE CASCADE ON UPDATE CASCADE,
    reportDate          Date
);

CREATE TABLE steduman.Cultivation(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    studentId           VARCHAR(255) REFERENCES steduman.Student(id) ON DELETE CASCADE ON UPDATE CASCADE,
    date                Date,
    essayTitle          VARCHAR(255),
    major               VARCHAR(255),
    subject             VARCHAR(255)
);


CREATE TABLE steduman.Project(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    name                VARCHAR(255),
    number              VARCHAR(255),
    source              VARCHAR(255),
    type                VARCHAR(255),
    startYear           INT,
    endYear             INT,
    cost                FLOAT,
    status              VARCHAR(255)
);

CREATE TABLE steduman.TeacherOfProject(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    projectId           INT REFERENCES steduman.Project(id) ON DELETE CASCADE ON UPDATE CASCADE,
    teacherId           VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    rank                INT
);

CREATE TABLE steduman.Course(
    id                  VARCHAR(255) PRIMARY KEY,
    name                VARCHAR(255),
    creditValue         FLOAT,
    creditHour          INT,
    target              VARCHAR(255),
    type                VARCHAR(255),
    established         BIT
);

CREATE TABLE steduman.TeacherOfCourse(
    teacherId           VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    courseId            VARCHAR(255) REFERENCES steduman.Course(id) ON DELETE CASCADE ON UPDATE CASCADE,
    PRIMARY KEY(teacherId,courseId)
);

CREATE TABLE steduman.PersonOfEssay(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    personId            VARCHAR(255) REFERENCES steduman.Person(id) ON DELETE CASCADE ON UPDATE CASCADE,
    essayId             INT REFERENCES steduman.Essay(id) ON DELETE CASCADE ON UPDATE CASCADE,
    rank                INT
);

CREATE TABLE steduman.Fund(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    fundNo              VARCHAR(255),
    essayId             INT REFERENCES steduman.Essay(id) ON DELETE CASCADE ON UPDATE CASCADE,
    fundName            VARCHAR(255)
);

CREATE TABLE steduman.Cooperation(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    name                VARCHAR(255),
    startYear           INT,
    endYear             INT,
    target              VARCHAR(255)
);

CREATE TABLE steduman.Laboratory(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    name                VARCHAR(255),
    personnelCount      INT,
    deviceCount         INT,
    deviceCost          FLOAT
);

CREATE TABLE steduman.Device(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    laboratoryId        INT REFERENCES steduman.Device(id) ON DELETE CASCADE ON UPDATE CASCADE,
    name                VARCHAR(255)
);

CREATE TABLE steduman.Library(
    id                  INT PRIMARY KEY DEFAULT 1,
    databaseTypeAmount  INT,
    fgnStoreAmount      FLOAT,
    chsStoreAmout       FLOAT,
    fgnPeriodicalAmount FLOAT,
    chsPeriodicalAmount FLOAT,
    fgnEBookAmount      FLOAT,
    chsEBookAmount      FLOAT,
    fgnEPeriodicalAmount FLOAT,
    chsEPeriodicalAmount FLOAT
);

CREATE TABLE steduman.Subscribe(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    type                VARCHAR(255),
    name                VARCHAR(255),
    subscribeYear       INT
);

CREATE TABLE steduman.Message(
    id                  INT PRIMARY KEY AUTO_INCREMENT,
    date                DATE,
    srcTeacherId        VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    dstTeacherId        VARCHAR(255) REFERENCES steduman.Teacher(id) ON DELETE CASCADE ON UPDATE CASCADE,
    message             VARCHAR(255),
    readed              BIT
);

INSERT INTO steduman.person (id, `name`, male, `mobilePhone`, teacher) 
	VALUES ('admin', '张三', true, '159XXXXXXXX', true);
INSERT INTO steduman.teacher (id, department, freeze, `passwordHashCode`, `officeAddress`, `officePhone`, major, `contactAddress`, `birthDate`, `expertTitle`, `finalDegree`, `finalSchool`, `finalMajor`, `graduationDate`, `workingSection`) 
	VALUES ('admin', '', false, '21232F297A57A5A743894A0E4A801FC3', '', '', '', '', '2011-09-07', '', '', '', '', '2011-09-13', '');

INSERT INTO steduman.authority (`name`, `function`) VALUES ('新建用户权', 'userdoc#adduser');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('删除用户权', 'userdoc#deleteuser');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('冻结用户权', 'userdoc#freezeuser');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('解冻用户权', 'userdoc#unfreezeuser');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('批量导入用户权', 'userdoc#uploaduser');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('修改用户信息权', 'userdoc#edituser');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('修改密码权', 'userdoc#changepassword');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('新建权限权', 'roleauth#addauthority');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('查询权限权', 'roleauth#queryauthority');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('删除权限权', 'roleauth#removeauthority');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('新建角色权', 'roleauth#addrole');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('查询角色权', 'roleauth#queryrole');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('删除角色权', 'roleauth#removerole');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('角色分配权限权', 'roleauth#allocauthority');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('用户分配角色权', 'roleauth#allocrole');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('导出用户信息权', 'userdoc#exportuser');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('教学成果录入权', 'achievement#teachingachievement');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('科研成果录入权', 'achievement#researchachievement');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('论文专著录入权', 'achievement#essayachievement');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('著作专利录入权', 'achievement#patentachievement');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('优秀人才培养录入权', 'achievement#cultivationachievement');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('成果查询权', 'achievement#achievementquery');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('导出成果权', 'achievement#exportachievement');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('添加教学科研项目权', 'research#addproject');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('更改教学科研项目权', 'research#changeproject');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('查询教学科研项目权', 'research#queryproject');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('人员结构与学术骨干管理权', 'research#researcher');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('添加课程权', 'course#addcourse');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('变更课程权', 'course#changecourse');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('查询课程权', 'course#querycourse');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('显示学生信息权', 'talents#showstuinfo');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('查询学生信息权', 'talents#querystuinfo');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('更改学生信息权', 'talents#changestuinfo');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('下载学生信息权', 'talents#download');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('主要国际国内学术会议管理权', 'academic#insertacademicmeeting');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('学术交流活动申请管理权', 'academic#appacademicactivity');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('国际交流合作项目管理权', 'academic#internationcooperation');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('工作条件管理权', 'academic#workingcondition');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('图书资料情况管理权', 'academic#libraryorder');
INSERT INTO steduman.authority (`name`, `function`) VALUES ('文件文档上传下载管理权', 'document#documentsmanager');

INSERT INTO steduman.`role` (`name`) VALUES ('系统管理员');

INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 1);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 2);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 3);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 4);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 5);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 6);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 7);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 8);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 9);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 10);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 11);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 12);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 13);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 14);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 15);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 16);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 17);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 18);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 19);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 20);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 21);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 22);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 23);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 24);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 25);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 26);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 27);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 28);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 29);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 30);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 31);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 32);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 33);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 34);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 35);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 36);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 37);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 38);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 39);
INSERT INTO steduman.access (`roleId`, `authorityId`) VALUES (1, 40);

INSERT INTO steduman.play (`teacherId`, `roleId`) VALUES ('admin', 1);
