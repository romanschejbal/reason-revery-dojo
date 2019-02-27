open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Story = {
  let component = React.component("Story");

  let createElement = (~children as _, ~story: StoryDao.t, ()) =>
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
      let (stories, setStories, hooks) = React.Hooks.state([], hooks);
      let hooks =
        React.Hooks.effect(
          OnMount,
          () => Some(() => print_endline("Unmount")),
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