open BsReactNative;

module Styles = {
  open Style;

  let container =
    style([
      marginTop(Pt(50.0)),
      alignItems(Center),
      justifyContent(Center),
    ]);
};

let component = ReasonReact.statelessComponent("Home");

let make =
    (
      ~navigation: Config.navigationProp,
      ~loginNavigation: Config.loginNavigation,
      _children,
    ) => {
  ...component,
  render: _self =>
    <SafeAreaView>
      <View style=Styles.container>
        <Button
          title="Go to details screen "
          onPress={() => navigation.push(UserDetails("Mike Grabowski"))}
        />
        <Button
          title="Log out"
          onPress={() => loginNavigation.navigate(Login)}
        />
      </View>
    </SafeAreaView>,
};