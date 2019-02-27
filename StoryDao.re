type view = {
  /* by: string,
     id: int,
     score: int,
     time: int, */
  title: string,
  /* url: option(string), */
};

type t = view;

let fetchTopStories = () => {
  [];
    /* let json = Yojson.Basic.from_file("./stories.json");
       json |> Yojson.Basic.Util.(to_list |> List.map(storyJson => {title: ""})); */
};

let title = story => story.title;