type t;
let fetchTopStories: unit => list(t);
let view_title: t => string;
let view_id: t => int;
let view_score: t => int;
let view_by: t => string;
let view_time: t => int;
let view_url: t => option(string);