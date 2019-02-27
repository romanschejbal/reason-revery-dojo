open Yojson.Basic.Util;

type view = {
  by: string,
  id: int,
  score: int,
  time: int,
  title: string,
  url: option(string),
};

type t = view;

type story =
  | Fetched(t)
  | NotFetched(int);

let fetchTopStories = () => {
  let json = Yojson.Basic.from_file("./stories.json");
  json
  |> to_list
  |> List.map(storyDetail =>
       Fetched({
         by: member("by", storyDetail) |> to_string,
         id: member("id", storyDetail) |> to_int,
         title: member("title", storyDetail) |> to_string,
         score: member("score", storyDetail) |> to_int,
         time: member("time", storyDetail) |> to_int,
         url: member("url", storyDetail) |> to_string_option,
       })
     );
};

let view_title = story => story.title;
let view_by = story => story.by;
let view_id = story => story.id;
let view_time = story => story.time;
let view_url = story => story.url;
let view_score = story => story.score;