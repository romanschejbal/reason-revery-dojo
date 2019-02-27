open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;
open Sys;

module URLStoryStory = {
  let component = React.component("URLStoryStory");

  let createElement =
      (~children as _, ~url: option(string), ~label: string, ()) =>
    component(hooks =>
      (
        hooks,
        switch (url) {
        | Some(url) =>
          <Clickable onClick={() => command("open " ++ url) |> ignore}>
            <Text
              style=Style.[fontFamily("Roboto-Regular.ttf"), fontSize(18)]
              text=label
            />
          </Clickable>
        | None => <View />
        },
      )
    );
};

module Story = {
  let component = React.component("Story");

  let createElement = (~children as _, ~story, ()) =>
    component(hooks => {
      let storyStyle =
        Style.[
          padding(10),
          border(
            ~color=
              StoryDao.view_url(story) == None ? Colors.white : Colors.black,
            ~width=1,
          ),
        ];
      let headerStyle =
        Style.[fontFamily("Roboto-Regular.ttf"), fontSize(18)];
      (
        hooks,
        <View style=storyStyle>
          <View>
            <Text style=headerStyle text={StoryDao.view_title(story)} />
          </View>
          <View>
            <URLStoryStory url={StoryDao.view_url(story)} label="Click me" />
          </View>
        </View>,
      );
    });
};

module StoryList = {
  let component = React.component("StoryList");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (stories, setStories, hooks) = React.Hooks.state([], hooks);
      let hooks =
        React.Hooks.effect(
          OnMount,
          () => {
            setStories(StoryDao.fetchTopStories());
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
          <View
            style=Style.[
              height(100),
              backgroundColor(Color.hex("#ff3e00")),
            ]>
            <Text text="Stories" style=textHeaderStyle />
          </View>
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
      backgroundColor(Colors.gray),
    ];

  let render = () => <View style=containerStyle> <StoryList /> </View>;

  UI.start(win, render);
};

App.start(init);