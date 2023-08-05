drop table Artists cascade;
drop table AlbumTypes cascade;
drop table Albums cascade;
drop table Tracks cascade;
drop table Playlists cascade;
drop table PlaylistTracks cascade;

create table Artists(
    id bigserial primary key,
    name text not null,
    about text,
    coverArt text
);

create table AlbumTypes(
    id bigserial primary key,
    name text not null
);

create table Albums(
    id bigserial primary key,
    name text not null,
    artistsID bigint[] not null,
    coverArt text,
    typeId bigint not null,
    createdAt date
);

alter table Albums add constraint albums_albumtypes_id_fk foreign key (typeId) references AlbumTypes(id);

create table Tracks (
	id bigserial primary key,
    name text not null,
    artistsID bigint[] not null,
	duration bigint not null,
	playCount bigint not null,
	albumID bigint not null,
    trackNumber bigint not null
);

alter table Tracks add constraint tracks_albumid_id_fk foreign key (albumid) references Albums(id);

create table Playlists(
    id bigserial primary key,
    name text not null,
    description text,
    coverArt text
);

create table PlaylistTracks(
    id bigserial primary key,
    playlistID bigint not null,
    trackID bigint not null,
    addedAt timestamp with time zone default CURRENT_TIMESTAMP not null
);

alter table PlaylistTracks add constraint playlisttracks_playlistid_id_fk foreign key (playlistID) references Playlists(id);
alter table PlaylistTracks add constraint playlisttracks_trackid_id_fk foreign key (trackID) references Tracks(id);