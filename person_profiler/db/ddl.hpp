#pragma once


char const* ddl = R"(
CREATE TABLE measure_group (
    id integer PRIMARY KEY AUTOINCREMENT,  
    name  text NOT NULL,
    active integer NOT NULL,
    weight float NOT NULL -- weight for completion this measure group
);

CREATE TABLE measure (
 id   integer  PRIMARY KEY AUTOINCREMENT,
 name text NOT NULL,
 active int NOT NULL DEFAULT 1,
 type integer NOT NULL, 
 measure_group_id integer NOT NULL,
 comment text NOT NULL,
 FOREIGN KEY (measure_group_id) REFERENCES measure_group(id)
);

CREATE TABLE day_type (
  id integer PRIMARY KEY AUTOINCREMENT,
  name text NOT NULL,
  active integer default 1
);

CREATE INDEX day_type_active ON day_type(active);

CREATE TABLE day(
 id  integer  PRIMARY KEY AUTOINCREMENT,
 day timestamp NOT NULL,
 comment text
);
CREATE UNIQUE INDEX day_timestamp_idx ON day(day);

CREATE TABLE day_type_day (
  day_id integer NOT NULL,
  day_type_id integer NOT NULL,
  FOREIGN KEY (day_id) REFERENCES day(id),
  FOREIGN KEY (day_type_id) REFERENCES day_type(id)
);
CREATE UNIQUE INDEX day_type_day_idx ON day_type_day(day_id);


CREATE TABLE forbidden_measure_group (
    day_id integer NOT NULL,
    measure_group_id integer NOT NULL,
    FOREIGN KEY (day_id) REFERENCES day(id),
    FOREIGN KEY (measure_group_id) REFERENCES measure_group(id)
);

CREATE TABLE options (
    name varchar(128) PRIMARY KEY,
	value text NOT NULL
);

CREATE TABLE estimation
(
 id         integer PRIMARY KEY AUTOINCREMENT,
 started    timestamp NOT NULL,
 expired    timestamp NOT NULL,
 active     integer NOT NULL,
 weight     float NOT NULL,
 border     float NOT NULL,
 reverse    integer NOT NULL,
 measure_id integer NOT NULL,
 next       integer default 0,
 day_type_id integer NOT NULL,
 FOREIGN KEY (measure_id)  REFERENCES measure(id)
);

CREATE INDEX estimation_active ON estimation(active);


CREATE TABLE value (
 id            integer PRIMARY KEY AUTOINCREMENT,
 value         float NOT NULL,
 day_id        integer NOT NULL,
 estimation_id integer NOT NULL,
 FOREIGN KEY (day_id) REFERENCES day(id),
 FOREIGN KEY (estimation_id) REFERENCES estimation(id)
);

CREATE INDEX value_day_id ON value(day_id);


)";