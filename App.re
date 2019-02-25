open Revery;
open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;
open Lwt;

module type StoryDaoT = {
  type t;
  let fetchTopStories: unit => Lwt.t(list(string));
  let fetchStory: string => Lwt.t(t);
  let title: t => string;
};

module StoryDao: StoryDaoT = {
  type view = {
    by: string,
    id: int,
    score: int,
    time: int,
    title: string,
    url: option(string),
  };

  type t = view;

  let getJson = url => {
    print_endline("Fetching url: " ++ url);
    Cohttp_lwt_unix.Client.get(Uri.of_string(url))
    >>= (
      ((_response, body)) => {
        Cohttp_lwt.Body.to_string(body)
        >|= (
          string => {
            print_endline(string);
            Yojson.Basic.from_string(string);
          }
        );
      }
    );
  };

  let fetchStory: string => Lwt.t(t) =
    storyId => {
      let storyUrl =
        "https://hacker-news.firebaseio.com/v0/item/"
        ++ storyId
        ++ ".json?print=pretty";
      getJson(storyUrl)
      >|= (
        response =>
          Yojson.Basic.Util.{
            by: member("by", response) |> to_string,
            id: member("id", response) |> to_int,
            score: member("score", response) |> to_int,
            time: member("time", response) |> to_int,
            title: member("title", response) |> to_string,
            url: member("url", response) |> to_string_option,
          }
      );
    };

  let fetchTopStories = () => {
    let topStoriesUrl = "https://hacker-news.firebaseio.com/v0/topstories.json?print=pretty";
    getJson(topStoriesUrl)
    >|= (
      response =>
        Yojson.Basic.Util.(
          to_list(response) |> filter_int |> List.map(string_of_int)
        )
    );
  };

  let title = story => story.title;
};

type story =
  | Fetched(StoryDao.t)
  | NotFetched(string);

module Story = {
  let component = React.component("Story");

  let createElement = (~children as _, ~story: story, ()) =>
    component(hooks => {
      let storyStyle =
        Style.[padding(10), border(~color=Colors.white, ~width=1)];
      let headerStyle =
        Style.[fontFamily("Roboto-Regular.ttf"), fontSize(18)];
      let contentStyle =
        Style.[fontFamily("Roboto-Regular.ttf"), fontSize(12)];
      (
        hooks,
        <View style=storyStyle>
          <View> <Text style=headerStyle text="Story header" /> </View>
          <View> <Text style=contentStyle text="Story content" /> </View>
        </View>,
      );
    });
};

module StoryList = {
  let component = React.component("StoryList");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (stories, setStories, hooks) =
        React.Hooks.state([NotFetched("1"), NotFetched("2")], hooks);
      let hooks =
        React.Hooks.effect(
          OnMount,
          () => {
            print_endline("Fetching stories...");
            let body = Lwt_main.run(StoryDao.fetchTopStories());
            List.map(id => NotFetched(id), body) |> setStories;
            /* StoryDao.fetchTopStories()
               >|= (
                 storyIds => {
                   setStories(List.map(id => NotFetched(id), storyIds));
                   List.iter(
                     id =>
                       StoryDao.fetchStory(id)
                       >|= (
                         story =>
                           print_endline("Fetched " ++ StoryDao.title(story))
                       )
                       |> Lwt_main.run,
                     storyIds,
                   );
                 }
               ); */

            Some(() => print_endline("Unmount"));
          },
          hooks,
        );

      let textHeaderStyle =
        Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(24),
          marginHorizontal(8),
        ];

      (
        hooks,
        <View>
          <Text text="Stories" style=textHeaderStyle />
          {React.listToElement(List.map(story => <Story story />, stories))}
        </View>,
      );
    });
};

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  let containerStyle =
    Style.[
      position(`Absolute),
      justifyContent(`FlexStart),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
      backgroundColor(Colors.black),
    ];

  let render = () => <View style=containerStyle> <StoryList /> </View>;

  UI.start(win, render);
};

App.start(init);