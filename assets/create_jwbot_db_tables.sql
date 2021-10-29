create table if not exists news
(
    id    integer not null
        constraint news_pk
            primary key autoincrement,
    title text    not null,
    date  date    not null,
    url   text default 'http://jw.cqjtu.edu.cn/jxxx/tzgg1.htm' not null
);

create unique index if not exists news_id_uindex
    on news (id);

create table if not exists user_info
(
    qq                 Integer not null
        constraint user_info_pk
            primary key,
    sid                text,
    news_subscriber    integer default 0,
    morning_subscriber integer default 0,
    friend_qq          integer
);

create table if not exists courses
(
    id               integer not null
        constraint courses_pk
            primary key autoincrement,
    qq               integer not null
        references user_info
            on update restrict on delete cascade,
    name             text    not null,
    classroom        text    not null,
    start_time       text    not null,
    end_time         text    not null,
    week_of_semester integer,
    week             integer not null
);

create unique index if not exists courses_id_uindex
    on courses (id);

create index if not exists courses_qq_index
    on courses (qq);

create unique index if not exists user_info_qq_uindex
    on user_info (qq);

